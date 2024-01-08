#include <emscripten/bind.h>
#include <string>
#include "scheduler/scheduler.h"

// NOTE: setting this here maintains imported courses in memory, improving performance
Scheduler scheduler;

std::string populate(std::string selectedCourses, int maxDepth, bool reimport) {
    // use reimport = true if a course needs to be updated, since for performance reasons
    // we assume that the same CRN gives the same course information.
    return scheduler.populateAndExport(selectedCourses, maxDepth, reimport);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("populate", &populate);
}

/*
EMSCRIPTEN_BINDINGS(external_constructors) {
  class_<Scheduler>("Scheduler")
    .constructor()
    .function("someFunction", &MyClass::someFunction)
    ;
}
*/
