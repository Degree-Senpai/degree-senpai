#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <unordered_map>
#include "schedule.h"

class Scheduler {
    public:
        // the Scheduler holds course instance data as of now but this may be changed in the future :)
        Scheduler();
        ~Scheduler();

        CourseInstance* makeCourseInstanceFromDictionary(std::unordered_map<std::string, std::string> courseInstanceData);
        
        void importCourseInstanceDictionary(const std::unordered_map<std::string, std::string> courseInstanceData, bool reimport=false);
        void importCourseInstanceDictionaries(const std::vector<std::unordered_map<std::string, std::string>> courseInstances, bool reimport=false);

        std::vector<std::vector<std::vector<int>>> exportSchedulesAsVectors(std::vector<std::vector<Schedule>> schedules);

        // main function to compute schedules!
        std::string populateAndExport(std::string, int max_collisions, bool reimport=false);
        std::vector<std::vector<Schedule>> importAndPopulate(std::vector<std::vector<std::unordered_map<std::string, std::string>>>, int, bool reimport=false);
        std::vector<std::vector<Schedule>> populate(std::vector<std::vector<int>>, int);

        // helper functions
        bool collidesWithSchedule(Schedule, CourseInstance*);
        bool collides(CourseInstance*, CourseInstance*);

        int getCRN(std::unordered_map<std::string, std::string>);
        std::string getName(std::unordered_map<std::string, std::string>);
        std::vector<int> getLinearTimeBlocks(std::unordered_map<std::string, std::string>);

        std::unordered_map<int, CourseInstance*> allCourseInstances; // maps crn to all course instances
        std::unordered_map<std::string, std::vector<int>> courseInstancesOfCourse; // maps course to its course instances (refered to by crn)
};

#endif
