<!-- CalendarCell.vue -->
<template>
    <div class="cell" @click="handleCellClick">
      {{ date }}
    </div>
  </template>
  
  <script>
  export default {
    props: [],
    methods: {
      handleCellClick() {
        this.$emit('cell-click', this.date);
      },
    },
  };
  </script>
  
  <style scoped>
  .cell {
    width: 50px;
    height: 50px;
    border: 1px solid #ccc;
    display: flex;
    justify-content: center;
    align-items: center;
    cursor: pointer;
  }
  </style>
  