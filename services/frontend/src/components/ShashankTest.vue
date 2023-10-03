<template>
  <div>
    <div v-for="(dept,index) in depts"
    :key="index">
      {{ dept.name }}
      <ul v-for="(course,index2) in dept.courses"
      :key="index2">
        <li>{{ course }}</li>
      </ul>
    </div>
  </div>
</template>

<script>
  export default {
    data() {
      return {
        depts: [{name: "CSCI", courses: ["CS1", "DS", "FOCS", "Comp_Org"]}, {name: "HASS", courses: ["H1", "H2", "H3", "H4"]}],
        courses: []
      };
    },
        
    methods: {

    }
  };
</script>

<style scoped>

</style>