<template>
    <div>
      <div class="top-row">
        <button class="schedule-toggle-button" @click="decrementSchedule()">
        Previous
        </button>
        <button class="schedule-toggle-button" @click="incrementSchedule()">
          Next
        </button>
        Displaying schedule {{ this.selectedSchedule + 1 }} / {{ this.generatedSchedules.length }}
      </div>
      <div class="block" v-for="(block, block_index) in blocks" :key="block_index" :style="writeStyle(block)">
        <div class="block-head" :style="writeInnerStyle(block)"> {{ allCourses[block.crn].name }}</div>
        <div class="block-details">
          {{ allCourses[block.crn].room }} <br>
          {{ allCourses[block.crn].professor }}

        </div>
      </div>
    </div>
  </template>
  
  <script>
  class CalendarBlockElement {
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
      let x = ((day - 1) / this.days) * 100;
      let y = (time - (this.beginHour * 60)); // begin at 8am
      y = y * (100 / ((this.endHour - this.beginHour) * 60)); // end at 10pm
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

  class CourseInstance {
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

  class TimeBlock {
    constructor(day, begin, end) {
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

    overlaps(other) {
      return (this.getMinBegin() > other.getMinBegin() && this.getMinEnd() < other.getMinEnd()) || (this.getMinEnd() > other.getMinBegin() && this.getMinEnd() < other.getMinEnd())
    }
  }
  
  export default {
      data() {
          return {
            beginHour: 8,
            endHour: 20,
            days: 5,

            // NOTE: SAMPLE DATA INPUTTED

            allCourses: {},
            selectedCourses: [],
            generatedSchedules: [], // shape: (schedule, days of week, columns of day, CRNs)
            blocks: [], // 2D array: day of week -> calendar blocks
            selectedSchedule: 0,
          };
      },
      async created() {
        this.testData();
      },
      methods: {
        // location specified as percentage of full width/height
        addCourse(crn) {
          this.selectedCourses.push(crn);
        },

        displaySchedule(schedule_index) {
          let schedule = this.generatedSchedules[schedule_index];
          this.blocks = [];
          for (let day = 0; day < schedule.length; day++) {
            let columns = schedule[day].length;
            for (let column = 0; column < columns; column++) {
              for (let crn of schedule[day][column]) {
                let course = this.allCourses[crn];
                for (let timeblock of course.timeblocks) {
                  if (timeblock.day != day + 1) {
                    continue;
                  }
                  let color = `hsla(${(course.crn / 3.8) % 360}, 10%, 55%, 0.8)`;
                  this.blocks.push(new CalendarBlockElement(course.crn, timeblock.day, timeblock.begin, timeblock.length, column, columns, color, 0.02, 0, 0.1));
                }
              }
            }
          }
        },

        incrementSchedule() {
          this.selectedSchedule = (this.selectedSchedule + 1) % this.generatedSchedules.length;
          this.displaySchedule(this.selectedSchedule);
        },

        decrementSchedule() {
          this.selectedSchedule = this.selectedSchedule - 1;
          if (this.selectedSchedule < 0) {
            this.selectedSchedule = this.generatedSchedules.length - 1;
          }
          this.displaySchedule(this.selectedSchedule);
        },

        modifyHSLA(hsla, h, s, l, a) {
          let values = hsla.split('(')[1];
          values = values.split(')')[0];
          values = values.split(',');
          h = h + Number(values[0]);
          s = s + Number(values[1].split('%')[0]);
          l = l + Number(values[2].split('%')[0]);
          a = a + Number(values[3]);
          return `hsla(${h % 360}, ${s}%, ${l}%, ${a})`;
        },

        writeStyle(calendarBlockElement) {
          let borderColor = this.modifyHSLA(calendarBlockElement.color, 0, 5, -10, 0);
          let backgroundColor = calendarBlockElement.color;
          return {
            backgroundColor: backgroundColor,
            border: `1px solid ${borderColor}`,
            left: `${calendarBlockElement.x}%`,
            top: `${calendarBlockElement.y}%`,
            width: `${calendarBlockElement.width}%`,
            height: `${calendarBlockElement.height}%`
          };
        },

        writeInnerStyle(calendarBlockElement) {
          let backgroundColor = this.modifyHSLA(calendarBlockElement.color, 0, -5, 10, 0);
          let textColor = this.modifyHSLA(calendarBlockElement.color, 0, -10, -40, 0.2);
          return {
            backgroundColor: backgroundColor,
            color: textColor
          };
        },

        testData() {
          let course1 = new CourseInstance(10000, 'Computer Science I', 'Jon', 'DCC 308', [new TimeBlock(1, 720, 830), new TimeBlock(4, 720, 830)]);
          let course2 = new CourseInstance(20000, 'Data Structures', 'Dorian', 'DCC 308', [new TimeBlock(2, 840, 950), new TimeBlock(5, 840, 950)]);
          let course3 = new CourseInstance(45000, '3D Animation', 'Alan', 'Sage 3330', [new TimeBlock(1, 720, 830), new TimeBlock(4, 720, 830)]);
          let course4 = new CourseInstance(47000, 'Visual Effects', 'Alan', 'Sage 3150', [new TimeBlock(1, 720, 890), new TimeBlock(4, 720, 890)]);
          this.allCourses[course1.crn] = course1;
          this.allCourses[course2.crn] = course2;
          this.allCourses[course3.crn] = course3;
          this.allCourses[course4.crn] = course4;
          
          this.generatedSchedules = [[[[10000], [45000]], [[20000]], [[]], [[10000], [45000]], [[20000]]], [[[10000], [47000]], [[20000]], [[]], [[10000], [47000]], [[20000]]], [[[10000]], [[20000]], [[]], [[10000]], [[]]]];  // shape: (schedule, days of week, columns of day, CRNs)
          this.displaySchedule(this.selectedSchedule);
        },
      }
  };
  </script>
  
  <style scoped>
    .schedule-toggle-button {
      border-radius: 4px;
      border: none;
      background-color: #5a6368;
      color:aliceblue;
      margin: 4px;
    }
    .top-row {
      border-radius: 4px;
      background-color: rgba(46, 47, 49, 0.829);
      justify-items: center;
    }
    .block {
      color: #000001;
      border-radius: 2px;
      position: absolute;
    }
    .block-head {
      font-size: 11px;
      font-weight: 700;
      position: relative;
      border-radius: 2px;
      padding: 2px;
      width: 100%;
      line-height: 1.3;
    }
    .block-details {
      font-size: 10px;
      font-weight: 600;
      padding: 2px;
      position: relative;
      line-height: 1.2;
    }
  </style>