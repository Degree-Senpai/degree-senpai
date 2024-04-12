<template>
    <!-- semester selection -->
    <div class="schedule-selection-main">

        <!-- SCHEDULE SELECTION -->
        <div class="multi-schedule-select">
            <!-- deletion prompt -->
            <div ref="scheduleDeletionPrompt" class="schedule-deletion-prompt" v-show="showScheduleDeletionPrompt">
                Confirm Deletion of Schedule '{{ defaultSchedule }}'?
                <div class="schedule-deletion-prompt-options">
                    <button class="schedule-confirm-delete" @click="deleteSchedule(defaultSchedule)">YES</button>
                    <button class="schedule-deny-delete" @click="cancelDeleteSchedule()">NO</button>
                </div>
            </div>

            <!-- add, switch and delete schedule -->
            <div class="schedule-selection">
                <button :ref="'renameSchedule'" v-bind:class="{'schedule-selection-button':schedule != defaultSchedule, 'schedule-selection-button-active':schedule == defaultSchedule}" v-for="(schedule, index) in schedules" :key="index" @click="setSchedule(schedule)">
                <span class="schedule-button-content">
                    <button class="schedule-selection-delete" @click.stop="promptDeleteSchedule(schedule)">
                    &#10008;
                    </button>
                    <button class="schedule-selection-edit" @click="renameScheduleButton(schedule, index)">
                    &#9998;
                    </button>
                    <span v-show="showScheduleRenamePrompt != schedule">{{ schedule }}</span>
                    <input :ref="'editScheduleNameInput'" v-show="showScheduleRenamePrompt == schedule" class="edit-schedule-input" v-model="renameScheduleInputField" @keyup.enter="renameSchedule(schedule, renameScheduleInputField)"/>
                </span>
                </button>
                <input class="new-schedule-input" placeholder="+ new schedule" v-model="newScheduleInputField" @keyup.enter="addSchedule(newScheduleInputField)"/>
            </div>
        </div>

        <!-- schedule grid -->
        <div class="courses-grid">
            <div v-for="(semester, semester_index) in getSchedule(defaultSchedule)" :key="semester_index" 
                v-bind:class="{'semester-block':hoverOverSemester!=semester_index, 'semester-block-highlighted':hoverOverSemester==semester_index}" 
                @dragenter="schedulerDragEnter($event, semester_index)" 
                @dragleave="schedulerDragLeave()" 
                @dragover.prevent 
                @drop="schedulerDrop($event, semester_index)">
                <div class="semester-title-row">
                    <span style="width: 100%;"><h3>Semester {{ semester_index + 1 }}</h3></span>
                    <span style="color:#a1a7a8; margin-top:-8px; margin-right:-8px;" class="schedule-search">
                    <button
                        type="button"
                        :ref="'addButton${index}'"
                        class="search-open"
                        @click="addCourseModal(semester_index)">
                        &#10010;
                    </button>
                    </span>
                </div>

                <div class="schedule-button-container" v-for="(course, course_index) in semester" :key="`${semester_index}-${course_index}`">
                    <button class="course-buttons" type="button" @click="goToCoursePage(course)" draggable="true" @dragstart="schedulerDrag($event, course, semester_index)">
                        <span :style="[arrayToHSLStyle(subjectColors[course.substring(0, 4)], 'color', [0, 15, 1], 100), {fontWeight: 700}]">{{ course.substring(0, 10) }}</span> <span style="color: #c7cfd0;">{{ course.substring(10) }}</span>
                    </button>
                    <button class="course-remove-button" type="button" @click="remove(semester_index, course, true, true)">
                        <span style="color:#b05f6e">&#10008;</span>
                    </button>
                </div>
            </div>
        </div>
    </div>
</template>

<script setup>
    import { userComputed, userData } from '@/stores/userStores'
    import { storeToRefs } from 'pinia'
    import { ref, onMounted } from 'vue'

    const showScheduleDeletionPrompt = ref(false);
    const showScheduleRenamePrompt = ref(false);
    const renameScheduleInputField = ref('');

    const userComputedStore = userComputed();
    const userDataStore = userData();
    const { fulfillments, possibilities } = storeToRefs(userComputedStore);
    const { schedules, defaultBeginSemester, defaultMajor, defaultSchedule } = storeToRefs(userDataStore);

    let schedulesLocal = {}


    onMounted(() => {
      console.log(`fulfillments before patch: ${JSON.stringify(fulfillments.value)}`);
      userComputedStore.$patch({ fulfillments: {test: 11, test2: 2} });
      console.log(`fulfillments after patch: ${JSON.stringify(fulfillments.value)}`);
      console.log(`possbilities: ${possibilities.value}`);
      console.log(`default begin semester: ${JSON.stringify(defaultBeginSemester.value)}, ${JSON.stringify(defaultMajor.value)}`);
    })

    // eslint-disable-next-line no-unused-vars
    function patchStore(store, variable, data) {
      store.$patch({[variable] : data})
    }

    // eslint-disable-next-line no-unused-vars
    function modalAdd(semester, course) {
      if (this.scheduleData[this.scheduleSelected].courses[semester].includes(course)) {
        this.remove(semester, course, true, true);
      }
      else {
        this.add(semester, course, true, true);
      }

      if (this.$refs.searchModal.closeModalOnSelection) {
        this.toggleSearchModal(false);
      }
    }

    function schedulerDrag(event, item, semester) {
        this.dragElement = item;
        this.dragFromSemester = semester;
        this.lastDropInsideZone = false;
        if (event != null) {
          event.dataTransfer.effectAllowed = "move";
        }
    }

    function schedulerDrop(event, dragToSemester) {
        event.preventDefault();
        if (this.dragElement != null) {
            if (this.dragFromSemester == -1) {
                this.add(dragToSemester, this.dragElement, true, true);
            }
            else if (this.dragFromSemester != -1 && this.dragFromSemester != dragToSemester) {
                this.remove(this.dragFromSemester, this.dragElement, false, false);
                this.add(dragToSemester, this.dragElement, false, false);
            }
        }
        this.hoverCounter = 0;
        this.hoverOverSemester = -1;
        this.lastDropInsideZone = true;
    }

    // eslint-disable-next-line no-unused-vars
    function schedulerRemove(event) {
      //console.log("remove func activated, hover counter: " + this.hoverCounter + " hoveroversem: " + this.hoverOverSemester + " dragFromSemester: " + this.dragFromSemester + ", " + this.dragElement);
      if (this.dragFromSemester == -1 || this.hoverOverSemester != -1 || this.lastDropInsideZone) {
        return
      }
      event.preventDefault();
      if (this.dragElement != null) {
          if (this.dragFromSemester != -1) {
              this.remove(this.dragFromSemester, this.dragElement, true, true);
          }
      }
      this.hoverOverSemester = -1;
    }

    function schedulerDragEnter(event, hoverOverSemester) {
        event.preventDefault();
        this.hoverOverSemester = hoverOverSemester;
        this.hoverCounter++;
    }
    
    function schedulerDragLeave() {
        this.hoverCounter--;
        if (this.hoverCounter == 0) {
            this.hoverOverSemester = -1;
        }
    }

    // eslint-disable-next-line no-unused-vars
    function add(semester, course, fulfill = true, recommend = true) {
      if (this.scheduleData[this.scheduleSelected].courses[semester].includes(course)) {
        return
      }
      this.scheduleData[this.scheduleSelected].courses[semester].push(course);
      this.$forceUpdate();
      this.$refs.searchModal.elementSelectionOccurrences = this.generateSearchOccurrences();
        
      // skips fulfill/recommend if this course occurs multiple times
      if (!(course in this.$refs.searchModal.elementSelectionOccurrences && this.$refs.searchModal.elementSelectionOccurrences[course].length > 1) && (fulfill || recommend)) {
        this.queryCalculations(fulfill, recommend);
      }

      this.saveuser();
    }

    function remove(semester, course, fulfill = true, recommend = true) {
      if (!this.scheduleData[this.scheduleSelected].courses[semester].includes(course)) {
        return
      }
      const index = [this.scheduleData[this.scheduleSelected].courses[semester].indexOf(course)];
      this.scheduleData[this.scheduleSelected].courses[semester].splice(index, 1);
      this.$forceUpdate();
      this.$refs.searchModal.elementSelectionOccurrences = this.generateSearchOccurrences();
        
      // skips fulfill/recommend if this course still occurs
      if (!(course in this.$refs.searchModal.elementSelectionOccurrences && this.$refs.searchModal.elementSelectionOccurrences[course].length > 0) && (fulfill || recommend)) {
        this.queryCalculations(fulfill, recommend);
      }
      
      this.saveuser();
    }
    
    // eslint-disable-next-line no-unused-vars
    function getDegree() {
      if (!this.scheduleData[this.scheduleSelected]) {
        return ''
      }
      return this.scheduleData[this.scheduleSelected].degree
    }

    function promptDeleteSchedule(schedule) {
      this.scheduleSelected = schedule;
      this.scheduleDeletionPrompt = true;
    }

    function cancelDeleteSchedule() {
      this.scheduleDeletionPrompt = false;
    }

    // eslint-disable-next-line no-unused-vars
    function commenceDeleteSchedule(schedule) {
      this.scheduleDeletionPrompt = false;
      this.deleteSchedule(schedule);
    }

    function renameScheduleButton(schedule, index) {
      this.scheduleBeingRenamed = schedule;
      this.renameScheduleInputField = schedule;
      this.scheduleBeingRenamedIndex = index;
      this.$nextTick(() => {
        if (this.$refs['editScheduleNameInput'][this.scheduleBeingRenamedIndex]) {
          this.$refs['editScheduleNameInput'][this.scheduleBeingRenamedIndex].focus();
        }
      });
    }

    function addSchedule(scheduleName, courseData = null) {
      if (this.schedules.includes(scheduleName)) {
        this.setSchedule(scheduleName);
        return
      }
      if (courseData == null) {
        let courseDataCourses = [];
        for (let i = 0; i < this.SemestersMax; i++) {
          courseDataCourses.push([]);
        }
        courseData = {'courses': courseDataCourses, 'degree': ''};
      }
      this.schedules.push(scheduleName);
      this.scheduleData[scheduleName] = courseData;
      this.setSchedule(scheduleName);

      this.saveuser();
    }

    function deleteSchedule(scheduleName) {
      delete this.scheduleData[scheduleName];
      this.schedules.splice([this.schedules.indexOf(scheduleName)], 1);
      if (scheduleName == this.scheduleSelected && this.schedules.length > 0) {
        this.setSchedule(this.schedules[0]);
      }
      if (this.schedules.length == 0) {
        this.scheduleSelected = '';
      }
      this.scheduleBeingRenamed = '';
      this.renameScheduleInputField = '';
      this.newScheduleInputField = '';

      this.saveuser();
    }

    function setSchedule(scheduleName, queryCalculations=true) {
      if (this.scheduleSelected == scheduleName) {
        return
      }
      this.scheduleSelected = scheduleName;
      this.degreeSelected = this.getDegree();
      this.scheduleBeingRenamed = '';
      this.renameScheduleInputField = '';
      this.newScheduleInputField = '';
      this.switchedSchedule = true;
      if (queryCalculations) {
        this.queryCalculations();
      }
    }

    function renameSchedule(oldScheduleName, newScheduleName) {
      if (oldScheduleName == newScheduleName || this.schedules.includes(newScheduleName)) {
        return
      }
      this.addSchedule(newScheduleName, this.scheduleData[oldScheduleName]);
      this.deleteSchedule(oldScheduleName);
      this.scheduleSelected = newScheduleName;

      this.saveuser();
    }

    function getSchedule(scheduleName) {
      if (!(schedulesLocal[scheduleName] && schedulesLocal[scheduleName].courses)) {
        return [];
      }
      return schedulesLocal[scheduleName].courses;
    }
</script>

<style scoped>

</style>