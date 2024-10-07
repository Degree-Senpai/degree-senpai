import 'bootstrap/dist/css/bootstrap.css';
import axios from 'axios';
import { createApp } from 'vue'
import { createPinia } from 'pinia'
import App from './App.vue'
import router from './router'

const pinia = createPinia()
const app = createApp(App)

app.use(pinia)
app.use(router);
app.mount('#app')


axios.defaults.withCredentials = true;
axios.defaults.baseURL = 'http://localhost:5000/';
