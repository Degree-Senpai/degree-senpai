compile commands: 

g++ -I/mnt/c/Users/eggyr/ERULN/C/json/include -std=c++17 test.cpp scheduler.cpp course.cpp utilities.cpp  -Wall -o test.out
emcc --bind -I/mnt/c/Users/eggyr/ERULN/C/json/include -std=c++17 scheduler.cpp course.cpp utilities.cpp schedulerAPI.cpp -Wall -o scheduler.js -s EXPORTED_RUNTIME_METHODS='["cwrap", "getValue", "setValue"]'


How I did webassembly:

so I created the C++ files as usual, and inside the cpp file that I want to expose a function, I included this thing:

    EMSCRIPTEN_BINDINGS(my_module) {
        emscripten::function("populate", &populate);
    }

to expose a function (in this case "populate")

XXXX OLD XXXXXI then ran the compile command: 
    emcc --bind -I/mnt/c/Users/eggyr/ERULN/C/json/include -std=c++17 scheduler.cpp course.cpp utilities.cpp schedulerAPI.cpp -Wall -o scheduler.js -s EXPORTED_RUNTIME_METHODS='["cwrap", "getValue", "setValue"]'

cmake is used to generate the output. cmake is controlled by the cmakelists.txt file, where I can specify all the stuff like linking libraries, my cpp files, other arguments etc. A directory called build
is created and the following commands are executed to run cmake:

    emcmake cmake ..
    cmake --build .

which gives me scheduler.js and scheduler.wasm. I dragged these into the public folder of the frontend, and inside the vue file that I want to use wasm, I wrote the following inside mounted:

    if (typeof Module == 'undefined') {
        const script = document.createElement('script');
        script.src = './scheduler.js';
        script.onload = () => {
        console.log("emscripten module loaded");
        }
        document.body.appendChild(script);
    }

and when I want to access the C++ function, I do the following:

    let result = Module.populate(data, 5);

basically just treating the C++ function as if it was a member of the Module.
