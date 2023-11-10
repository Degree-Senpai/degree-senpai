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
        <div :style="getCalendarBlockBackgroundStyle(hours, days)"></div>
        <div class="block" v-for="(block, block_index) in blocks" :key="block_index" :style="getCalendarBlockStyle(block)">
          <div class="block-head" :style="getCalendarBlockTitleStyle(block)">
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
  /* global Module */

  import { CalendarBlockElement } from '@/scheduler/calendarformat';
  //import {CalendarBlockElement, CourseInstance, formatGeneratedSchedules} from '@/scheduler/calendarformat.js';
  //import {formatSelectedCourses} from '@/scheduler/scheduleformat.js';
  import {modifyHSLA} from '@/utilities/colorutils.js';
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
            generatedSchedules: [[]], // shape: (schedule, days of week, columns of day, CRNs)
            blocks: [], // 2D array: day of week -> calendar blocks
            selectedSchedule: 0,
          };
      },
      mounted() {
        if (typeof Module == 'undefined') {
          const script = document.createElement('script');
          script.src = './scheduler.js';
          script.onload = () => {
            console.log("emscripten module loaded");
          }
          document.body.appendChild(script);
        }
      },
      methods: {
        addCourse(crn) {
          this.selectedCourses.push(crn);
        },

        displaySchedule(schedule_index) {
          let schedule = this.generatedSchedules[schedule_index];
          this.blocks = [];
          // something's wrong with what I'm feeding into generated schedules
          for (let day = 0; day < schedule.length; day++) {
            let columns = schedule[day].length;
            for (let column = 0; column < columns; column++) {
              for (let crn of schedule[day][column]) {
                let course = this.allCourses[crn];
                for (let timeblock of course.timeblocks) {
                  if (timeblock.day != day) {
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
          this.testData();
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



        getCalendarBlockStyle(calendarBlockElement) {
          let borderColor = modifyHSLA(calendarBlockElement.color, -25, 10, -30, 0.2);
          let backgroundColor = modifyHSLA(calendarBlockElement.color, 34, 5, 25, 0.09);
          return {
            backgroundColor: backgroundColor,
            border: `2px solid ${borderColor}`,
            left: `${calendarBlockElement.x}%`,
            top: `${calendarBlockElement.y}%`,
            width: `${calendarBlockElement.width}%`,
            height: `${calendarBlockElement.height}%`
          };
        },

        getCalendarBlockTitleStyle(calendarBlockElement) {
          let backgroundColor = modifyHSLA(calendarBlockElement.color, 0, 5, -5, 0);
          return {
            backgroundColor: backgroundColor,
            color: '#000000',
          };
        },

        getCalendarBlockBodyStyle(calendarBlockElement) {
          let borderColor = modifyHSLA(calendarBlockElement.color, 0, 10, -30, 0);
          let backgroundColor = modifyHSLA(calendarBlockElement.color, 10, 20, -90, 1);
          return {
            backgroundColor: backgroundColor,
            border: `1px solid ${borderColor}`,
          };
        },

        getCalendarBlockBackgroundStyle(hours, days) {
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
          console.log('BEGIN TESTING OF SCHEDULER WEBASSEMBLY');
          let data = [[{name: "data structures", crn: "20001", timeBlocks: "2280, 2390, 6600, 6710"}],
          [{name: "computer science I", crn: "10001", timeBlocks: "840, 950, 5160, 5270"},
          {name: "computer science I", crn: "10002", timeBlocks: "2280, 2390, 6600, 6710"}],
          [{name: "3d animation", crn: "41001", timeBlocks: "840, 950, 5160, 5270"},
          {name: "3d animation", crn: "41002", timeBlocks: "960, 1070, 5280, 5390"},
          {name: "3d animation", crn: "41003", timeBlocks: "2280, 2390, 6600, 6710"},
          {name: "3d animation", crn: "41004", timeBlocks: "2340, 2390, 6660, 6770"}],
          [{name: "graphics storytelling", crn: "42000", timeBlocks: "480, 590, 4800, 4910"},
          {name: "graphics storytelling", crn: "42001", timeBlocks: "840, 950, 5160, 5270"},
          {name: "graphics storytelling", crn: "42002", timeBlocks: "960, 1070, 5280, 5390"},
          {name: "graphics storytelling", crn: "42003", timeBlocks: "2280, 2390, 6600, 6710"},
          {name: "graphics storytelling", crn: "42004", timeBlocks: "2340, 2390, 6660, 6770"}],
          [{name: "introduction to ecse", crn: "50000", timeBlocks: "480, 590, 4800, 4910"},
          {name: "introduction to ecse", crn: "50001", timeBlocks: "840, 950, 5160, 5270"},
          {name: "introduction to ecse", crn: "50002", timeBlocks: "960, 1070, 5280, 5390"},
          {name: "introduction to ecse", crn: "50003", timeBlocks: "2280, 2390, 6600, 6710"},
          {name: "introduction to ecse", crn: "50004", timeBlocks: "2340, 2390, 6660, 6770"}],
          [{name: "introduction to electronics", crn: "51000", timeBlocks: "540, 590, 4800, 4910"},
          {name: "introduction to electronics", crn: "51001", timeBlocks: "900, 950, 5220, 5270"},
          {name: "introduction to electronics", crn: "51002", timeBlocks: "1020, 1070, 5340, 5390"},
          {name: "introduction to electronics", crn: "51003", timeBlocks: "2340, 2390, 6660, 6710"},
          {name: "introduction to electronics", crn: "51004", timeBlocks: "2400, 2390, 6720, 6770"}],
          [{name: "data science", crn: "22000", timeBlocks: "4800, 5030"},
          {name: "data science", crn: "22001", timeBlocks: "5220, 5390"},
          {name: "data science", crn: "22002", timeBlocks: "5340, 5510"},
          {name: "data science", crn: "22003", timeBlocks: "6660, 6830"},
          {name: "data science", crn: "22004", timeBlocks: "6720, 6890"}],
          [{name: "data mining", crn: "23001", timeBlocks: "6660, 6830"},
          {name: "data mining", crn: "23002", timeBlocks: "6720, 6890"}]];
          data = JSON.stringify(data);
          try {
            console.log('HIIII');
            let result = Module.populate(data, 5);
            console.log(result);
          } catch (e) {
            console.error(e);
          }
        }
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
  