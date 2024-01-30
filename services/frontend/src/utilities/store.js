import { defineStore } from 'pinia';
//import { Colors, ColorPalette, Color } from '@/utilities/colorservice.js';

export const colorStore = defineStore('sharedStore', {
    state: () => ({
    // Your shared state properties here
        colorService: null,
    }),
    actions: {
    // Your methods or actions here
    },
});

export const schedulerStore = defineStore('sharedStore', {
    state: () => ({
        // Your shared state properties here
        
    }),
    actions: {
        // Your methods or actions here
    },
});

export const plannerStore = defineStore('sharedStore', {
    state: () => ({
        // Your shared state properties here
        
    }),
    actions: {
        // Your methods or actions here
    },
});
