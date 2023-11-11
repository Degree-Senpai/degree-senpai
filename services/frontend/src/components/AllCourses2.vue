<template>
  <div>
    <h2>Filter Courses</h2>
    <select v-model="selectedSubject">
      <option value="">All Subjects</option>
      <option v-for="subject in subjects" :key="subject" :value="subject">
        {{ subject }}
      </option>
    </select>
  </div>
</template>

<script>
export default {
  data() {
    return {
      selectedSubject: "",
      subjects: ["MATH", "SCI", "ENG"], // Add more subjects as needed
    };
  },
  watch: {
    selectedSubject() {
      this.$emit("filter-change", this.selectedSubject);
    },
  },
};
</script>