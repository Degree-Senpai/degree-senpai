<template>
    <Header></Header>
        <button class="toggle-calendar-button" @click="toggleComponent">
                {{ isEnabled ? 'Disable' : 'Enable' }} Full Calendar
        </button>
        <div class="mini-calendar" ref="miniCalContainer" v-if="!isEnabled">
            <MiniCalendar @custom-event="toggleSidebar" @custom-event2="changeDayandHour(day, hour)"></MiniCalendar>
        </div>
        <div class="full-calendar" v-if="isEnabled">
            <FullCalendar @custom-event="toggleSidebar" @custom-event2="changeDayandHour(day, hour)" ></FullCalendar>
        </div>
        <SideBar v-if="showSidebar" day=dayInput hour=hourInput></SideBar>
        
</template>
  
<script>
    import HeaderComponent from "@/components/PageHeader";
    import MiniCalendar from "@/components/MiniCalendar.vue";
    import FullCalendar from "@/components/FullCalendar.vue";
    import SideBar from "@/components/SideBar.vue";
    export default {
        name: 'HomePage',
        components: {
    MiniCalendar,
    Header: HeaderComponent,
    FullCalendar,
    SideBar,
},
        data() {
            return {
                data: 'test',
                isEnabled: false,
                showSidebar: false,
                dayInput: 'test',
                hourInput: 'test',
            };
        },
        methods: {
            toggleComponent(){
                this.isEnabled = !this.isEnabled;
            },
            toggleSidebar() {
                this.showSidebar = !this.showSidebar;
            },
            changeDayandHour(day, hour){
                this.dayInput = day;
                this.hourInput = hour;
            }
        },
        async created() {
            document.title = 'Degree Senpai - Scheduler';
        },
    }
</script>
  
<style>
    .full-calendar{
        position: fixed;
        top: 52%;
        right: 5px; 
        transform: translate(0, -50%);
    }
    .toggle-calendar-button{
        padding: 6px;
        padding-left: 12px;
        padding-right: 12px;
        border-radius: 4px;
        border: none;
        font-size: 16px;
        color:#b6bec4;
        background-color: #1b1d1d;
    }
    .mini-calendar{
        position: fixed;
        top: 50%;
        right: 5px; 
        transform: translate(0, -50%);
    }
    .top {
        justify-content: left;
        text-align: left;
        padding-top: 10px;
        padding-left: 50px;
    }

    .course-info{
        background-color: gray;
        display:flex;
        height: 90vh;
        width: 100%;
        flex: 8;
        border: 2px solid black;
        overflow-y: auto;
        min-width: 400px;
        max-width: 650px;
        text-align: center;
    }
</style>
  