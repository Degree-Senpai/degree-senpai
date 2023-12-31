import timeit
from functools import lru_cache
from .catalog import Catalog
from .fulfillment_status import Fulfillment_Status
from ..math.sorting import sorting
from ..recommender.recommender import Recommender
from .fulfill import get_branched_element_match, get_optimized_fulfillment, get_element_match

@lru_cache(maxsize=100)
def recommend(elements_selected, catalog:Catalog, requirements, custom_tags=None, enable_tensorflow=False, attributes_replacement=None) -> dict:
    '''
    gives possible courses to take

    returns: recommendation (dict): {best template : {alternative template : fulfillment list}}
    '''
    if custom_tags is not None and not len(custom_tags):
        custom_tags = None

    start = timeit.default_timer()
    if Recommender.cache is None:
        print(f' RECOMMENDER --- BUILDING RECOMMENDER')
        Recommender.initialize(catalog)
        Recommender.ENABLE_TENSORFLOW = enable_tensorflow
        print(f' RECOMMENDER --- FINISHED BUILDING RECOMMENDER')
    recommendation = dict() # {best template : {alternative template : fulfillment list}}
    # note that best template == alternative template if best template does not contain wildcards

    fulfillment = get_optimized_fulfillment(elements_selected, requirements, attributes_replacement, relevant_templates=catalog.get_templates())

    for requirement in fulfillment['fulfillments'].keys():
        print(f'  RECOMMENDER --- SOLVING requirement {requirement}')
        # here we receive the list of fulfillment sets from get course match
        
        matched_fulfillment = get_element_match(requirement, catalog.get_elements())
        matches_and_courses = []

        matched_fulfillment:Fulfillment_Status

        # remove the elements already selected
        elements_recommended = matched_fulfillment.fulfillment_set
        elements_fulfilled = 0
        for element in elements_selected:
            if element in elements_recommended:
                elements_recommended.discard(element)
                elements_fulfilled += 1

        matched_fulfillment.requirement.elements_fulfilled = elements_fulfilled
        matched_fulfillment.requirement.importance = elements_fulfilled

        # course_R_bindings = num_bindings(max_fulfillments, recommended_courses, Bind_Type.R)
        course_relevances = Recommender.embedded_relevance(elements_selected, elements_recommended, catalog.tags, custom_tags)

        final_score = dict()
        for element in elements_recommended:
            score = course_relevances.get(element)
            # score += (course_R_bindings.get(course) / 50.0)
            final_score.update({element : score})

        elements_recommended = sorting.dictionary_sort(final_score)
        matches_and_courses.append([matched_fulfillment.requirement,elements_recommended])

        recommendation.update({requirement:matches_and_courses})

    end = timeit.default_timer()
    print(f'\r---------------------------------------recommendation runtime: {end - start}\n')

    return recommendation
