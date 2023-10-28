import { FastCourseInstance } from "./coursedata";

export class Schedule {
    constructor(data = null, courseInstances = null) {
        this.days = 5;
        this.hours = 12;
        this.blocksPerHour = 2;
        
        this.bb = Array(this.days * this.hours * this.blocksPerHour).fill(0);
        this.numCollisions = 0;

        if (data == null) {
            this.data = Array(this.days).fill([[]]);
            this.courseInstances = [];
        }
        else {
            this.data = data;
            this.courseInstances = courseInstances;
        }
        if (this.courseInstances == null) {
            this.computeCourseInstances();
        }
    }

    addCourse(day, column, course) {
        this.data[day][column].push(course);
        this.courseInstances.push(course);
        if (this.updateBB(course)) {
            // updateBB returns whether there was a collision or not
            ++this.numCollisions;
        }
        return this.numCollisions;
    }

    numColumns(day) {
        return this.data[day].length;
    }

    computeCourseInstances() {
        // use only in constructor
        this.courseInstances = [];
        for (const dayData of this.data) {
            for (const columnData of dayData) {
                for (const course of columnData) {
                    this.courseInstances.push(course);
                }
            }
        }
    }

    makeBB() {
        for (const course of this.courseInstances) {
            for (let i = 0; i < this.days * this.hours * this.blocksPerHour; ++i) {
                this.bb[i] = this.bb[i] + course.bb[i]
            }
        }
    }

    updateBB(course) {
        let collided = 0;
        for (let i = 0; i < this.days * this.hours * this.blocksPerHour; ++i) {
            this.bb[i] = this.bb[i] + course.bb[i];
            collided = collided | (this.bb[i] & course.bb[i])
        }
        return collided;
    }
}

export function formatSelectedCourses(selectedCourses) {
    let groupedCourses = {};
    for (const course of selectedCourses) {
        let fastInstance = new FastCourseInstance(course.crn, course.timeblocks);
        if (groupedCourses[course.name] != null) {
            groupedCourses[course.name].push(fastInstance);
        }
        else {
            groupedCourses[course.name] = [fastInstance];
        }
    }
    return groupedCourses;
}

export function scheduleFill(selectedGroupedCourseInstances, maxDepth=3) {
    let schedules = [defaultSchedule()]; // schedule objects
    for (const courseName in selectedGroupedCourseInstances) {
        let newSchedules = [];
        for (let schedule of schedules) {
            for (const courseInstance of selectedGroupedCourseInstances[courseName]) {
                let clone = Object.assign(Object.create(Object.getPrototypeOf(schedule)), schedule);
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

function makeSchedule(courseInstances, bb, numCollisions, days=5, hours=12, blocksPerHour=2) {
    return {
        days: days,
        hours: hours,
        blocksPerHour: blocksPerHour,
        courseInstances: courseInstances,
        bb: bb,
        numCollisions: numCollisions
    }
}

function defaultSchedule() {
    const days = 5;
    const hours = 12;
    const blocksPerHour = 2;
    return makeSchedule([], Array(days * hours * blocksPerHour).fill(0), 0, days, hours, blocksPerHour)
}

function scheduleAddCourse(schedule, course) {
    schedule.courseInstances.push(course);
    if (scheduleUpdateBB(schedule, course)) {
        // updateBB returns whether there was a collision or not
        ++schedule.numCollisions;
    }
    return schedule.numCollisions;
}

function scheduleUpdateBB(schedule, course) {
    let collided = 0;
    for (let i = 0; i < schedule.days * schedule.hours * schedule.blocksPerHour; ++i) {
        schedule.bb[i] = schedule.bb[i] + course.bb[i];
        collided = collided || (schedule.bb[i] && course.bb[i])
        console.log(`COLLIDED: ${collided}`)
    }
    return collided;
}

export function scheduleMakeBB(schedule) {
    for (const course of schedule.courseInstances) {
        for (let i = 0; i < schedule.days * schedule.hours * schedule.blocksPerHour; ++i) {
            schedule.bb[i] = schedule.bb[i] + course.bb[i]
        }
    }
}
