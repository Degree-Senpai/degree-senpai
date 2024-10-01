<template>
  <div>     
      <div class="top-row">
        <button class="schedule-toggle-button" @click="decrementSchedule()">
        Previous
        </button>
        <button class="schedule-toggle-button" @click="incrementSchedule()">
          Next
        </button>
        Displaying schedule {{ selectedSchedule + 1 }} / {{ generatedSchedules && generatedSchedules[activeSemester] ? generatedSchedules[activeSemester].length : null }}
        ( Collisions: {{ generatedSchedules && generatedSchedules[activeSemester] && generatedSchedules[activeSemester][selectedSchedule] ? generatedSchedules[activeSemester][selectedSchedule].collisions : null }} )
      </div>
      <div style="width: 100%; top: 32px; bottom: 0px; position:absolute">
        <div :style="getCalendarBackgroundStyle(hours, days)"></div>
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
  
  <script setup>
    /* global Module */
    import { ref } from 'vue';
    import { storeToRefs } from 'pinia';
    import { CalendarBlockElement, CourseInstance } from '@/scheduler/calendar';
    import { formatSelectedCoursesAsList } from '../scheduler/schedule.js';
    import {modifyHSLA} from '@/utilities/colorutils.js';

    import { Schedule } from '../scheduler/schedule.js'
    import { userScheduleData } from '@/stores/userStores.js';
    import { userScheduleComputed } from '@/stores/userStores.js';
    import { classData } from '@/stores/userStores.js';

    //////////// import {colorStore} from '@/utilities/store.js';
    //////////// import {schedulerStore} from '@/utilities/store.js';
    // eslint-disable-next-line no-unused-vars
    //////////// const colorService = colorStore();
    // eslint-disable-next-line no-unused-vars
    //////////// const schedulerData = schedulerStore();

    const hours = 12;
    const days = 5;

    // NOTE: SAMPLE DATA INPUTTED

    const SDStore = userScheduleData();
    const SCStore = userScheduleComputed();
    const CDStore = classData();

    const {allCourses} = storeToRefs(CDStore);
    const {selectedCourses, activeSemester} = storeToRefs(SDStore);
    const {generatedSchedules} = storeToRefs(SCStore);
    
    var blocks = ref([]); // list of CalenderBlockElement objects
    const selectedSchedule = ref(0);

    async function init() {
      //testing
      //////////// const colors = colorService.colorService;
      //////////// console.log('OUTPUTTING COLOR STORE');
      //////////// console.log(JSON.stringify(colors));

      if (typeof Module == 'undefined') {
        const script = document.createElement('script');
        script.src = './scheduler.js';
        script.onload = () => {
          console.log("emscripten scheduler module loaded");
        }
        document.body.appendChild(script);
      }
      // eslint-disable-next-line no-unused-vars
      const delay = millis => new Promise((resolve, reject) => {
        // eslint-disable-next-line no-unused-vars
        setTimeout(t => resolve(), millis)
      });
      await delay(300);
      testData();
    }

    function displaySchedule(schedule_index) {
      const schedule = generatedSchedules.value[activeSemester.value][schedule_index].renderStructure;
      blocks.value = [];
      // something's wrong with what I'm feeding into generated schedules
      for (let day = 0; day < schedule.length; day++) {
        for (let row = 0; row < schedule[day].length; row++) {
          const columns = schedule[day][row].length;
          for (let column = 0; column < columns; column++) {
            for (const crn of schedule[day][row][column]) {
              const course = allCourses.value[crn];
              for (let timeblock of course.timeblocks) {
                if (timeblock.day != day) {
                  continue;
                }
                const color = `hsla(${(crn / 3.795) % 360}, 14%, 69%, 0.7)`;
                blocks.value.push(new CalendarBlockElement(crn, timeblock.day, timeblock.begin, timeblock.length, column, columns, color, 0.02, 0, 0.1));
              }
            }
          }
        }
      }
    }

    function incrementSchedule() {
      selectedSchedule.value = (selectedSchedule.value + 1) % generatedSchedules.value[activeSemester.value].length;
      displaySchedule(selectedSchedule.value);
    }

    function decrementSchedule() {
      selectedSchedule.value = selectedSchedule.value - 1;
      if (selectedSchedule.value < 0) {
        selectedSchedule.value = generatedSchedules.value[activeSemester.value].length - 1;
      }
      displaySchedule(selectedSchedule.value);
    }


    function getCalendarBlockStyle(calendarBlockElement) {
      let borderColor = modifyHSLA(calendarBlockElement.color, -25, 10, -30, 0.2);
      let backgroundColor = modifyHSLA(calendarBlockElement.color, 34, 5, 25, 0.09);
      return {
        backgroundColor: backgroundColor,
        border: `2px solid ${borderColor}`,
        left: `${calendarBlockElement.x}%`,
        top: `${calendarBlockElement.y}%`,
        width: `${calendarBlockElement.width}%`,
        height: `${calendarBlockElement.height}%`,
        position: 'absolute'
      };
    }

    function getCalendarBlockTitleStyle(calendarBlockElement) {
      let backgroundColor = modifyHSLA(calendarBlockElement.color, 0, 5, -5, 0);
      return {
        backgroundColor: backgroundColor,
        color: '#000000',
      };
    }

    function getCalendarBackgroundStyle(hours, days) {
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
    }

    function generateSchedules(lowestNBins=1, maxBins=5, reimport=false) {
      // extracts data from the schedules binned by collisions from WASM module and puts it in a neat list in generatedSchedules
      // lowestNBins represents the leftmost N populated bins to put into generatedSchedules (empty bins don't count)
      if (!selectedCourses.value || !selectedCourses.value[activeSemester]) {
        return;
      }

      let data = formatSelectedCoursesAsList(allCourses.value, selectedCourses.value[activeSemester]);
      data = JSON.stringify(data);

      try {
        var schedules = Module.populate(data, maxBins, reimport);
        schedules = JSON.parse(schedules);
      } catch (e) {
        console.error(`Encountered error running scheduler WASM code: ${e}`);
      }

      generatedSchedules.value[activeSemester.value] = [];

      for (let bin = 0; bin < schedules.length; bin++) {
        const scheduleBin = schedules[bin];

        if (scheduleBin.length == 0) {
          continue;
        }

        for (const scheduleInstance of scheduleBin) {
          let schedule = new Schedule(allCourses.value, scheduleInstance);
          schedule.collisions = bin;
          generatedSchedules.value[activeSemester.value].push(schedule);
        }

        lowestNBins--;
        if (lowestNBins < 1) {
          break;
        }
      }
      displaySchedule(selectedSchedule.value);
    }

    function testData() {
      console.log('BEGIN TESTING OF SCHEDULER WEBASSEMBLY');

      selectedCourses.value[activeSemester] = [20001, 10001, 10002, 41001, 41002, 41003, 41004, 42000, 42001, 42002, 42003, 42004, 50000, 50001, 50002, 50003, 50004, 50005];

      /*
      let data1 = [[{name: "data structures", crn: "20001", timeBlocks: "2280, 2390, 6600, 6710"}],
      [{name: "computer science I", crn: "10001", timeBlocks: "840, 950, 5160, 5270"},
      {name: "computer science I", crn: "10002", timeBlocks: "2280, 2390, 6600, 6710"}],
      [{name: "3d animation", crn: "41001", timeBlocks: "840, 950, 5160, 5270"},
      {name: "3d animation", crn: "41002", timeBlocks: "960, 1070, 5280, 5390"},
      {name: "3d animation", crn: "41003", timeBlocks: "2280, 2390, 6600, 6710"},
      {name: "3d animation", crn: "41004", timeBlocks: "2340, 2450, 6660, 6770"}],
      [{name: "graphics storytelling", crn: "42000", timeBlocks: "480, 590, 4800, 4910"},
      {name: "graphics storytelling", crn: "42001", timeBlocks: "840, 950, 5160, 5270"},
      {name: "graphics storytelling", crn: "42002", timeBlocks: "960, 1070, 5280, 5390"},
      {name: "graphics storytelling", crn: "42003", timeBlocks: "2280, 2390, 6600, 6710"},
      {name: "graphics storytelling", crn: "42004", timeBlocks: "2340, 2450, 6660, 6770"}],
      [{name: "introduction to ecse", crn: "50000", timeBlocks: "480, 590, 4800, 4910"},
      {name: "introduction to ecse", crn: "50001", timeBlocks: "840, 950, 5160, 5270"},
      {name: "introduction to ecse", crn: "50002", timeBlocks: "960, 1070, 5280, 5390"},
      {name: "introduction to ecse", crn: "50003", timeBlocks: "2280, 2390, 6600, 6710"},
      {name: "introduction to ecse", crn: "50004", timeBlocks: "2340, 2450, 6660, 6770"},
      {name: "introduction to ecse", crn: "50005", timeBlocks: "2220, 2330, 6540, 6650"}]];

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

      let data2 = [[{name: "data structures", crn: "20001", timeBlocks: "2280, 2390, 6600, 6710"}],
      [{name: "computer science I", crn: "10001", timeBlocks: "2290, 2390, 6610, 6710"}]];
      */

      allCourses.value['20001'] = new CourseInstance('20001', 'CSCI 1200 data structures', 'akeyl', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      allCourses.value['10001'] = new CourseInstance('10001', 'CSCI 1100 computer science I', 'akeyl', 'DCC 308', null, [840, 950, 5160, 5270]);
      allCourses.value['10002'] = new CourseInstance('10002', 'CSCI 1100 computer science I', 'akeyl', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      allCourses.value['41001'] = new CourseInstance('41001', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [840, 950, 5160, 5270]);
      allCourses.value['41002'] = new CourseInstance('41002', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [960, 1070, 5280, 5390]);
      allCourses.value['41003'] = new CourseInstance('41003', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      allCourses.value['41004'] = new CourseInstance('41004', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [2340, 2450, 6660, 6770]);
      allCourses.value['42000'] = new CourseInstance('42000', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [480, 590, 4800, 4910]);
      allCourses.value['42001'] = new CourseInstance('42001', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [840, 950, 5160, 5270]);
      allCourses.value['42002'] = new CourseInstance('42002', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [960, 1070, 5280, 5390]);
      allCourses.value['42003'] = new CourseInstance('42003', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      allCourses.value['42004'] = new CourseInstance('42004', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [2340, 2450, 6660, 6770]);
      allCourses.value['50000'] = new CourseInstance('50000', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [480, 590, 4800, 4910]);
      allCourses.value['50001'] = new CourseInstance('50001', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [840, 950, 5160, 5270]);
      allCourses.value['50002'] = new CourseInstance('50002', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [960, 1070, 5280, 5390]);
      allCourses.value['50003'] = new CourseInstance('50003', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      allCourses.value['50004'] = new CourseInstance('50004', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [2340, 2450, 6660, 6770]);
      allCourses.value['50005'] = new CourseInstance('50005', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [2220, 2330, 6540, 6650]);


      generateSchedules();


      /*try {
        console.log('test large 2');
        let result = Module.populate(data1, 10, false);
        console.log(result);
      } catch (e) {
        console.error(e);
      }

      try {
        console.log('test large 3');
        let result = Module.populate(data1, 3, false);
        console.log(result);
      } catch (e) {
        console.error(e);
      }

      try {
        console.log('test persistence 1: if persistent, we should see CS1 occurring Monday');
        let result = Module.populate(data2, 10, false);
        console.log(result);
      } catch (e) {
        console.error(e);
      }

      try {
        console.log('test persistence 2: we updated original: CS1 occurs Tuesday');
        let result = Module.populate(data2, 10, true);
        console.log(result);
      } catch (e) {
        console.error(e);
      }*/
    }

    init();
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
    .calendar-background {
      background-size: cover;
      background-position: center;
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
  