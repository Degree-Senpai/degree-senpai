export class Course {
    constructor(subject, id, name, description, credits, instances) {
        this.subject = subject;
        this.id = id;
        this.name = name; // ex: Data Structures
        this.description = description;
        this.credits = credits;
        this.instances = instances; // {semester: list of CourseInstance objects}
        if (instances == null) {
            instances = {};
        }
    }
}

export class CourseInstance {
    // this class stores only information relevant to the current offering (instance) of the course, additional
    // information is stored as a Course class containing information such as description, credit count and groups
    constructor(crn, name, professor, room, timeblocks, lineartimeblocks) {
        this.crn = crn; // unique identifier for the instance of this course, ex: 40295
        this.name = name; // ex: CSCI 1200 Data Structures
        this.professor = professor; // string
        this.room = room; // string ex: DCC 308
        this.timeblocks = timeblocks; // list of timeblocks
        this.lineartimeblocks = lineartimeblocks;
        if (timeblocks == null) {
            this.timeblocks = [];
            for (let i = 0; i < lineartimeblocks.length; i = i + 2) {
                this.timeblocks.push(new TimeBlock(Math.floor(lineartimeblocks[i] / 1440), lineartimeblocks[i] % 1440, lineartimeblocks[i + 1] % 1440));
                //console.log(`generated timeblock ${JSON.stringify(this.timeblocks)}`)
            }
        }
        if (lineartimeblocks == null) {
            this.lineartimeblocks = [];
            for (const timeblock of timeblocks) {
                this.lineartimeblocks.push(timeblock.begin + (timeblock.day * 1440));
                this.lineartimeblocks.push(timeblock.end + (timeblock.day * 1440));
            }
        }
    }
}
