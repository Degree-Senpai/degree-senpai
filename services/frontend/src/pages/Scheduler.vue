<template>
    <Header></Header>
        <button class="toggle-calendar-button" @click="toggleComponent">
                {{ isEnabled ? 'Disable' : 'Enable' }} Full Calendar
        </button>
        <div class="mini-calendar" ref="miniCalContainer" v-if="!isEnabled">
            <MiniCalendar @custom-event="toggleSidebar"></MiniCalendar>
        </div>
        <div class="full-calendar" v-if="isEnabled">
            <FullCalendar @custom-event="toggleSidebar" ></FullCalendar>
        </div>
        <SideBar v-if="showSidebar"></SideBar>
        
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
  