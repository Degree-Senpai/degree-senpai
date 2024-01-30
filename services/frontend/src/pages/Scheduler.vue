<template>
    <div>
        <Header></Header>
        <div class="landing">
            <LandingComponent/>
        </div>
        <button class="toggle-calendar-button" @click="toggleComponent" >
            {{ isEnabled ? 'Disable' : 'Enable' }} Full Calendar
        </button>
        <div class="mini-calendar" ref="miniCalContainer" v-show="!isEnabled">
            miniCalendar
            <MiniCalendar @custom-event="toggleSidebar" @custom-event2="changeDayandHour(day, hour)"></MiniCalendar>
        </div>
        <div class="full-calendar-container" v-show="isEnabled">
            <div class="full-calendar">
                <FullCalendar/>
            </div>
        </div>
        <SideBar v-if="showSidebar" day=dayInput hour=hourInput></SideBar>
    </div>
        
</template>
  
<script>
    import HeaderComponent from "@/components/PageHeader";
    import MiniCalendar from "@/components/MiniCalendar.vue";
    import FullCalendar from "@/components/FullCalendar.vue";
    import SideBar from "@/components/SideBar.vue";
    import LandingComponent from "@/components/LandingComponent.vue";
    export default {
        name: 'HomePage',
        components: {
            MiniCalendar,
            Header: HeaderComponent,
            FullCalendar,
            SideBar,
            LandingComponent
        },
        data() {
            return {
                data: 'test',
                isEnabled: true,
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
  
<style scoped>
    .landing {
        position: absolute;
        width: 42vw;
        height: 90vh;
        margin: 4px;
        padding: 4px;
    }
    .full-calendar-container{
        position: absolute;
        width: 55vw;
        height:90vh;
        margin: 4px;
        padding: 4px;
        margin-top: 10px;
        right: 10px;
        border: 2px solid #262628;
        border-radius: 2px;
    }
    .full-calendar{
        position: absolute;
        border-radius: 2px;
        left: 48px;
        right: 4px;
        top: 4px;
        bottom: 4px;
    }
    .toggle-calendar-button{
        position: fixed;
        z-index: 9999;
        top: 16px;
        right: 255px;
        background-color: #535556;
        color: white;
        padding: 4px;
        font-size: 1em;
        border: none;
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
  