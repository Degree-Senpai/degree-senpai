#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>
#include "scheduler.h"
#include "utilities.h"

using json = nlohmann::json;

/*
* SCHEDULER METHODS
*/

Scheduler::Scheduler() {  // Default constructor with initializer list
}


// IMPORTING AND CREATION OF COURSE INSTANCES

std::shared_ptr<CourseInstance> Scheduler::makeCourseInstanceFromDictionary(std::unordered_map<std::string, std::string> courseInstanceData) {
    std::string name = this->getName(courseInstanceData);
    int crn = this->getCRN(courseInstanceData);
    std::vector<int> linearTimeBlocks = this->getLinearTimeBlocks(courseInstanceData);
    std::cout << "made new course instance " << name << " (" << crn << ") " << recursiveVecToString(linearTimeBlocks) << std::endl;
    return std::make_shared<CourseInstance>(name, crn, linearTimeBlocks);
}

void Scheduler::importCourseInstanceDictionaries(std::vector<std::unordered_map<std::string, std::string>> courseInstances) {
    for (auto& courseInstanceData : courseInstances) {
        if (this->allCourseInstances.find(this->getCRN(courseInstanceData)) == this->allCourseInstances.end()) {
            std::shared_ptr<CourseInstance> courseInstance = this->makeCourseInstanceFromDictionary(courseInstanceData);
            this->allCourseInstances.insert({courseInstance->crn, courseInstance});
            std::cout << "added course instance " << courseInstance->name << " to scheduler" << std::endl;
        }
    }
}

std::vector<std::vector<std::vector<int>>> Scheduler::exportSchedulesAsDictionaries(std::vector<std::vector<Schedule>> schedules) {
    // replaces schedule objects with just the CRN
    std::vector<std::vector<std::vector<int>>> schedulesCRN(schedules.size());
    for (long unsigned int i = 0; i < schedules.size(); ++i) {
        for (long unsigned int j = 0; j < schedules[i].size(); ++j) {
            schedulesCRN[i].push_back(schedules[i][j].getCourseCRNs());
        }
    }
    return schedulesCRN;
}


// IMPORTING HELPER FUNCTIONS

int Scheduler::getCRN(std::unordered_map<std::string, std::string> courseInstanceData) {
    return stoi(courseInstanceData["crn"]);
}

std::string Scheduler::getName(std::unordered_map<std::string, std::string> courseInstanceData) {
    return courseInstanceData["name"];
}

std::vector<int> Scheduler::getLinearTimeBlocks(std::unordered_map<std::string, std::string> courseInstanceData) {
    return stringToIntVec(courseInstanceData["timeBlocks"]);
}


// POPULATE FUNCTIONS

std::string Scheduler::populateAndExport(std::string selectedCoursesJson, int max_collisions) {
    std::vector<std::vector<std::unordered_map<std::string, std::string>>> data = json::parse(selectedCoursesJson);
    std::vector<std::vector<std::vector<int>>> schedulesCRN = this->exportSchedulesAsDictionaries(this->populate(data, max_collisions));
    std::string schedulesCRNJson = json(schedulesCRN).dump();
    return schedulesCRNJson;
}

std::vector<std::vector<std::vector<int>>> Scheduler::populateAndExport(std::vector<std::vector<std::unordered_map<std::string, std::string>>> selectedCourses, int max_collisions) {
    return this->exportSchedulesAsDictionaries(this->populate(selectedCourses, max_collisions));
}

std::vector<std::vector<Schedule>> Scheduler::populate(std::vector<std::vector<std::unordered_map<std::string, std::string>>> selectedCourses, int max_collisions) {
    // this is a wrapper around populate that first imports non-existing course instances, and then calls populate with a newly built selectedCourses with just CRNs
    std::vector<std::vector<int>> selectedCoursesCRN(selectedCourses.size());
    for (long unsigned int i = 0; i < selectedCourses.size(); ++i) {
        this->importCourseInstanceDictionaries(selectedCourses[i]);
        for (long unsigned int j = 0; j < selectedCourses[i].size(); ++j) {
            selectedCoursesCRN[i].push_back(this->getCRN(selectedCourses[i][j]));
        }
    }
    return this->populate(selectedCoursesCRN, max_collisions);
}

std::vector<std::vector<Schedule>> Scheduler::populate(std::vector<std::vector<int>> selectedCourses, int max_collisions) {
    // selectedCourses is a 2D vector: 1st dimension organizes the CRN of course instances by the course the instance it belongs to
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<Schedule>> schedules; // binned by number of collisions
    schedules.push_back({Schedule()});

    for (const auto& courseGroup : selectedCourses) {
        // interates through each "course" the user has selected, with each course being a vector of sections selected for that course
        // (note that I put course in quotations since it is not technically a course object - just a list of sections!)
        std::vector<std::vector<Schedule>> newSchedules(max_collisions + 1);

        for (auto& scheduleBin : schedules) {
            for (auto& schedule : scheduleBin) {
                // iterate through all schedules - it doesn't matter which bin it is in, we just go through all of them!

                for (const auto& courseInstanceCRN : courseGroup) {
                    std::shared_ptr<CourseInstance> courseInstance = this->allCourseInstances[courseInstanceCRN];
                    // this loop expands the size of schedule exponentially by multiplying its size by the # of sections of each course!
                    bool collides = collidesWithSchedule(schedule, courseInstance);

                    // decide if to keep going with this schedule
                    if (schedule.collisions == max_collisions && collides) {
                        continue;
                    }

                    Schedule scheduleCopy = schedule;
                    if (collides) {
                        ++scheduleCopy.collisions;
                    }
                    scheduleCopy.addCourseInstance(courseInstance); // Add the course instance to the schedule
                    newSchedules[scheduleCopy.collisions].push_back(scheduleCopy);
                }
            }
        }

        schedules = newSchedules; // Assign the new schedules to the original list
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "\nSCHEDULER RUNTIME: " << duration.count() << " microseconds\n" << std::endl;
    return schedules;
}


// HELPERS FOR POPULATE

bool Scheduler::collidesWithSchedule(Schedule schedule, std::shared_ptr<CourseInstance> courseInstance) {
    for (const auto& existingCourseInstance : schedule.getCourseInstances()) {
        if (collides(existingCourseInstance, courseInstance)) {
            return true;
        }
    }
    return false;
}

bool Scheduler::collides(std::shared_ptr<CourseInstance> courseInstance1, std::shared_ptr<CourseInstance> courseInstance2) {
    // collision is considered true if the two time blocks touch each other (one ends when the other begins)
    // this reduces the runtime of the algorithm
    long unsigned int i = 0;
    long unsigned int j = 0;
    while (i != courseInstance1->linearTimeBlocks.size() && j != courseInstance2->linearTimeBlocks.size()) {
        if (courseInstance1->linearTimeBlocks[i] < courseInstance2->linearTimeBlocks[j]) {
            ++i;
            if (j & 1) {
                return true;
            }
        } else {
            ++j;
            if (i & 1) {
                return true;
            }
        }
    }
    return false;
}
