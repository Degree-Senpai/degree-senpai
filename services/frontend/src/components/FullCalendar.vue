<template>
    <div>
      
    </div>
  </template>
  
  <script>
  class CalendarBlockStyle {
    constructor(day, time, length, xpos, xtotal, color) {
      [this.x, this.y] = this.getLocation(day, time);
      this.height = this.getLength(length);

      let xoffset = 0;
      [this.width, xoffset] = this.getXPos(xpos, xtotal);
      this.x += xoffset;

      this.color = color;

      // constants
      this.beginHour = 8;
      this.endHour = 22;
    }
    getLocation(day, time) {
      let x = ((day - 1) / this.days) * 100;
      let y = (time - (this.beginHour * 60)); // begin at 8am
      y = y * (100 / ((this.endHour - this.beginHour) * 60)); // end at 10pm
      return (x, y);
    }
    getXPos(xpos, xtotal) {
      let segmentWidth = 20 / xtotal;
      return segmentWidth, xpos * segmentWidth
    }
    getLength(length) {
      return length * (100 / ((this.endHour - this.beginHour) * 60));
    }
  }
  class TimeBlock {
    constructor(day, begin, end) {
      this.day = day;
      this.begin = begin;
      this.end = end;
    }

    getMinBegin() {
      return ((this.day - 1) * 1440) + this.begin
    }

    getMinEnd() {
      return ((this.day - 1) * 1440) + this.end
    }

    overlaps(other) {
      return (this.getMinBegin() > other.getMinBegin() && this.getMinEnd() < other.getMinEnd()) || (this.getMinEnd() > other.getMinBegin() && this.getMinEnd() < other.getMinEnd())
    }
  }
  class CourseInstance {
    // this class stores only information relevant to the current offering (instance) of the course, additional
    // information is stored as a Course class containing information such as description, credit count and groups
    constructor(crn, name, professor, room, timeblocks) {
      this.crn = crn; // unique identifier for the instance of this course, ex: 40295
      this.name = name; // ex: CSCI 1200 Data Structures
      this.professor = professor; // string
      this.room = room; // string ex: DCC 308
      this.timeblocks = timeblocks;
    }
  }
  class CalendarBlock {
    constructor(instance, style) {
      this.instance = instance;
      this.style = style;
    }
  }
  
  export default {
      data() {
          return {
            beginHour: 8,
            endHour: 22,
            days: 5,

            // NOTE: SAMPLE DATA INPUTTED

            allCourses: {},
            selectedCourses: [],
            blocks: [], // 2D array: day of week -> calendar blocks
          };
      },
      methods: {
        // location specified as percentage of full width/height
        addCourse(crn) {
          this.selectedCourses.push(crn);
        },
        overlaps(crn1, crn2) {
          if (crn1 == crn2) {
            return false
          }
          course1 = this.allCourses[crn1];
          course2 = this.allCourses[crn2];
          for (const timeblock1 of course1) {
            for (const timeblock2 of course2) {
              if (timeblock1.overlaps(timeblock2)) {
                return true
              }
            }
          }
          return false
        },

        constructBlocks() {
          let overlaps = {}; // records number of overlaps this crn has
          for (const crn1 of this.selectedCourses) {
            overlaps[crn1] = 0;
            for (const crn2 of this.selectedCourses) {
              if (this.overlaps(crn1, crn2)) {
                overlaps[crn1] = overlaps[crn1] + 1;
              }
            }
          }
          for (const crn of this.selectedCourses) {
            let courseInstance = allCourses[crn];
            style = this.getStyle(courseInstance, overlaps[crn]);
            this.blocks.push(CalendarBlock(courseInstance, style));
          }
        },
      }
  };
  </script>
  
  <style scoped>
  /* Add your CSS styling here for the calendar cells */
  </style>