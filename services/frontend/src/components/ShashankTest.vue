<template>
  <div>
    <div v-for="dept in depts">
      {{ dept.name }}
      <ul v-for="course in dept.courses">
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