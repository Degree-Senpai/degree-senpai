<template>
    <div>
        <!-- SEARCH BAR -->
        <input
            ref="searchBar"
            v-model="searchBarInput"
            @input="searchBarHandler"
            placeholder="Enter Course Name"
            @keyup.enter="searchBarEnter"
        />

        <!-- FILTERS -->
        <div>
            <!-- SUBJECT FILTER -->
            <!-- iterates through all schools -->
            <div v-for="{school, school_index} in schools" :key="school_index">
                <span> {{ school.name }} </span>
                <!-- iterates through all subjects -->
                <div v-for="{subject, subject_index} in school.subjects" :key="subject_index">
                    <button type="button" @click="filterSubject(subject.name)"> {{ subject.abbreviation }} </button>
                </div>
            </div>

            <!-- CREDIT FILTER -->
            <!-- PROFESSOR FILTER -->
            <!-- TIME FILTER -->
            <!-- LEVEL FILTER -->
            <!-- OUT OF MAJOR FILTER -->
        </div>

        <!-- SEARCH RESULTS -->
        <div>
            <span>NEXT</span> {{ searchResultsPage }} <span>PREVIOUS</span>
            <ul>
                <!-- COURSE BLOCK -->
                <li 
                    v-for="(course, course_index) in searchResults" 
                    :key="course_index"
                    @click="selectCourse(course.name)"
                    draggable="true"
                    @dragstart="dragCourse($event, course.name)">

                    {{ course.name }} <br>
                    {{ course.professor }} <br>
                    {{ course.description }}

                    <!-- SECTIONS -->
                    <li
                        v-if="flag_SectionEnable"
                        v-for="(section, section_index) in course.sections"
                        :key="section_index"
                        @click="toggleSection(section.crn)">
                        {{ section.crn }} {{ section.description }}
                    </li>
                </li>
            </ul>
        </div>
    </div>
</template>

<script setup>
    /* global Module */
    import { ref } from 'vue';
    import { storeToRefs } from 'pinia';

    import { userData } from '@/stores/userStores.js';
    import { schoolData } from '@/stores/userStores.js';

    const userDataStore = userData();
    const schoolDataStore = schoolData();

    // REACTIVE VARIABLES FROM STORES
    const {courseInstances, courses, schools} = storeToRefs(schoolDataStore);
    const {selectedCourses, activeSemester} = storeToRefs(userDataStore);

    const searchBarInput = ref("");
    const searchResultsPage = ref(0);
    const searchResults = ref([]);

    const flag_SectionEnable = ref(false);
</script>

<style scoped>

</style>
