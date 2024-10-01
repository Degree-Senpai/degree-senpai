import { defineStore } from 'pinia'
import { ref } from 'vue'

export const userScheduleData = defineStore('userScheduleData', () => {
  const selectedCourses = ref({}) // dictionary (by year) of list of CRN representing user selected courses
  const presentSemesters = ref([]) // list of semesters to render
  const activeSemester = ref(60) // represented by semesters since fall 2000
  const selectedMajor = ref('computer science')

  return { selectedCourses, presentSemesters, activeSemester, selectedMajor }
})

export const userScheduleComputed = defineStore('userScheduleComputed', () => {
  const generatedSchedules = ref({}) // dictionary (by year) of 2D list of CRNs (shape: (schedule, classes))

  return { generatedSchedules }
})

export const userPlannerComputed = defineStore('userPlannerComputed', () => {
  const fulfillments = ref({})
  const possibilities = ref({})

  return { fulfillments, possibilities }
})

export const classData = defineStore('classData', () => {
  const allCourses = ref({}) // dictionary of CRN: CourseInstance object

  return { allCourses }
})