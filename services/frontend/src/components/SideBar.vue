<template>
    <aside class="sidebar">
      <div>
        Available Classes at {{ hour }} on a {{ day }}:
      </div>
    </aside>
  </template>
  
  <script>
  export default{
    props: {
      day : {
        type:String,
        default: "DefaultDay",
      },
      hour : {
        type: String,
        default: "DefaultHour"
      }
    }
  }

  </script>
  <style scoped>
  .sidebar {
    width: 550px;
    background-color: #1b1b1b;
    position: fixed;
    padding: 4px;
    border: 2px solid;
    border-color: black;
    top: 15;
    left: 0;
    height: 90%;
    overflow-y: auto;
    z-index: 100;
  }

  
  .sidebar .router-link-active {
    background-color: #555;
  }
  </style>
  