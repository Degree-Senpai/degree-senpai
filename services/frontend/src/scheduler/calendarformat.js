export class CalendarBlockElement {
    // block rendering for calendar

    constructor(crn, day, time, length, column, columns, color, blockMargin=0, lMargin=0, rMargin=0) {
        // constants
        this.days = 5;
        this.beginHour = 8;
        this.endHour = 20;
        this.margin = blockMargin;
        this.crn = crn;
        let xoffset = 0;

        [this.x, this.y] = this.getLocation(day, time);
        this.height = this.getLength(length);
        [this.width, xoffset] = this.getColumn(column, columns, lMargin, rMargin);
        this.x += xoffset + this.width * (blockMargin / 2);
        this.y += this.height * (blockMargin / 2);

        this.height *= (1 - blockMargin);
        this.width *= (1 - blockMargin);

        this.color = color;
        this.clashing = (columns > 1);
    }
    getLocation(day, time) {
        let x = ((day) / this.days) * 100;
        let y = (time - (this.beginHour * 60));
        y = y * (100 / ((this.endHour - this.beginHour) * 60));
        //console.log("day: " + day + " x: " + x + " y: " + y);
        return [x, y];
    }
    getColumn(column, columns, lMargin, rMargin) {
        let segmentWidth = (100 * (1 - lMargin - rMargin) / this.days) / columns;
        return [segmentWidth, column * segmentWidth + lMargin * (100 / this.days)]
    }
    getLength(length) {
        return length * (100 / ((this.endHour - this.beginHour) * 60));
    }
}

export class CourseInstance {
    // this class stores only information relevant to the current offering (instance) of the course, additional
    // information is stored as a Course class containing information such as description, credit count and groups
    constructor(crn, name, professor, room, timeblocks) {
        this.crn = crn; // unique identifier for the instance of this course, ex: 40295
        this.name = name; // ex: CSCI 1200 Data Structures
        this.professor = professor; // string
        this.room = room; // string ex: DCC 308
        this.timeblocks = timeblocks; // list of numbers
    }
}


export function formatGeneratedSchedules(generatedSchedules) {
    // converts generated schedules (3D array of CRN) into 4D formatted for rendering
    console.log(`generatedSchedules: ${JSON.stringify(generatedSchedules)}`)
    let formattedSchedules = [];
    for (let schedule of generatedSchedules) {
        let timeBlockByDay = Array(this.days);
        for (let i = 0; i < timeBlockByDay.length; ++i) {
            timeBlockByDay[i] = Array(0);
        }
        for (let course of schedule.courseInstances) {
            for (let timeblock of course.timeblocks) {
                timeBlockByDay[timeblock.day].push(timeblock);
                console.log(`adding timeblock ${JSON.stringify(timeblock)} of course ${course.name}, new array ${JSON.stringify(timeBlockByDay)}`)
            }
        }
        console.log(`$TIMEBLOCKBYDAY: ${JSON.stringify(timeBlockByDay)}`);
        // add code for separating collisions into different columns

        // placeholder that just overlaps them
        let columns = Array(this.days);
        for (let i = 0; i < this.days; ++i) {
            columns[i] = Array(1);
            columns[i][0] = Array(0);
            for (let timeblock of timeBlockByDay[i]) {
                columns[i][0].push(timeblock.crn);
            }
        }
        formattedSchedules.push(columns);
    }
    console.log(`formattedSchedules: ${formattedSchedules}`)
    return formattedSchedules
}
