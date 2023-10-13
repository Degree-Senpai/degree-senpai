<template>
    <div :class="['cell', size]" @click="handleCellClick" ref="CalendarCell">
      {{ date }}
    </div>
  </template>
  
  <script>
import Scheduler from '@/pages/Scheduler.vue';



  export default {
    props: {
        size: {
            type: String,
            default: "default",
        }
    },
    data() {
        return {
          showSidebar: false,
        };
    },
    methods: {
        handleCellClick() {
            this.$emit('cell-click', this.date);
            Scheduler.$refs.SchedulerRef.toggleSidebar();
            //this.toggleSidebar();
        },
    },
    components: { }
};
  </script>
  
  <style scoped>
  .small {
    width: 40px;
    height: 30px;
    border: 1px solid #ccc;
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
  