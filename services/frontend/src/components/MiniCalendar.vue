<template>
  <div>
    <table>
      <thead>
        <tr>
          <th></th>
          <th>M</th>
          <th>T</th>
          <th>W</th>
          <th>R</th>
          <th>F</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="hour in hours" :key="hour">
          <td>{{ hour }}</td>
          <td v-for="day in days" :key="day">
            <CalendarCell @custom-event="toggleSidebar" @custom-event2="changeDayandHour(day.name, hour.name)" @click="$emit('custom-event', 'custom-event2')"  cellDay=day.name cellHour=hour.name size="small"></CalendarCell>
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
            hours: ["8", "9", "10", "11", "12", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"],
        };
    },
    components: { CalendarCell }
};
</script>

<style scoped>
/* Add your CSS styling here for the calendar cells */
.labelText{
  size: 12px;
}
</style>