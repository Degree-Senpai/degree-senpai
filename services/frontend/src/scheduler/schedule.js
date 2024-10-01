import { newArray } from '../utilities/arrayHelp';

export class Schedule {
    constructor(allCourses, selectedCourses) {
        this.allCourses = allCourses;
        this.selectedCourses = selectedCourses;
        this.collisions = 0;
        this.renderStructure = [];
        this.formatForCalendar(this.selectedCourses);
    }

    formatForCalendar(schedule) {
        let parsed = newArray([5,0,0,0]); // shape: (day of week, row, column, course)
        // a new row is created if a course does not collide with any previous row,
        // otherwise it will be grouped with all collisions

        //console.log(`parsing schedule ${JSON.stringify(schedule)}`);
        for (const crn of schedule) {
            const course = this.allCourses[crn];
            for (const timeblock of course.timeblocks) {
                let overlaps = false;
                for (let row = 0; row < parsed[timeblock.day].length; row++) {
                    for (let column = 0; column < parsed[timeblock.day][row].length; column++) {
                        if (this.overlapsAny(parsed[timeblock.day][row][column], crn)) {
                            overlaps = true;
                            break;
                        }
                    }
                    if (overlaps) {
                        let added = false;
                        for (let column = 0; column < parsed[timeblock.day][row].length; column++) {
                            if (!this.overlapsAny(parsed[timeblock.day][row][column], crn)) {
                                parsed[timeblock.day][row][column].push(crn);
                                added = true;
                                break;
                            }
                        }
                        if (!added) {
                            parsed[timeblock.day][row].push([crn]);
                        }
                        break;
                    }
                }
                if (!overlaps) {
                    parsed[timeblock.day].push([[crn]]);
                }
            }
        }
        this.renderStructure = parsed;
    }

    overlaps(courseCRN1, courseCRN2) {
        let i = 0;
        let j = 0;
        while (i != this.allCourses[courseCRN1].lineartimeblocks.length && j != this.allCourses[courseCRN2].lineartimeblocks.length) {
            if (this.allCourses[courseCRN1].lineartimeblocks[i] < this.allCourses[courseCRN2].lineartimeblocks[j]) {
                ++i;
                if (j & 1) {
                    return true;
                }
            } else if (this.allCourses[courseCRN1].lineartimeblocks[i] == this.allCourses[courseCRN2].lineartimeblocks[j]) {
                ++i;
                ++j;
                if ((i & 1) == (j & 1)) {
                    return true;
                }
            } else {
                ++j;
                if (i & 1) {
                    return true;
                }
            }
        }
        return false;
    }

    overlapsAny(coursesCRN, courseCRN) {
        if (coursesCRN.length == 0) {
            return false;
        }
        for (const courseEntryCRN of coursesCRN) {
            if (this.overlaps(courseEntryCRN, courseCRN)) {
                return true;
            }
        }
        return false;
    }
}

export function formatSelectedCoursesAsDictionary(allCourses, selectedCourses) {
    let groupedCourses = {};
    for (const CRN of selectedCourses) {
        const course = allCourses[CRN];
        if (course == null) {
            continue;
        }
        if (groupedCourses[course.name] != null) {
            groupedCourses[course.name].push(courseInstanceToJson(course));
        }
        else {
            groupedCourses[course.name] = [courseInstanceToJson(course)];
        }
    }
    return groupedCourses;
}

export function formatSelectedCoursesAsList(allCourses, selectedCourses) {
    let formatted = [];
    const groupedDict = formatSelectedCoursesAsDictionary(allCourses, selectedCourses);
    for(let key in groupedDict) {
        formatted.push(groupedDict[key]);
    }
    return formatted;
}

function courseInstanceToJson(courseInstance) {
    return {name: courseInstance.name, crn: courseInstance.crn.toString(), timeBlocks: courseInstance.lineartimeblocks.join(', ')};
}
