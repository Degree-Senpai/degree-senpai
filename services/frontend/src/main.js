import 'bootstrap/dist/css/bootstrap.css';
import axios from 'axios';

import { createApp } from 'vue'
// import { createPinia } from 'pinia';
import App from './App.vue'
import router from './router'

// const pinia = createPinia()
const app = createApp(App)

axios.defaults.withCredentials = true;
axios.defaults.baseURL = 'http://localhost:5000/';

app.use(router);
// app.use(pinia);
app.mount('#app');
/*
import {colorStore} from './utilities/store.js'
import { Colors, ColorPalette, Color } from './utilities/colorservice.js'
const colorstore = colorStore();

const colors = Colors(ColorPalette(
    Color(40, 20, 50, 1), // main
    Color(50, 10, 70, 1), // highlight
    Color(80, 15, 60, 1), // layerouter
    Color(75, 20, 55, 1), // layermid
    Color(70, 22, 52, 1), // layerinner
    Color(100, 30, 60, 1), // iconmain
    Color(100, 20, 70, 0.9), // iconsub
    Color(100, 40, 50, 1) // iconsmall
));

colorstore.colorService = colors;*/
