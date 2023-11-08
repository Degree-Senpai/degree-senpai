#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <unordered_map>
//#include <emscripten/val.h>
#include "course.h"

class Scheduler {
    public:
        Scheduler();

        std::shared_ptr<CourseInstance> makeCourseInstance(std::unordered_map<std::string, std::string> courseInstanceData);
        void importCourseInstances(const std::vector<std::unordered_map<std::string, std::string>>);

        // main function to compute schedules!
        std::vector<std::vector<Schedule>> populate(std::vector<std::unordered_map<std::string, std::string>>, int);
        std::vector<std::vector<Schedule>> populate(std::vector<int>, int);
        std::vector<std::vector<Schedule>> populate(std::vector<std::vector<std::unordered_map<std::string, std::string>>>, int);

        std::vector<std::vector<Schedule>> populate(std::vector<std::vector<int>>, int);

        // helper functions
        bool collidesWithSchedule(Schedule, std::shared_ptr<CourseInstance>);
        bool collides(std::shared_ptr<CourseInstance>, std::shared_ptr<CourseInstance>);

        int getCRN(std::unordered_map<std::string, std::string>);
        std::string getName(std::unordered_map<std::string, std::string>);
        std::vector<int> getLinearTimeBlocks(std::unordered_map<std::string, std::string>);

        std::unordered_map<int, std::shared_ptr<CourseInstance>> allCourseInstances; // maps crn to all course instances
        std::unordered_map<std::string, std::vector<int>> courseInstancesOfCourse; // maps course to its course instances (refered to by crn)
};

#endif
