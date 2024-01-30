<template>
  <div class="container">
    <div>
      <input
        ref="searchBar"
        class="search-input"
        v-model="searchInput"
        @input="inputHandler"
        placeholder="Enter course name"
        @keyup.enter="selectEnter"
      />
    </div>
    <div class="subject-filter">
      <div class="subject-group" :style="arrayToHSLBackground(searchFilterGroupColors[subject_group.title.toLowerCase()])" v-for="(subject_group, subject_group_index) in searchFilterGroups" :key="subject_group_index">
        <span :style="getSubjectTitle(subject_group.title)">{{ extractTitle(subject_group.title) }}</span><br>
        <button class="subject-buttons" :style="getFilterColor(subject)" type="button" v-for="(subject, subject_index) in subject_group.elements" :key="subject_index" @click="filterElement(subject)">
        {{ extractTitle(subject) }}
        </button>
        <br>
      </div>
    </div>
    <div ref="searchMatchList">
      <ul class="search-results">
        <li v-show="searchMatches.length === 0" class="no-results">No results found</li>
        <li v-show="searchContextSize < searchMatches.length && searchDisplayContext > 0" @click="decrementSearchPage">
          <span class="search-traversal-button">PREVIOUS</span>
        </li>
        <li class="search-result" v-for="(course, index) in searchMatches.slice(searchDisplayContext * searchContextSize, (searchDisplayContext + 1) * searchContextSize)" :key="index" @click="selectMatch(index)" draggable="true" @dragstart="elementDragStart($event, course.display_name)">
          <div>
          <span class="search-result" :style="getElementColorFront(course.search_name)"> {{ course.display_name.substring(0, 10) }} </span> 
          <span v-bind:class="{'search-result-body':!(course.display_name in elementSelectionOccurrences), 'search-result-body-selected':course.display_name in elementSelectionOccurrences}" :style="getElementColorRear(course.display_name)"> {{ course.display_name.substring(10) }} </span>
          <span class='search-result-body2'> credits: {{ readData(course.search_name, 'credits') }} </span>
          <br>
          <div class="search-result-description">{{ readData(course.search_name, 'description') }} <br></div>
          <span class="selected-tag" v-show="course.display_name in elementSelectionOccurrences"> (selected in semester {{ getElementOccurrences(course.display_name) }}) </span>
        </div>
        </li>
        <li v-show="(searchDisplayContext + 1) * searchContextSize < searchMatches.length" @click="incrementSearchPage">
          <span class="search-traversal-button" >NEXT</span>
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
import debounce from 'lodash/debounce';
import axios from 'axios';

export default {
  data() {
    return {
      /* -------- PARAMETERS -------- */
      searchDisplayContext: 0,
      searchContextSize: 50,
      closeModalOnSelection: false,

      /* -------- SEARCH ELEMENTS -------- */
      allElements: [],
      elementSearchPool: [],
      elementType: {}, // course can be (1) core, (2) useful
      elementSelectionOccurrences: {},
      courseData: [],

      /* -------- SEARCH FILTERS -------- */
      searchFilterGroups: [],
      searchFilterColors: {},
      searchFilterGroupColors: {},
      filterSubjectBy: '',

      /* -------- FLAGS AND INPUTS -------- */
      showSelf: false,
      bannerText: -1,
      searchInput: '',
      searchMatches: [],
    };
  },

  methods: {
    elementDragStart(event, element) {
      event.dataTransfer.effectAllowed = "move";
      this.$emit('elementDragStart', element);
    },

    incrementSearchPage() {
      //this.$refs.searchMatchList.scrollTop = 0;
      if ((this.searchDisplayContext + 1) * this.searchContextSize < this.searchMatches.length) {
        this.searchDisplayContext++;
      }
    },
    decrementSearchPage() {
      if (this.searchDisplayContext > 0) {
        this.searchDisplayContext--;
      }
    },
    getElementOccurrences(element) {
      if (!(element in this.elementSelectionOccurrences)) {
        return
      }
      return this.elementSelectionOccurrences[element].join(', ')
    },
    sumAsciiValues(word) {
      let sum = 0;
      for (let i = 0; i < word.length; i++) {
        sum += word.charCodeAt(i); // Get ASCII value of character and add to sum
      }
      return sum;
    },
    getElementColorFront(element){
      const type = this.elementType[element];
      if (type == "core" || element.startsWith("csci 1200")) {
        return this.makeHSLColor(20, 85, 70)
      } 
      else if (type == "useful" || element.startsWith("csci 4220")) {
        return this.makeHSLColor(90, 75, 65)
      }
      return this.makeHSLColor(40, 55, 70)
    },
    getElementColorRear(element){
      if (element in this.elementSelectionOccurrences) {
        return this.makeHSLColor(90, 55, 75)
      }
      return this.makeHSLColor(40, 40, 80)
    },
    getFilterColor(filter) {
      const colors = this.searchFilterColors[filter];
      if (this.filterSubjectBy == filter) {
        return this.makeHSLBackground(colors[0] + 10, colors[1] + 30, colors[2] + 30)
      }
      
      return this.arrayToHSLBackground(colors)
    },
    getSubjectTitle(subject) {
      let color = this.searchFilterGroupColors[subject.toLowerCase()];
      if (color == null) {
        return
      }
      return this.makeHSLColor(color[0], color[1] + 10, color[2] + 50, color[3] + 0.4);
    },

    extractColorHSL(element) {
      if (element.includes("#")) {
        return element.split("#")[1].split(",")
      }
      return []
    },

    extractTitle(element) {
      return element.split('#')[0]
    },


    arrayToHSLBackground(array) {
      if (array == null) {
        return
      }
      if (array.length == 4) {
        return {
          backgroundColor: `hsla(${array[0]}, ${array[1]}%, ${array[2]}%, ${array[3]})`
        };
      }
      return {
        backgroundColor: `hsl(${array[0]}, ${array[1]}%, ${array[2]}%)`
      };
    },

    makeHSLColor(hue, saturation, lightness, alpha=-1) {
      if (alpha != -1) {
        return {
          color: `hsla(${hue}, ${saturation}%, ${lightness}%, ${alpha})`
        };
      }
      return {
        color: `hsl(${hue}, ${saturation}%, ${lightness}%)`
      };
    },

    makeHSLBackground(hue, saturation, lightness, alpha = -1) {
      if (alpha != -1) {
        return {
          backgroundColor: `hsla(${hue}, ${saturation}%, ${lightness}%, ${alpha})`
        };
      }
      return {
        backgroundColor: `hsl(${hue}, ${saturation}%, ${lightness}%)`
      };
    },

    generateElementColor(element, hue_offset = 0, saturation_offset = 0, lightness_offset = 0, alpha = 1, index = -1) {
      let colors = this.extractColorHSL(element);
      if (colors.length == 3) {
        return [(colors[0] + hue_offset) % 360, (colors[1] + saturation_offset), (colors[2] + lightness_offset), alpha]
      }
      if (index != -1) {
        return [(index * 360 + hue_offset) % 360, saturation_offset, lightness_offset, alpha]
      }
      else {
        return [hue_offset % 360, saturation_offset, lightness_offset, alpha]
      }
    },

    randColorText(text, offset, multiple = 1, saturation = 33, lightness = 70) {
      return {
        backgroundColor: `hsl(${((this.sumAsciiValues(text) + offset) % 360) * multiple}, ${saturation}, ${lightness})`
      };
    },

    outputResult(val) {
      // Increment the value and emit an event to notify the parent
      this.$emit('result', val);
    },

    onOpen(bannerText) {
      this.$nextTick(() => {
        this.$refs.searchBar.focus();
      });
      this.bannerText = bannerText;
    },

    onClose() {
      this.searchInput = '';
      this.searchDisplayContext = 0;
      this.searchMatches = this.elementSearchPool;
    },

    inputHandler() {
      this.debouncedsearch();
    },

    debouncedsearch: debounce(function () {
      this.searchDisplayContext = 0;
      this.search();
    }, 400),

    readData(coursename, attribute) {
      if (this.courseData == null) {
        return ''
      }
      const data = this.courseData[coursename];
      if (data == null || data[attribute] == null) {
        return '';
      }
      return data[attribute][0]
    },

    search(count = -1) {
      let input = this.searchInput;
      input = input.toLowerCase();
      if (count == -1) {
        this.searchMatches = this.elementSearchPool.filter(element => element.search_name.includes(input));
      } else {
        this.searchMatches = [];
        for (let i = 0; i < this.elementSearchPool.length && this.searchMatches.length < count; ++i) {
          if (this.elementSearchPool[i].search_name.includes(input)) {
            this.searchMatches.push(this.elementSearchPool[i]);
          }
        }
      }
    },

    selectEnter() {
      this.search(1);
      this.selectMatch(0);
      this.searchInput = '';
      this.search();
    },

    selectMatch(position) {
      position = (this.searchContextSize * this.searchDisplayContext) + position;
    if (this.searchMatches.length > 0 && position < this.searchMatches.length) {
        this.outputResult(this.searchMatches[position].display_name);
      }
    },

    filterElement(subject) {
      this.elementSearchPool = [];
      this.searchDisplayContext = 0;
      if (subject == '' || this.filterSubjectBy == subject) {
        this.filterSubjectBy = '';
        this.elementSearchPool = this.allElements;
      }
      else {
        this.filterSubjectBy = subject;
        subject = subject.toLowerCase();
        this.elementSearchPool = this.allElements.filter(course => course.search_name.startsWith(subject));
      }
      this.search();
    },

    async fetchElements() {
      const response = await axios.get('/api/dp/courses/false');
      let course_list = await response.data;
      for (let i = 0; i < course_list.length; ++i) {
        this.allElements.push({display_name: course_list[i], search_name: course_list[i].toLowerCase()});
        this.elementSearchPool.push({display_name: course_list[i], search_name: course_list[i].toLowerCase()});
      }
      this.searchMatches = this.elementSearchPool;
    },

    async fetchData() {
      const response = await axios.get('/api/dp/coursedetails');
      let course_data = await response.data;
      this.courseData = course_data;
      console.log(JSON.stringify(this.courseData));
    },

    async fetchSearchFilterGroups() {
      const response = await axios.get('/api/dp/subjectgroups');
      this.searchFilterGroups = await response.data;
    },

    computeSearchFilterColors() {
      for (let i = 0; i < this.searchFilterGroups.length; ++i) {
        let group = this.searchFilterGroups[i];
        this.searchFilterGroupColors[group.title.toLowerCase()] = this.generateElementColor(group.title, 0, 12, 70, 0.3, i / this.searchFilterGroups.length);
        for (let j = 0; j < group.elements.length; ++j) {
          this.searchFilterColors[group.elements[j]] = this.generateElementColor(group.title, j, 10 - j * 0.25, 70 - 5 + j * 0.2, 0.9, i / this.searchFilterGroups.length);
        }
      }
      this.$emit('setSubjectColors', this.searchFilterColors);
      this.$emit('setSubjectGroupColors', this.searchFilterGroupColors);
      console.log(`subjectColors ${JSON.stringify(this.searchFilterColors)}`);
      console.log(`searchFilterGroupColors ${JSON.stringify(this.searchFilterGroupColors)}`);
    }
  },
  async created() {
    await this.fetchElements();
    await this.fetchSearchFilterGroups();
    await this.fetchData();
    this.computeSearchFilterColors();
  },  
};
</script>

<style scoped>
.container {
  display: flex;
  flex-flow: column;
  height: 100%;
}
.search-input {
  width: 100%;
  font-size: 0.9vw;
  padding: 2px;
  margin-top: 2px;
  border-radius: 4px;
  border: none;
}

.results {
  position: absolute;
  top: 100%;
  left: 0;
  width: 334px;
  background-color: rgba(39, 43, 44, 0.75);
  list-style: none;
  padding: 8px;
  border-radius: 4px;
  border: 2px solid #565a5c;
  backdrop-filter: blur(4px);
}
.results h3 {
  font-size: 14px;
  margin: 2px;
  color:#b3bfc3;
}

.search-results {
  left: 0;
  width: 100%;
  height: 50vh;
  background-color: rgba(30, 32, 33, 0.7);
  list-style: none;
  margin-top: 2px;
  padding: 2px;
  border-radius: 4px;
  border: none;
  overflow-y:scroll;
}
.search-results::-webkit-scrollbar {
        width: 10px; /* Adjust width for vertical scrollbar */
        height: 4px; /* Adjust height for horizontal scrollbar */
}

.search-results::-webkit-scrollbar-track {
    background: #f1f1f100; /* Color of the track */
}

.search-results::-webkit-scrollbar-thumb {
    background: rgba(109, 119, 130, 0.5); /* Color of the scroll thumb */
    border-radius: 2px; /* Rounded corners on the scroll thumb */
}

.search-results::-webkit-scrollbar-thumb:hover {
    background: #555; /* Color of the scroll thumb when hovered */
}
.search-result {
  font-weight: 600;
  font-size: 1em;
  margin-bottom: 10px;
}
.search-result-body {
  font-weight: 450;
  font-size: 0.9em;
  margin-left: 10px;
}
.search-result-body2 {
  font-weight: 450;
  font-size: 0.9em;
  margin-left: 10px;
  color:#a4abad;
}
.search-result-body-selected {
  font-weight: 400;
  font-style: italic;
  font-size: 0.9em;
}
.search-result-description {
  font-size: 0.8em;
  color:#abb1b7;
}
.search-traversal-button {
  font-weight: 850;
  font-size: 13px;
  color: #b2babc;
}
.selected-tag {
  font-weight: 500;
  font-size: 11px;
  color: #9fa7b0;
}

.subject-filter {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 2px;
  margin-top: 4px;
  margin-bottom: 4px;
  position: relative;
}

.subject-group {
  font-weight: 700;
  font-size: 0.9vw;
  color: #141415;
  border-radius: 4px;
  padding: 4px;
  padding-top: 4px;
  padding-bottom: 2px;
}

.subject-buttons {
  border: none;
  border-radius: 4px;
  font-size: 0.85em;
  width: 22%;
  padding: 0px;
  font-weight: 600;
  margin-bottom: 0px;
  margin-top: 0px;
  margin-left: 2px;
  margin-right: 2px;
  color: #141415;
  background-color:#141415;
  transition: background-color 0.15s ease;
  text-align: center;
}
.subject-buttons:hover {
  background-color: rgb(80, 85, 87);
}

.suggestions {
  top: 100%;
  left: 0;
  width: 100%;
  max-height: 250px;
  background-color: #4c5458;
  list-style: none;
  margin-bottom: 2px;
  padding: 2px;
  border-radius: 4px;
  border: 2px solid #2d2e31;
  overflow-y: auto;
}

.search-results li {
  padding: 2px;
  font-size: 0.9vw;
  cursor: pointer;
}

.search-results li:hover {
  background-color: #27292b;
}

.no-results {
  padding: 2px;
  text-align: center;
}
</style>
