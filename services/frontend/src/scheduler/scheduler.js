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
        collided = 0;
        for (let i = 0; i < this.days * this.hours * this.blocksPerHour; ++i) {
            this.bb[i] = this.bb[i] + course.bb[i];
            collided = collided | (this.bb[i] & course.bb[i])
        }
        return collided;
    }
}

export function scheduleFill(selectedGroupedCourseInstances, maxDepth=3) {
    let schedules = [defaultSchedule()]; // schedule objects
    for (const courseWithManyInstances in selectedGroupedCourseInstances) {
        let newSchedules = [];
        for (let schedule in schedules) {
            for (const courseInstance in courseWithManyInstances) {
                let clone = Object.assign(Object.create(Object.getPrototypeOf(schedule)), schedule);
                collisions = scheduleAddCourse(clone, courseInstance);
                if (collisions > maxDepth) {
                    continue;
                }
                newSchedules.push(clone);
            }
        }
        schedules = newSchedules;
    }
}

function makeSchedule(courseInstances, bb, numCollisions) {
    return {
        days: 5,
        hours: 12,
        blocksPerHour: 2,
        courseInstances: courseInstances,
        bb: bb,
        numCollisions: numCollisions
    }
}

function defaultSchedule() {
    return makeSchedule([], Array(this.days * this.hours * this.blocksPerHour).fill(0), 0)
}

function scheduleAddCourse(schedule, course) {
    schedule.courseInstances.push(course);
    if (scheduleUpdateBB(schedule, course)) {
        // updateBB returns whether there was a collision or not
        ++this.numCollisions;
    }
    return this.numCollisions;
}

function scheduleUpdateBB(schedule, course) {
    collided = 0;
    for (let i = 0; i < schedule.days * schedule.hours * schedule.blocksPerHour; ++i) {
        schedule.bb[i] = schedule.bb[i] + course.bb[i];
        collided = collided | (schedule.bb[i] & course.bb[i])
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
