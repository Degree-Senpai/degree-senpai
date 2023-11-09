export function formatSelectedCourses(selectedCourses) {
    let groupedCourses = {};
    for (const course of selectedCourses) {
        //let fastInstance = new FastCourseInstance(course.crn, course.timeblocks);
        if (groupedCourses[course.name] != null) {
            groupedCourses[course.name].push(course);
        }
        else {
            groupedCourses[course.name] = [course];
        }
    }
    return groupedCourses;
}

export function scheduleFill(selectedGroupedCourseInstances, maxDepth=8) {
    let schedules = [defaultSchedule()]; // schedule objects
    for (const courseName in selectedGroupedCourseInstances) {
        let newSchedules = [];
        for (let schedule of schedules) {
            for (const courseInstance of selectedGroupedCourseInstances[courseName]) {
                let clone = Object.assign(Object.create(Object.getPrototypeOf(schedule)), schedule);
                clone.proof++;
                console.log(`clone: ${JSON.stringify(clone)}, original: ${JSON.stringify(schedule)}`);
                let collisions = scheduleAddCourse(clone, courseInstance);
                if (collisions > maxDepth) {
                    continue;
                }
                newSchedules.push(clone);
                console.log(`appending courseinstance ${courseInstance.crn} success, current collisions: ${collisions}`)
            }
        }
        schedules = newSchedules;
        console.log(`returning schedules ${JSON.stringify(schedules)}`)
    }
    return schedules
}

function makeSchedule(courseInstances, numCollisions=0, days=5, hours=12) {
    return {
        days: days,
        hours: hours,
        courseInstances: courseInstances,
        numCollisions: numCollisions,
        proof: 0
    }
}

function defaultSchedule() {
    const days = 5;
    const hours = 12;
    return makeSchedule([], 0, days, hours)
}

function scheduleAddCourse(schedule, course) {
    schedule.numCollisions += courseCollisions(schedule, course)
    schedule.courseInstances.push(course);
    return schedule.numCollisions;
}

export function courseCollisions(schedule, courseAdding) {
    let collisions = 0
    for (const course of schedule.courseInstances) {
        for (const timeblock of course.timeblocks) {
            let breakloop = false;
            for (const addingTimeBlock of courseAdding.timeblocks) {
                if (addingTimeBlock.day != timeblock.day) {
                    continue
                }
                let width = Math.max((addingTimeBlock.end - timeblock.begin), (timeblock.end - addingTimeBlock.begin));
                // console.log(`width: ${width}, total length: ${timeblock.length + addingTimeBlock.length}`)
                if (width < (timeblock.length + addingTimeBlock.length)) {
                    // console.log(`appending timeblock ${JSON.stringify(addingTimeBlock)} caused collision with ${JSON.stringify(timeblock)}`)
                    collisions++;
                    breakloop = true;
                    break
                }
            }
            if (breakloop) {
                break
            }
        }
    }
    return collisions
}
