#include "Course.h"

Schedule::Schedule() {
    collisions = 0;
}

void Schedule::addCourseInstance(CourseInstance courseInstance) {
    courseInstances.push_back(courseInstance);
}

std::vector<CourseInstance> Schedule::getCourseInstances() {
    return courseInstances;
}
