<template>
    <Header></Header>
    <div class="top">
        <h1>Home Page</h1>
    </div>
</template>
  
<script>
    import HeaderComponent from "@/components/PageHeader";
    export default {
        name: 'HomePage',
        components: {
            Header: HeaderComponent,
        },
        data() {
            return {
                data: 'test',
                userid: 'testuser',
            };
        },
    }
</script>
  
<style>
    .top {
        justify-content: left;
        text-align: left;
        padding-top: 10px;
        padding-left: 50px;
    }
</style>
  