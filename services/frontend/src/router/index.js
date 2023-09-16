import { createRouter, createWebHistory } from 'vue-router'
import Scheduler from '../pages/Scheduler.vue'
import DegreePlanner from '../pages/DegreePlanner.vue'

const routes = [
  {
    path: '/',
    name: 'home',
    component: Scheduler
  },
  {
    path: '/scheduler',
    name: 'scheduler',
    component: Scheduler
  },
  {
    path: '/degreeplanner',
    name: 'DegreePlanner',
    component: DegreePlanner
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

export default router
