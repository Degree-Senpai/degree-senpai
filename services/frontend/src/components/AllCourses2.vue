<template>
  <div>
    <h2>Filter Courses</h2>
    <select v-model="selectedSubject">
      <option value="">All Subjects</option>
      <option v-for="subject in subjects" :key="subject" :value="subject">
        {{ subject }}
      </option>
    </select>
    <div v-for="(course, index) in courses" :key="index">
        
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      selectedSubject: "",
      subjects: ["MATH", "SCI", "ENG"], // Add more subjects as needed,
      courses: []
    };
  },
  watch: {
    selectedSubject() {
      this.$emit("filter-change", this.selectedSubject);
    },
  },
  mounted() {
    for (let i = 0; i < 25; i++) {
        const course = {
        id: i + 1,
        name: "place"
        };
        this.courses.push(course);
        // code: `${name.arrayElement(["MATH", "SCI", "ENG"])}-${name({
        //     min: 1000,
        //     max: 9999,
        // })}`,
        // };
        // this.courses.push(course);
    }
  }
};
</script>