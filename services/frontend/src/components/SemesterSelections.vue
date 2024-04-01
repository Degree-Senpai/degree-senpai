<template>
    <!-- semester selection -->
    <div class="semester-selection-main">

        <!-- SCHEDULE SELECTION -->
        <div class="schedule-selection">
            <!-- deletion prompt -->
            <div ref="deletionPrompt" class="schedule-deletion-prompt" v-show="showScheduleDeletionPrompt">
                Confirm Deletion of Schedule '{{ scheduleSelected }}'?
                <div class="schedule-deletion-prompt-options">
                    <button class="schedule-confirm-delete" @click="deleteSchedule(scheduleSelected)">YES</button>
                    <button class="schedule-deny-delete" @click="cancelDeleteSchedule()">NO</button>
                </div>
            </div>

            <!-- add, switch and delete schedule -->
            <div class="schedule-selection">
                <button :ref="'renameSchedule'" v-bind:class="{'schedule-selection-button':schedule != scheduleSelected, 'schedule-selection-button-active':schedule == scheduleSelected}" v-for="(schedule, index) in schedules" :key="index" @click="setSchedule(schedule)">
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
            <div v-for="(semester, semester_index) in getSchedule(scheduleSelected)" :key="semester_index" 
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

</script>

<style scoped>

</style>