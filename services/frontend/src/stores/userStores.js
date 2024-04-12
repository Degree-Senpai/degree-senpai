import { defineStore } from 'pinia'
import { ref } from 'vue'

export const userComputed = defineStore('userComputed', () => {
  const fulfillments = ref({})
  const possibilities = ref({})

  return { fulfillments, possibilities }
})

export const userData = defineStore('userData', () => {
  const schedules = ref({})

  const defaultBeginSemester = ref(60) // represented by semesters since fall 2000
  const defaultMajor = ref('computer science')
  const defaultSchedule = ref('')

  return { schedules, defaultBeginSemester, defaultMajor, defaultSchedule }
})