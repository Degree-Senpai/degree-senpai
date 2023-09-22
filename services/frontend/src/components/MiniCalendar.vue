<!-- Calendar.vue -->
<template>
    <div>
      <div class="calendar">
        <calendar-cell
          v-for="date in calendarDates"
          :key="date"
          :date="date"
          @cell-click="handleCellClick"
        ></calendar-cell>
      </div>
    </div>
  </template>
  
  <script>
  import CalendarCell from './CalendarCell.vue';
  
  export default {
    components: {
      CalendarCell,
    },
    data() {
      return {
        startDate: new Date(2023, 8, 1), // September 2023 (months are 0-based)
        daysInMonth: 30, // Update this to match the number of days in the current month
      };
    },
    computed: {
      calendarDates() {
        const dates = [];
        for (let i = 1; i <= this.daysInMonth; i++) {
          dates.push(new Date(this.startDate.getFullYear(), this.startDate.getMonth(), i));
        }
        return dates;
      },
    },
    methods: {
      handleCellClick(date) {
        // Handle cell click here, e.g., emit an event or perform an action.
        console.log('Clicked on date:', date);
      },
    },
  };
  </script>
  
  <style scoped>
  .calendar {
    display: grid;
    grid-template-columns: repeat(5, 1fr); /* 7 columns for days of the week */
    gap: 0px;
  }
  </style>
  