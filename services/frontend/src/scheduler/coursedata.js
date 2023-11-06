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
        this.timeblocks = timeblocks; //TimeBlock object
    }
}

export class FastCourseInstance {
    constructor(crn, timeblocks) {
        this.crn = crn;
        this.days = 5;
        this.hours = 12;
        this.blocksPerHour = 2;
        this.bb = Array(this.days * this.hours * this.blocksPerHour).fill(0);
        for (const timeblock of timeblocks) {
            for (let i = timeblock.bbBegin; i < timeblock.bbEnd; ++i) {
                this.bb[i + (timeblock.day * this.hours * this.blocksPerHour)] = 1;
            }
        }
    }
}

export class TimeBlock {
    // single time block for course instance, a course instance may have multiple timeblocks
    constructor(crn, day, begin, end) {
        this.crn = crn;
        this.day = day;
        this.begin = begin;
        this.end = end;
        this.length = this.end - this.begin;
    }

    getMinBegin() {
        return ((this.day - 1) * 1440) + this.begin
    }
  
    getMinEnd() {
        return ((this.day - 1) * 1440) + this.end
    }

    timeToBB(time) {
        return parseInt(time / this.bbBlockSize);
    }

    hasCollision(timeBlock) {
        if (this.day != timeBlock.day) {
            return false
        }
        for (let i = this.bbBegin; i < this.bbEnd; ++i) {
            if (timeBlock.bb[i] == 1) {
                return true
            }
        }
        return false
    }
}
