from fastapi import FastAPI, Body, Response
from fastapi.middleware.cors import CORSMiddleware

from .degree_planner.planner import Planner
from .degree_planner.math.dictionary_array import Dict_Array
from .degree_planner.dp.requirement import Requirement
from .degree_planner.user.schedule import Schedule

from .redis_manipulations import get_redis_status, purify, get_schedule, save_schedule, delete_schedule
from .celery_app import dp_recommend, dp_fulfill, dp_fulfill_groups, dp_fulfill_details, dp_groups

import time
import asyncio
import uuid
import hashlib

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:8080"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
planner = Planner(enable_tensorflow=False)
planner.import_data()

hash_heads = set()
hash_heads_queue = []
MAX_HASH_HEADS = 1000
difficulty = '000'

recommendation_results = dict() # {user: results}
fulfillment_detail_results = dict() # {user: details}

def rate_limited(seconds_interval):
    def decorator(func):
        call_history = {}
        add_history = {}
        async def rate_limited_function(userid, *args, **kwargs):
            time_added = time.time()
            add_history.update({userid:time_added})
            time_elapsed = time.time() - call_history.get(userid, 0.0)
            time_wait = seconds_interval - time_elapsed

            debug_time = time.time()

            if time_wait > 0:
                print(f'rate limited!')
                await asyncio.sleep(time_wait)

            if add_history.get(userid) != time_added:
                # means another call occurred while waiting, so cancel this one
                print(f'cancelling function call sent at time {debug_time}')
                return

            print(f'executing function call made at time {debug_time}')
            
            call_history.update({userid:time.time()})
            return await func(userid, *args, **kwargs)
        return rate_limited_function
    return decorator

def is_admin_user(session):
    if 'user' in session and (session['user']['admin'] or session['user']['super_admin']):
        return True
    return False



##########################################################################
##########################################################################
'''=======================DELETE BEFORE MERGING========================'''

@app.get('/api/dp/redisstat')
async def dp_redis_status():
    return get_redis_status()

@app.get('/api/dp/redispurify')
async def dp_redis_purify():
    await purify()

'''===================================================================='''
##########################################################################
##########################################################################



async def dp_redis_delete_schedule(scheduleid):
    await delete_schedule(scheduleid)


@app.post("/api/setsemester")
async def set_semester(userid:str = Body(...), semester:str = Body(...)):
    pass


@app.get("/api/dp/newuser")
async def dp_new_user():
    # Generate a token and userid
    global hash_heads
    global hash_heads_queue

    hashhead = str(uuid.uuid4())
    while hashhead in hash_heads:
        print(f'AN ASTRONOMICALLY UNLIKELY EVENT HAS OCCURRED (uuid4 collision)')
        hashhead = str(uuid.uuid4())
    hash_heads.add(hashhead)
    hash_heads_queue.append(hashhead)

    if len(hash_heads_queue) > MAX_HASH_HEADS:
        for discardedHash in hash_heads_queue[:int(MAX_HASH_HEADS / 2)]:
            hash_heads.remove(discardedHash)
        hash_heads_queue = hash_heads_queue[int(MAX_HASH_HEADS / 2):]
    global difficulty
    # print(f'beginning new session with hashhead {hashhead}')
    return {'hashhead': hashhead, 'difficulty': difficulty}

# a dead simple rate limiter that will probably give a security analyst a stroke
def valid_hash(hashval:str):
    if len(hashval) < 36:
        return False
    if hashval[0:36] not in hash_heads:
        return False

    input_bytes = hashval.encode('utf-8')
    return (hashlib.sha256(input_bytes).hexdigest()[0:len(difficulty)] == difficulty)

@app.get("/api/dp/validateid/{userid}")
async def dp_validate_id(userid:str):
    print(f'user attempted to validate id {userid}: {valid_hash(userid)}! current valid heads: {hash_heads}')
    return {'valid': valid_hash(userid)}

@app.get('/api/dp/info')
async def dp_info():
    print(f"DEGREES: {planner.degrees()}")
    return {"degrees":planner.degrees()}

@app.get('/api/dp/getschedule/{scheduleid}')
async def dp_get_schedule(scheduleid:str):
    # dictionary of schedule data
    schedule_data = await get_schedule(scheduleid)
    return schedule_data

@app.post('api/dp/saveschedule')
async def dp_save_schedule(schedule_data:dict = Body(...)):
    # verify schedule_data structure: {'name':schedule_name, 'courses':course_names, 'degree':degree_name}
    if not len(schedule_data) == 3:
        return False
    if not isinstance(str, schedule_data.get('name', None)):
        return False
    if not isinstance(list, schedule_data.get('courses', None)):
        return False
    for course in schedule_data.get('courses'):
        if not isinstance(str, course):
            return False
    if not isinstance(str, schedule_data.get('degree', None)):
        return False
    
    save_schedule(schedule_data.get('name'), schedule_data)

@app.post('/api/dp/fulfillment')
async def dp_get_fulfillment(userid:str = Body(...), degree_name:str = Body(...), taken_courses:list = Body(...), attributes_replacement:dict = Body(...)):
    if not valid_hash(userid):
        return None
    
    io = planner.output

    wildcard_resolutions = Dict_Array(attributes_replacement, list_type='list')
    degree = planner.get_degree(degree_name)
    if degree is None:
        print(f'NO DEGREE')
        return None
    print(f"TEMPLATES: {planner.catalog.get_templates()}")
    requirements = degree.requirements

    taken_courses_convert_to_elements = []
    for course in taken_courses:
        element = planner.catalog.get_element(course)
        if element is None:
            continue
        taken_courses_convert_to_elements.append(element)
    taken_courses = list(taken_courses_convert_to_elements)
    templates = list(planner.catalog.get_templates())

    fulfillment = dp_fulfill(taken_courses, requirements, wildcard_resolutions, [], False, templates, degree.group_attributes)
    formatted_fulfillments = io.format_fulfillments_dict(fulfillment['fulfillments'], taken_courses)
    
    # BEGIN DETAILS COMPUTATION ----
    await dp_begin_fulfillment_details(userid, taken_courses, planner.catalog, requirements, wildcard_resolutions)
    #-------------------------------

    #fulfillment_groups_and_tally = dp_fulfill_groups(fulfillment, groups) # contains groups and tally
    fulfillment_groups_and_tally = dict()
    fulfillment_groups_and_tally.update({'groups': fulfillment['groups']}) # formats groups
    fulfillment_groups_and_tally.update({'combos': fulfillment['combos']})
    fulfillment_groups_and_tally.update({'fulfillments': formatted_fulfillments}) # adds fulfillments

    return fulfillment_groups_and_tally

@rate_limited(3)
async def dp_begin_fulfillment_details(userid:str, taken_courses, catalog, requirements, wildcard_resolutions):
    details = dp_fulfill_details.delay(taken_courses, catalog, requirements, wildcard_resolutions)
    fulfillment_detail_results.update({userid:details})


@app.get('/api/dp/fulfillmentdetails/{userid}')
async def dp_get_fulfillment_details(userid:str):
    if not valid_hash(userid):
        return None
    
    i = 0
    while(fulfillment_detail_results.get(userid, None) is None or not fulfillment_detail_results.get(userid).ready()):
        await asyncio.sleep(0.5)
        print(f'waiting for fulfillment details...  queued: {fulfillment_detail_results.get(userid, None) is not None}')
        if fulfillment_detail_results.get(userid, None) is None:
            return None
        i+=1
        if i > 20:
            print('timeout')
            return None
    
    details = fulfillment_detail_results.get(userid).result
    return details

@rate_limited(3)
async def dp_begin_recommendation_limited(userid:str, degree_name:str, taken_courses:list, attributes_replacement:dict):
    degree = planner.get_degree(degree_name)
    if degree is None:
        return
    requirements = degree.requirements
    wildcard_resolutions = Dict_Array(attributes_replacement, list_type='list')

    taken_courses_convert_to_elements = []
    for course in taken_courses:
        element = planner.catalog.get_element(course)
        if element is None:
            continue
        taken_courses_convert_to_elements.append(element)
    taken_courses = taken_courses_convert_to_elements

    recommendation = dp_recommend.delay(taken_courses, planner.catalog, requirements, specification_sets=Requirement.specification_sets, attributes_replacement=wildcard_resolutions)
    recommendation_results.update({userid:recommendation})
    
@app.post('/api/dp/recommend')
async def dp_begin_recommendation(userid:str = Body(...), degree_name:str = Body(...), taken_courses:list = Body(...), attributes_replacement:dict = Body(...)):
    if not valid_hash(userid):
        return
    await dp_begin_recommendation_limited(userid, degree_name, taken_courses, attributes_replacement)

@app.get('/api/dp/recommend/{userid}')
async def dp_get_recommendation(userid:str):
    if not valid_hash(userid):
        return None
    
    io = planner.output
    i = 0
    while(recommendation_results.get(userid, None) is None or not recommendation_results.get(userid).ready()):
        await asyncio.sleep(1)
        print(f'waiting for recommendation...  queued: {recommendation_results.get(userid, None) is not None}')
        if recommendation_results.get(userid, None) is None:
            return None
        i+=1
        if i > 40:
            print('timeout')
            return None
    
    recommendation = recommendation_results.get(userid).result
    formatted_recommendations = io.format_recommendations(recommendation)
    #print(f'RECOMMENDATIONS: {formatted_recommendations}')
    results = Dict_Array(list_type='list')

    # formatted recommendations is a list of dictionaries with each dictionary being a recommendation, containing
    # entries such as name and fulfillment set
    for recommendation in formatted_recommendations:
        results.add(recommendation['name'].casefold(), recommendation)

    results.sort_elements('get', ("courses_fulfilled",), True)
    return results.dictionary

@app.get('/api/dp/courses/{lowercase}')
async def dp_get_courses(lowercase:bool):
    courses = list(planner.catalog.get_elements())
    if lowercase:
        courses = [course.name.casefold() for course in courses]
    else:
        courses = [course.name for course in courses]
    courses.sort()
    return courses

@app.get('/api/dp/subjectgroups')
async def dp_get_subject_groups():
    return planner.subject_groups

@app.get('/')
async def root(request):
    return Response(content='Degree Senpai API is online!',)
