compile commands: 

g++ -I/mnt/c/Users/eggyr/ERULN/C/json/include -std=c++17 test.cpp scheduler.cpp course.cpp utilities.cpp  -Wall -o test.out

emcc --bind -I/mnt/c/Users/eggyr/ERULN/C/json/include -std=c++17 scheduler.cpp course.cpp utilities.cpp schedulerAPI.cpp -Wall -o scheduler.js