#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "course.h"

class Scheduler {
    public:
        Scheduler();
        std::vector<std::vector<Schedule>> populate(std::vector<std::vector<std::shared_ptr<CourseInstance>>>, int);
        bool collidesWithSchedule(Schedule, std::shared_ptr<CourseInstance>);
        bool collides(std::shared_ptr<CourseInstance>, std::shared_ptr<CourseInstance>);
};

#endif