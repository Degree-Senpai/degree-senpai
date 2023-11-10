#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <unordered_map>
#include "schedule.h"

class Scheduler {
    public:
        // the Scheduler holds course instance data as of now but this may be changed in the future :)
        Scheduler();

        std::shared_ptr<CourseInstance> makeCourseInstanceFromDictionary(std::unordered_map<std::string, std::string> courseInstanceData);
        
        void importCourseInstanceDictionary(const std::unordered_map<std::string, std::string> courseInstanceData);
        void importCourseInstanceDictionaries(const std::vector<std::unordered_map<std::string, std::string>> courseInstances);

        std::vector<std::vector<std::vector<int>>> exportSchedulesAsVectors(std::vector<std::vector<Schedule>> schedules);

        // main function to compute schedules!
        std::string populateAndExport(std::string, int max_collisions);
        std::vector<std::vector<std::vector<int>>> populateAndExport(std::vector<std::vector<std::unordered_map<std::string, std::string>>> selectedCourses, int max_collisions);
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
