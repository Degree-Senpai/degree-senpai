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
      <div style="width: 100%; top: 32px; bottom: 0px; position:absolute">
        <div :style="writeBackground(hours, days)"></div>
        <div class="block" v-for="(block, block_index) in blocks" :key="block_index" :style="writeStyle(block)">
          <div class="block-head" :style="writeTitleStyle(block)">
            <span class="block-head-h2" style="font-size: 0.9em">{{ allCourses[block.crn].name.substring(0, 9) }}</span> <br>
            {{ allCourses[block.crn].name.substring(10) }} <br>
            <span class="block-head-h2" style="color:black"> CRN: {{ block.crn }} </span>
          </div>
          <div class="block-details">
            {{ allCourses[block.crn].room }} <br>
            {{ allCourses[block.crn].professor }}

          </div>
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
            hours: 12,
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
                  let color = `hsla(${(course.crn / 3.795) % 360}, 14%, 69%, 0.7)`;
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
          let borderColor = this.modifyHSLA(calendarBlockElement.color, -25, 10, -30, 0.2);
          let backgroundColor = this.modifyHSLA(calendarBlockElement.color, 34, 5, 25, 0.09);
          return {
            backgroundColor: backgroundColor,
            border: `2px solid ${borderColor}`,
            left: `${calendarBlockElement.x}%`,
            top: `${calendarBlockElement.y}%`,
            width: `${calendarBlockElement.width}%`,
            height: `${calendarBlockElement.height}%`
          };
        },

        writeTitleStyle(calendarBlockElement) {
          let backgroundColor = this.modifyHSLA(calendarBlockElement.color, 0, 5, -5, 0);
          return {
            backgroundColor: backgroundColor,
            color: '#000000',
          };
        },

        writeBodyStyle(calendarBlockElement) {
          let borderColor = this.modifyHSLA(calendarBlockElement.color, 0, 10, -30, 0);
          let backgroundColor = this.modifyHSLA(calendarBlockElement.color, 10, 20, -90, 1);
          return {
            backgroundColor: backgroundColor,
            border: `1px solid ${borderColor}`,
          };
        },

        writeBackground(hours, days) {
          return {
            zIndex: 999,
            height: '100%',
            width: '100%',
            border: 'none',
            position: 'absolute',
            background: `
              linear-gradient(hsla(230,10%,70%,.2) 1px, transparent 1px),
              linear-gradient(hsla(260,10%,90%,.3) 1px, transparent 1px), 
              linear-gradient(-90deg, hsla(260,10%,70%,.3) 1px, transparent 1px),
              hsla(250, 5%, 30%, 0.4)`,
            backgroundSize: `
              20% ${100 / hours}%,
              20% ${200 / hours}%,
              20% ${100 / days}%`
          }
        },

        testData() {
          let course1 = new CourseInstance(10000, 'CSCI 1100 COMPUTER SCIENCE I', 'Akeyl', 'DCC 308', [new TimeBlock(1, 720, 830), new TimeBlock(4, 720, 830)]);
          let course2 = new CourseInstance(20000, 'CSCI 1200 DATA STRUCTURES', 'Shashank', 'DCC 308', [new TimeBlock(2, 840, 950), new TimeBlock(5, 840, 950)]);
          let course3 = new CourseInstance(45000, 'ARTS 4070 3D ANIMATION', 'Alan', 'Sage 3330', [new TimeBlock(1, 720, 830), new TimeBlock(4, 720, 830)]);
          let course4 = new CourseInstance(47000, 'ARTS 4080 ANIMATION PRODUCTION AND VISUAL EFFECTS', 'Alan', 'Sage 3150', [new TimeBlock(1, 720, 890), new TimeBlock(4, 720, 890)]);
          let course5 = new CourseInstance(51000, 'ECSE 2010 Circuits', 'Alan', 'Sage 3330', [new TimeBlock(1, 600, 710), new TimeBlock(4, 600, 710)]);
          let course6 = new CourseInstance(52000, 'ECSE 2100 Intro to Electronics', 'Alan', 'DCC 318', [new TimeBlock(2, 600, 710), new TimeBlock(5, 600, 710)]);
          this.allCourses[course1.crn] = course1;
          this.allCourses[course2.crn] = course2;
          this.allCourses[course3.crn] = course3;
          this.allCourses[course4.crn] = course4;
          this.allCourses[course5.crn] = course5;
          this.allCourses[course6.crn] = course6;
          this.generatedSchedules = [[[[10000, 51000], [45000]], [[20000, 52000]], [[]], [[10000, 51000], [45000]], [[20000, 52000]]], [[[10000], [47000]], [[20000]], [[]], [[10000], [47000]], [[20000]]], [[[10000]], [[20000]], [[]], [[10000]], [[]]]];  // shape: (schedule, days of week, columns of day, CRNs)
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
      background-color: rgba(46, 47, 49, 0.8);
      justify-items: center;
    }
    .block {
      color: #000001;
      z-index: 9999;
      border-radius: 4px;
      position: absolute;
      backdrop-filter: blur(1px);
      overflow-x: auto;
      overflow-y: auto;
    }
    .block-head {
      font-size: 0.71vw;
      font-weight: 900;
      position: relative;
      border-radius: 4px;
      padding: 2px;
      width: auto;
      height: auto;
      overflow:visible;
      line-height: 1.1;
    }
    .block-head-h2 {
      font-size: 0.65vw;
      font-weight: 600;
      width: auto;
      color:#303132;
    }
    .block-details {
      font-size: 0.65vw;
      font-weight: 600;
      padding: 2px;
      width: auto;
      color: #1c1c1d;
      border-radius: 20px;
      position: relative;
      line-height: 1.1;
      max-height: 100%;
    }

    .block::-webkit-scrollbar {
        width: 3px; /* Adjust width for vertical scrollbar */
        height: 4px; /* Adjust height for horizontal scrollbar */
    }

    .block::-webkit-scrollbar-track {
        background: #f1f1f100; /* Color of the track */
    }

    .block::-webkit-scrollbar-thumb {
        background: rgba(67, 74, 81, 0.5); /* Color of the scroll thumb */
        border-radius: 2px; /* Rounded corners on the scroll thumb */
    }

    .block::-webkit-scrollbar-thumb:hover {
        background: #555; /* Color of the scroll thumb when hovered */
    }


    .calendar-background {
      z-index: 999;
      height: 100%;
      width: 90%;
      border: none;
      position: absolute;
      background:
        linear-gradient(-90deg, rgba(0,0,0,.05) 1px, transparent 1px),
        linear-gradient(rgba(0,0,0,.05) 1px, transparent 1px), 
        linear-gradient(-90deg, rgba(0, 0, 0, .04) 1px, transparent 1px),
        linear-gradient(rgba(0,0,0,.04) 1px, transparent 1px),
        linear-gradient(transparent 3px, #f2f2f2 3px, #f2f2f2 78px, transparent 78px),
        linear-gradient(-90deg, #aaa 1px, transparent 1px),
        linear-gradient(-90deg, transparent 3px, #f2f2f2 3px, #f2f2f2 78px, transparent 78px),
        linear-gradient(#aaa 1px, transparent 1px),
        #f2f2f2;
      background-size:
        4px 4px,
        4px 4px,
        80px 80px,
        80px 80px,
        80px 80px,
        80px 80px,
        80px 80px,
        80px 80px;
      }
  </style>