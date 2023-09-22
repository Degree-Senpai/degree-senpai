<template>
    <Header></Header>
    <div class="course-info">
        <h1>Course Info</h1>
        <div class="mini-calendar" ref="miniCalContainer">
            <MiniCalendar></MiniCalendar>
        </div>
    </div>
</template>
  
<script>
    import HeaderComponent from "@/components/PageHeader";
    import MiniCalendar from "@/components/MiniCalendar.vue";
    export default {
        name: 'HomePage',
        components: {
            MiniCalendar,
            Header: HeaderComponent,
        },
        data() {
            return {
                data: 'test',
            };
        },
        async created() {
            document.title = 'Degree Senpai - Scheduler';
        },
    }
</script>
  
<style>
    .mini-calendar{
        justify-content: right;
        position: fixed;
        left: 85%;
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
  