<template>
  <div id="app">
    <FilterCourses @filter-change="handleFilterChange" />
    <CourseList :courses="courses" :selectedSubject="selectedSubject" />
  </div>
</template>

<script>
import axios from "axios";
import CourseList from "@/components/AllCourses.vue";
import FilterCourses from "@/components/AllCourses2.vue";

export default {
  components: {
    CourseList,
    FilterCourses,
  },
  data() {
    return {
      courses: [],
      selectedSubject: "",
    };
  },
  methods: {
    handleFilterChange(subject) {
      this.selectedSubject = subject;
    },
  },
  mounted() {
        // Generate 25 fake courses
    for (let i = 0; i < 25; i++) {
        const course = {
        id: i + 1,
        name: "place"
        }
        // code: `${name.arrayElement(["MATH", "SCI", "ENG"])}-${name({
        //     min: 1000,
        //     max: 9999,
        // })}`,
        // };
        this.courses.push(course);
    }
  },
};
</script>

<style>
/* #app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
} */
</style>