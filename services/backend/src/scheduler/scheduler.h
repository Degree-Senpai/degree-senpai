#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "course.h"
#include <vector>

class Scheduler {
    public:
        Scheduler();
        void populate(std::vector<std::vector<CourseInstance>>);
        bool collidesWithSchedule(Schedule, CourseInstance);
        bool collides(CourseInstance, CourseInstance);
};

#endif