import { defineStore } from 'pinia'
import { ref } from 'vue'

export const schoolData = defineStore('schoolData', () => {
    const courseInstances = ref({}) // dictionary of CRN: CourseInstance object
    const courses = ref({}) // dictionary of course name: Course object

    // subjects belong to schools
    const schools = ref([]) // a list of objects containing subject group name and subject names
    const subjects = ref({}) // {subject name: object containing abbreviation and details}

    // majors belong to departments
    const departments = ref([]) // a list of objects containing school name and majors
    const majors = ref({}) // {major name: object containing abbreviation and details}
  
    return { courses, courseInstances, schools, subjects, departments, majors }
  })
  
  export const colorStore = defineStore('colors', () => {
    const colorPalettes = ref({}) // name: palette object
    const colorServices = ref({}) // name: service (usually there should only be one colorService here)
  
    return {colorPalettes, colorServices}
  })
