<template>
    <aside class="sidebar">
      
    </aside>
  </template>
  
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
  
  .sidebar ul {
    list-style: none;
    padding: 0;
  }
  
  .sidebar li {
    padding: 10px;
  }
  
  .sidebar a {
    color: white;
    text-decoration: none;
  }
  
  .sidebar .router-link-active {
    background-color: #555;
  }
  </style>
  