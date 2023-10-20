<template>
    <div :class="['cell', size]" @click="$emit('custom-event', 'custom-event2')" ref="CalendarCell">
      {{ date }}
    </div>
  </template>
  
  <script>



  export default {
    props: {
        size: {
            type: String,
            default: "default",
        },
        cellDay: {
            type: String,
            default: "Default",
        },
        cellHour: {
            type: String,
            default: "Default"
        }
    },
    data() {
        return {
          showSidebar: false,
          day: "Default",
          hour: "Default",
        };
    },
    methods: {
        handleCellClick() {
            this.$emit('custom-event');
            this.$emit('custom-event2', this.day, this.hour)
            this.$emit('cell-click', this.date);
            
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
  