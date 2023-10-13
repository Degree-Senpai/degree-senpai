<template>
    <Header></Header>
    <div ref="SchedulerRef">
        <div class="toggle-calendar-button">
            <button @click="toggleComponent" >
                {{ isEnabled ? 'Disable' : 'Enable' }} Full Calendar
            </button>
        </div>
        <div class="mini-calendar" ref="miniCalContainer" v-if="!isEnabled">
            <MiniCalendar></MiniCalendar>
        </div>
        <div class="full-calendar" v-if="isEnabled">
            <FullCalendar></FullCalendar>
        </div>
        <SideBar v-if="showSidebar"></SideBar>
    </div>
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
            };
        },
        methods: {
            toggleComponent(){
                this.isEnabled = !this.isEnabled;
            },
            toggleSidebar() {
                this.showSidebar = !this.showSidebar;
            },
        },
        async created() {
            document.title = 'Degree Senpai - Scheduler';
        },
    }
</script>
  
<style>
    .full-calendar{
        position: fixed;
        top: 55%;
        right: 5px; 
        transform: translate(0, -50%);
    }
    .toggle-calendar-button{
        position: fixed;
        top: 7%;
        right: 15px;
        transform: translate(0, -50%);
        background-color: #535556;
        color: white;
        padding: 4px;
        font-size: 1em;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        width: 200px;
        text-align: center;
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
  