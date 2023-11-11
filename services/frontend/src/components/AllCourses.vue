<template>
 <div>
    <h2>Course List</h2>
    <ul>
      <li v-for="course in filteredCourses" :key="course.id">
        {{ course.name }}
      </li>
    </ul>
  </div>
</template>

<script>
 export default {
  props: {
    courses: Array,
    selectedSubject: String,
  },
  computed: {
    filteredCourses() {
      return this.selectedSubject
        ? this.courses.filter(course =>
            course.code.startsWith(this.selectedSubject)
          )
        : this.courses;
    },
  },
};
</script>

<style scoped>

</style>