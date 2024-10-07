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

export class TimeBlock {
    constructor(day, begin, end) {
        this.day = day;
        this.begin = begin;
        this.end = end;
        this.length = this.end - this.begin;
    }
}
