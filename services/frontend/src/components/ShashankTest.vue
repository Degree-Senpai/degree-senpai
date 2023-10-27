<template>
  <div class="grid">
    <div class="dept" v-for="(dept,index) in depts"
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
        depts: [{name: "CSCI", courses: ["CS1", "DS", "FOCS", "Comp_Org", "r", "r2", "34"]}, {name: "HASS", courses: ["H1", "H2", "H3", "H4"]},
        {name: "PHYS", courses: ["Physics 1", "Physics 2", "Physics 3", "Physics 4"]}, {name: "ECON", courses: ["E1", "E2", "E3", "E4"]},
        {name: "MGMT", courses: ["Intro to MGMT", "Financial Accting", "Managerial Finance", "ML Apps"]}],
        courses: []
      };
    },
        
    methods: {

    }
  };
</script>

<style scoped>
  .dept {
    grid-column: span 1;
  }
  div {
    border: 2px solid blue;
    padding: .5em;
  }
  .grid {
    max-width: 60rem;
    margin: 0 auto;
    display: column;
    columns: 2;
    grid-gap: 10px;
  }

  .grid > * {
    break-inside: avoid;
    margin-bottom: 1em;
  }
</style>