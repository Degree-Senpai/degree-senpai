#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <string>
#include "scheduler.h"

std::string populate(std::string selectedCourses, int maxDepth) {
    Scheduler scheduler;
    return scheduler.populateAndExport(selectedCourses, maxDepth);
}

int test(int num) {
    return num + num;
}

EMSCRIPTEN_BINDINGS(scheduler) {
    emscripten::function("test", &test);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("populate", &populate);
}
