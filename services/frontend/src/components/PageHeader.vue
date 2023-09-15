<template>
    <b-navbar>
        <div class="dark">
            <h1>Degree Senpai</h1>
            <div class="region-select">
                <div class="dropdown">
                    <button class="dropdown-button">{{region}}</button>
                    <div class="dropdown-content">
                        <button class="dropdown-items" v-for="option in regions" :key="option" @click="setRegion(option)">{{ option }}</button>
                    </div>
                </div>
            </div>
        </div>
    </b-navbar>
</template>

<script>
    import axios from 'axios';
    export default {
        data() {
            return {
                userid: "testuser",
                regions: ["AP", "BR", "ESPORTS", "KR", "LATAM", "NA", "EU"],
                region: 'Select Region',
                response: '',
            };
        },
        methods: {
            async setRegion(region) {
                let data = {
                    "userid": this.userid,
                    "region": region,
                };
                const response = await axios.post('/api/setregion', data);
                this.response = response.data;
                this.region = region;
            },
        }
    };
</script>

<style>
    .dark {
        background-color: rgb(33, 35, 36);
        color:rgb(222, 207, 231);
        display: flex;
    }

    .dark h1 {
        margin-left: 50px;
        font-weight: 600;
    }

    .region-select {
        margin-left: auto;
        margin-right: 50px;
        margin-top: 5px;
        justify-self: right;
        justify-content: right;
    }

    .dropdown {
        display: inline-block;
        width: 120px;
        height: 30px;
    }

    /* Show the dropdown menu on hover */
    .dropdown:hover .dropdown-content {
        display: block;
    }
    .dropdown-button {
        background-color: #535556;
        color: white;
        padding: 8px;
        font-size: 1em;
        border: none;
        border-radius: 8px;
        cursor: pointer;
        width: 120px;
    }
    .dropdown-button:hover {
        background-color: #6b6c6c;
    }

    /* Dropdown Content (Hidden by Default) */
    .dropdown-content {
        display: none;
        position: absolute;
        background-color: #272525;
        color: white;
        min-width: 160px;
        box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
        z-index: 1;
        border-radius: 8px;
    }

    .dropdown-items {
        background-color: #272525;
        border-radius: 8px;
        border: 0px;
        margin: 2px;
        padding: 2px;
        text-align: center;
        color: white;
        display: flex;
        width: 99%;
    }
    .dropdown-items:hover {
        background-color: #474444;
        color: white;
    }
</style>
