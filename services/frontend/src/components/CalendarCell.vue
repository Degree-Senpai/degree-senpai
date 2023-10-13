<!-- CalendarCell.vue -->
<template>
    <div :class="['cell', size]" @click="handleCellClick">
      {{ date }}
    </div>
  </template>
  
  <script>
  export default {
    props: {
      size: {
        type: String,
        default: "default",
      }
    },
    methods: {
      handleCellClick() {
        this.$emit('cell-click', this.date);
      },
    },
  };
  </script>
  
  <style scoped>
  .small {
    width: 2vw;
    height: 4vh;
    border: 2px solid #ccc;
    display: flex;
    justify-content: center;
    align-items: center;
    cursor: pointer;
  }

  .large{
    width: 8vw;
    min-width: 80px;
    height: 6vh;
    min-height: 40px;
    border: 2px solid #ccc;
    display: flex;
    justify-content: center;
    align-items: center;
    cursor: pointer;
  }
  </style>
  