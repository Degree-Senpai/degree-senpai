<template>
    <Header></Header>
    <div class="top">
        <h1>Degree Senpai</h1>
        {{ data }}
    </div>
</template>
  
<script>
    import HeaderComponent from "@/components/PageHeader";
    import axios from "axios";
    export default {
        name: 'DegreeSenpai',
        components: {
            Header: HeaderComponent,
        },
        data() {
            return {
                data: 'test',
                userid: 'testuser',
                region: 'NA',
            };
        },
        methods: {
            async test() {
                const reply = await axios.get('/');
                this.data = reply.data;
            },
            async createuser() {

                let data = {
                    "userid": this.userid,
                    "region": this.region,
                };
                await axios.post('/api/newuser', data);
            },
        },
        created() {
            this.test()
            this.createuser()
        }
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
  