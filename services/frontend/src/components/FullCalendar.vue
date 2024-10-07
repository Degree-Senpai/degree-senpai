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
            <span class="block-head-h2" style="font-size: 0.9em">{{ courseInstances[block.crn].name.substring(0, 9) }}</span> <br>
            {{ courseInstances[block.crn].name.substring(10) }} <br>
            <span class="block-head-h2" style="color:black"> CRN: {{ block.crn }} </span>
          </div>
          <div class="block-details">
            {{ courseInstances[block.crn].room }} <br>
            {{ courseInstances[block.crn].professor }}

          </div>
        </div>
      </div>
    </div>
  </template>
  
  <script setup>
    /* global Module */
    import { ref } from 'vue';
    import { storeToRefs } from 'pinia';

    import { Schedule } from '../scheduler/schedule.js'
    import { CalendarBlockElement } from '@/scheduler/calendar';
    import { CourseInstance } from '@/scheduler/courses.js';
    import { formatSelectedCoursesAsList } from '../scheduler/schedule.js';
    import { modifyHSLA } from '@/utilities/colorservice.js';

    import { userData } from '@/stores/userStores.js';
    import { userComputed } from '@/stores/userStores.js';
    import { schoolData } from '@/stores/userStores.js';

    // CONSTANTS
    const hours = 12;
    const days = 5;

    // STORES
    const userDataStore = userData();
    const userComputedStore = userComputed();
    const schoolDataStore = schoolData();

    // REACTIVE VARIABLES FROM STORES
    const {courseInstances} = storeToRefs(schoolDataStore);
    const {selectedCourses, activeSemester} = storeToRefs(userDataStore);
    const {generatedSchedules} = storeToRefs(userComputedStore);
    
    // OTHER VARIABLES
    const blocks = ref([]); // list of CalenderBlockElement objects to render
    const selectedSchedule = ref(0); // position of active schedule within generatedSchedules


    /* DEMO COLOR SERVICE USAGE */
    import { colorStore } from '@/stores/globalDataStore.js';
    import { Colors, ColorPalette, Color } from './utilities/colorservice.js'
    const { colorPalettes, colorServices } = storeToRefs(colorStore);

    const palette = ColorPalette(
        Color(40, 20, 50, 1), // main
        Color(50, 10, 70, 1), // highlight
        Color(80, 15, 60, 1), // layerouter
        Color(70, 22, 52, 1), // layerinner
        Color(100, 30, 60, 1), // iconmain
        Color(100, 40, 50, 0.9) // iconsmall
    );
    const colors = Colors(palette);

    colorPalettes.value["mainPalette"] = palette;
    colorServices.value["main"] = colors;




    async function init() {
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

     /**************************************************************************
     * 
     * MAIN LOGIC
     * 
     *************************************************************************/

    function incrementSchedule() {
      selectedSchedule.value = (selectedSchedule.value + 1) % generatedSchedules.value[activeSemester.value].length;
      generateBlocks(selectedSchedule.value);
    }

    function decrementSchedule() {
      selectedSchedule.value = selectedSchedule.value - 1;
      if (selectedSchedule.value < 0) {
        selectedSchedule.value = generatedSchedules.value[activeSemester.value].length - 1;
      }
      generateBlocks(selectedSchedule.value);
    }

    function generateSchedules(lowestNBins=1, maxBins=5, reimport=false) {
      /*
      Calls WASM to generate possible schedules, calls formatSelectedCoursesAsList to generate a 4D
      array structure for proper displaying of calendar blocks, and creates Schedule objects.

      ARGS:
        - lowestNBins: the smallest collision bins to extract schedules from. Empty bins do not count.
        - maxBins: the maximum number of bins to use in the collision algorithm. Set this to the maximum
          number of collisions we expect to display.
        - reimport: reimport class data to WASM. This is unnecessary most of the time since class data
          doesn't change often, and restarting the program always refreshes it.
      */
      if (!selectedCourses.value || !selectedCourses.value[activeSemester.value]) {
        return;
      }

      // formats selected courses for WASM processing
      let data = formatSelectedCoursesAsList(courseInstances.value, selectedCourses.value[activeSemester.value]);
      data = JSON.stringify(data);

      // WASM processing
      try {
        var schedules = Module.populate(data, maxBins, reimport);
        schedules = JSON.parse(schedules);
      } catch (e) {
        console.error(`Encountered error running scheduler WASM code: ${e}`);
      }

      // extracts WASM results (schedules binned by # of collisions) and put it in generatedSchedules as list
      // lowestNBins represents the lowest N populated bins to put into generatedSchedules (empty bins don't count)
      generatedSchedules.value[activeSemester.value] = [];

      for (let bin = 0; bin < schedules.length; bin++) {
        const scheduleBin = schedules[bin];

        if (scheduleBin.length == 0) {
          continue;
        }

        for (const scheduleInstance of scheduleBin) {
          let schedule = new Schedule(courseInstances.value, scheduleInstance);
          schedule.collisions = bin;
          generatedSchedules.value[activeSemester.value].push(schedule);
        }

        lowestNBins--;
        if (lowestNBins < 1) {
          break;
        }
      }
      generateBlocks(selectedSchedule.value);
    }

    function generateBlocks(schedule_index) {
      /*
      Generates calendar blocks by reading the 4D array structure parsed earlier. Call this everytime the calendar
      needs to be rerendered.
      */
      const schedule = generatedSchedules.value[activeSemester.value][schedule_index].renderStructure;
      blocks.value = [];
      for (let day = 0; day < schedule.length; day++) {
        for (let row = 0; row < schedule[day].length; row++) {
          const columns = schedule[day][row].length;
          for (let column = 0; column < columns; column++) {
            for (const crn of schedule[day][row][column]) {
              const course = courseInstances.value[crn];
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


    /**************************************************************************
     * 
     * CSS DISPLAY MANAGEMENT FOR CALENDAR BLOCKS
     * 
     *************************************************************************/

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

    /**************************************************************************
     * 
     * TESTING
     * 
     *************************************************************************/

    function testData() {
      console.log('BEGIN TESTING OF SCHEDULER WEBASSEMBLY');

      selectedCourses.value[activeSemester.value] = [20001, 10001, 10002, 41001, 41002, 41003, 41004, 42000, 42001, 42002, 42003, 42004, 50000, 50001, 50002, 50003, 50004, 50005];
      // selectedCourses.value[activeSemester] = [51000, 51001, 51002, 51003, 51004, 22000, 22001, 22002, 22003, 22004, 23001, 23002];
      // selectedCourses.value[activeSemester] = [20001, 10001];

      courseInstances.value['20001'] = new CourseInstance('20001', 'CSCI 1200 data structures', 'akeyl', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      courseInstances.value['10001'] = new CourseInstance('10001', 'CSCI 1100 computer science I', 'akeyl', 'DCC 308', null, [840, 950, 5160, 5270]);
      courseInstances.value['10002'] = new CourseInstance('10002', 'CSCI 1100 computer science I', 'akeyl', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      courseInstances.value['41001'] = new CourseInstance('41001', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [840, 950, 5160, 5270]);
      courseInstances.value['41002'] = new CourseInstance('41002', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [960, 1070, 5280, 5390]);
      courseInstances.value['41003'] = new CourseInstance('41003', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      courseInstances.value['41004'] = new CourseInstance('41004', 'ARTS 4070 3d animation', 'alan', 'DCC 308', null, [2340, 2450, 6660, 6770]);
      courseInstances.value['42000'] = new CourseInstance('42000', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [480, 590, 4800, 4910]);
      courseInstances.value['42001'] = new CourseInstance('42001', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [840, 950, 5160, 5270]);
      courseInstances.value['42002'] = new CourseInstance('42002', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [960, 1070, 5280, 5390]);
      courseInstances.value['42003'] = new CourseInstance('42003', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      courseInstances.value['42004'] = new CourseInstance('42004', 'ARTS 2070 graphics storytelling', 'shashank', 'DCC 308', null, [2340, 2450, 6660, 6770]);
      courseInstances.value['50000'] = new CourseInstance('50000', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [480, 590, 4800, 4910]);
      courseInstances.value['50001'] = new CourseInstance('50001', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [840, 950, 5160, 5270]);
      courseInstances.value['50002'] = new CourseInstance('50002', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [960, 1070, 5280, 5390]);
      courseInstances.value['50003'] = new CourseInstance('50003', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [2280, 2390, 6600, 6710]);
      courseInstances.value['50004'] = new CourseInstance('50004', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [2340, 2450, 6660, 6770]);
      courseInstances.value['50005'] = new CourseInstance('50005', 'ECSE 1010 introduction to ecse', 'shashank', 'DCC 308', null, [2220, 2330, 6540, 6650]);

      generateSchedules();
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
  