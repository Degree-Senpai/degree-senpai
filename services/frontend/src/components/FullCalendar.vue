<template>
    <div>
      <table>
        <thead>
          <tr>
            <th></th>
            <th>Mon</th>
            <th>Tues</th>
            <th>Wed</th>
            <th>Thurs</th>
            <th>Fri</th>
          </tr>
        </thead>
        <tbody>
          <tr v-for="hour in hours" :key="hour">
            <td>{{ hour }}</td>
            <td v-for="day in days" :key="day">
              <calendar-cell size="large"> </calendar-cell>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </template>
  
  <script>
  import CalendarCell from './CalendarCell.vue';
  
  export default {
      data() {
          return {
              days: ['M', 'T', 'W', 'Th', 'F'],
              hours: ["8AM", "9AM", "10AM", "11AM", "12PM", "1PM", "2PM", "3PM", "4PM", "5PM", "6PM", "7PM", "8PM", "9PM"],
          };
      },
      components: { CalendarCell }
  };
  </script>
  
  <style scoped>
  /* Add your CSS styling here for the calendar cells */
  </style>