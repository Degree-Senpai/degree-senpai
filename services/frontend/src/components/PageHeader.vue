<template>
    <div class="dark">
        <span v-if="False">
            <button class="home" @click="goToPage('/')"><img src="../assets/logo.png" style="width: 42px; height: 42px; align-self: center;"></button>
        </span>
        <button class="home" @click="goToPage('/')">Degree Senpai</button>
        <span style="width: 50px"></span>
        <button class="nav-bar-item" style="color: #9aa1ba" @click="goToPage('/scheduler')">Scheduler</button>
        <button class="nav-bar-item" style="color: #b08eb0" @click="goToPage('/degreeplanner')">Degree Planner</button>
        <div class="semester-select">
            <div class="dropdown">
                <button class="dropdown-button">{{selectedSemester}}</button>
                <div class="dropdown-content">
                    <button class="dropdown-items" v-for="option in semesterOptions" :key="option" @click="setSemester(option)">{{ option }}</button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
    import axios from 'axios';
    export default {
        data() {
            return {
                userid: "nulluser",
                semesterOptions: ["2022 Fall", "2023 Spring", "2023 Fall", "2024 Spring"],
                selectedSemester: 'Select Semester',
                response: '',
            };
        },
        methods: {
            saveToLocalStorage(key, value) {
                try {
                    localStorage.setItem(key, value);
                    return true;
                } catch (error) {
                    console.error("Failed to save to localStorage:", error);
                    return false;
                }
            },
            getFromLocalStorage(key) {
                try {
                    const value = localStorage.getItem(key);
                    if (value == "undefined") {
                        return undefined;
                    }
                    return value;
                } catch (error) {
                    console.error("Failed to retrieve from localStorage:", error);
                    return null;
                }
            },
            removeFromLocalStorage(key) {
                try {
                    localStorage.removeItem(key);
                    return true;
                } catch (error) {
                    console.error("Failed to remove from localStorage:", error);
                    return false;
                }
            },
            loadUser() {
                let userid = this.getFromLocalStorage('userid');
                if (userid) {
                    this.userid = userid;
                }
                let selectedSemester = this.getFromLocalStorage('selectedSemester');
                if (selectedSemester) {
                    this.selectedSemester = selectedSemester;
                }
            },
            goToPage(page) {
                this.$router.push(page);
            },
            async setSemester(semester) {
                let data = {
                    "userid": this.userid,
                    "semester": semester,
                };
                const response = await axios.post('/api/setsemester', data);
                this.response = response.data;
                this.selectedSemester = semester;
                this.saveToLocalStorage('selectedSemester', this.selectedSemester);
            },
        },
        async created() {
            this.loadUser();
        }
    };
</script>

<style scoped>
    .dark {
        padding-top: 8px;
        padding-bottom: 4px;
        align-items: center;
        background-color: rgb(33, 35, 36);
        display: flex;
    }

    .home {
        font-weight: 600;
        margin-left: 20px;
        font-size: 28px;
        color:rgb(167, 176, 184);
        background-color: rgba(0,0,0,0);
        border: none;
    }

    .nav-bar-item {
        min-width: 130px;
        margin-left: 12px;
        margin-right: 12px;
        padding: 4px;
        font-weight: 550;
        font-size: 20px;
        border: none;
        border-radius: 4px;
        background-color: rgba(0,0,0,0);
    }

    .nav-bar-item:hover {
        background-color: rgba(184, 174, 174, 0.2);
    }

    .semester-select {
        margin-left: auto;
        margin-right: 20px;
        justify-self: right;
        justify-content: right;
    }

    .dropdown {
        display: inline-block;
        width: 200px;
        height: 20px;
    }

    /* Show the dropdown menu on hover */
    .dropdown:hover .dropdown-content {
        display: block;
    }
    .dropdown-button {
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
        border-radius: 4px;
        text-align: center;
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
