<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<script>
  export default {
    name: 'DegreeSenpai',
    components: [],
    metaInfo() {
      return {
        title: "DegreeSenpai",
        titleTemplate: null,
        meta: [
          {
            vmid: "description",
            name: "description",
            content:
              "Degree Planner and Course Scheduler for RPI Students",
          },
          {
            vmid: "keywords",
            content: "RPI, Degree Senpai, RPI Scheduler, Course Picker, Course Scheduler, Degree Planner, Rensselaer Polytechnic Institute",
          },
          { property: "og:title", content: "Degree Planner and Course Scheduler" },
          { property: "og:site_name", content: "Degree Senpai" },
          { property: "og:type", content: "website" },
          { name: "robots", content: "index" },
          { name: "color-scheme", content: this.colorSchemeMeta },
        ],
      };
    }
  }
</script>

<style>
  body {
    font-family: 'Trebuchet MS', 'Lucida Sans Unicode', 'Lucida Grande', 'Lucida Sans', Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: left;
    color: #e6e7e7;
    background-color: #363839;
  }
</style>
