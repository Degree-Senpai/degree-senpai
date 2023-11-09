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
  /* global Module */

  import {CalendarBlockElement} from '@/scheduler/coursedata.js';
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
      mounted() {
        if (typeof Module == 'undefined') {
          const script = document.createElement('script');
          script.src = './scheduler.js';
          script.onload = () => {
            console.log("emscripten module loaded");
          }
          document.body.appendChild(script);
        } else {
          this.testData();
        }
      },
      async created() {
        this.generatedSchedules = [[]];
        //this.testData();
      },
      methods: {
        // location specified as percentage of full width/height
        addCourse(crn) {
          this.selectedCourses.push(crn);
        },

        displaySchedule(schedule_index) {
          let schedule = this.generatedSchedules[schedule_index];
          this.blocks = [];
          this.testData();
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
          this.selectedSchedule = (this.selectedSchedule + 1) % this.generatedSchedules.length;
          this.displaySchedule(this.selectedSchedule);
        },

        decrementSchedule() {
          this.testData();
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

        formatGeneratedSchedules(generatedSchedules) {
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
          {name: "graphics storytelling", crn: "42004", timeBlocks: "2340, 2390, 6660, 6770"}]];
          data = JSON.stringify(data);
          try {
            console.log('HIIII');
            let result = Module.populate(data, 5);
            console.log(result);
          } catch (e) {
            console.error(e);
          }
          /*
          if (Module && Module.cwrap) {
            //let schedules = Module._populate(data, 5);
            console.log(`attempting to call populate on module`);
            const test = Module.cwrap('test', 'number', ['number']);
            let result = test(5);
            console.log(`result: ${result}`);
            const populate = Module.cwrap('populate', 'string', ['string', 'number']);
            let schedules = populate(data, 5);
            console.log(`generated schedules by webassembly C++: ${schedules}`);
          } else {
            console.error("emscripten module is not yet loaded!");
          }*/
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