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
    width: 40px;
    height: 30px;
    border: 2px solid #ccc;
    display: flex;
    justify-content: center;
    align-items: center;
    cursor: pointer;
  }

  .large{
    width: 230px;
    height: 60px;
    border: 2px solid #ccc;
    display: flex;
    justify-content: center;
    align-items: center;
    cursor: pointer;
  }
  </style>
  