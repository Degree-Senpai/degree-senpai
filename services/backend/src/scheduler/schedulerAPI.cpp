#include <emscripten/bind.h>
#include <string>
#include "scheduler/scheduler.h"

std::string populate(std::string selectedCourses, int maxDepth, bool reimport) {
    Scheduler scheduler;
    return scheduler.populateAndExport(selectedCourses, maxDepth, reimport);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("populate", &populate);
}

EMSCRIPTEN_BINDINGS(external_constructors) {
  class_<Scheduler>("Scheduler")
    .constructor()
    .function("someFunction", &MyClass::someFunction)
    ;
}
