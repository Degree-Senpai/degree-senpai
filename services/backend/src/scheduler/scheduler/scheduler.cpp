#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>
#include "scheduler.h"
#include "../utilities/utilities.h"

using json = nlohmann::json;

/*
* The scheduler object will create CourseInstance objects when populate is called and there are new courses it previously hasn't stored.
* The scheduler will delete all CourseInstance objects it created upon destruction.
*/

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {
    for (auto& courseInstance: this->allCourseInstances) {
        delete courseInstance.second;
    }
}


// IMPORTING AND CREATION OF COURSE INSTANCES

/* Generates and returns CourseInstance from an ordered_map with strings as elements */
CourseInstance* Scheduler::makeCourseInstanceFromDictionary(std::unordered_map<std::string, std::string> courseInstanceData) {
    std::string name = this->getName(courseInstanceData);
    int crn = this->getCRN(courseInstanceData);
    std::vector<int> linearTimeBlocks = this->getLinearTimeBlocks(courseInstanceData);
    CourseInstance* courseInstance = new CourseInstance(name, crn, linearTimeBlocks);
    return courseInstance;
}

/* Wrapper for importCourseInstanceDictionary to import multiple CourseInstances at once */
void Scheduler::importCourseInstanceDictionaries(std::vector<std::unordered_map<std::string, std::string>> courseInstances, bool reimport) {
    for (auto& courseInstanceData : courseInstances) {
        this->importCourseInstanceDictionary(courseInstanceData, reimport);
    }
}

/* If a CourseInstance with this CRN has not been added, a new CourseInstance will be created and added to Scheduler */
void Scheduler::importCourseInstanceDictionary(std::unordered_map<std::string, std::string> courseInstanceData, bool reimport) {
    if (this->allCourseInstances.find(this->getCRN(courseInstanceData)) == this->allCourseInstances.end()) {
        CourseInstance* courseInstance = this->makeCourseInstanceFromDictionary(courseInstanceData);
        this->allCourseInstances[courseInstance->crn] = courseInstance;
        //std::cout << "added course instance " << courseInstance->name << " to scheduler" << std::endl;
    } else if (reimport) {
        CourseInstance* courseInstance = this->makeCourseInstanceFromDictionary(courseInstanceData);
        delete this->allCourseInstances[courseInstance->crn];
        this->allCourseInstances[courseInstance->crn] = courseInstance;
    }
}

/* Replaces the 2D vector of schedules generated by populate with a 3D vector of CRNs */
std::vector<std::vector<std::vector<int>>> Scheduler::exportSchedulesAsVectors(std::vector<std::vector<Schedule>> schedules) {
    std::vector<std::vector<std::vector<int>>> schedulesCRN(schedules.size());
    for (long unsigned int i = 0; i < schedules.size(); ++i) {
        for (long unsigned int j = 0; j < schedules[i].size(); ++j) {
            schedulesCRN[i].push_back(schedules[i][j].getCourseCRNs());
        }
    }
    return schedulesCRN;
}


// IMPORTING FROM DICTIONARY HELPER FUNCTIONS

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

/* Wrapper for populate that takes in JSON string of 2D vector of course dictionaries ({string, string}) and returns JSON string of 3D vector of CRNs (int) */
std::string Scheduler::populateAndExport(std::string selectedCoursesJson, int max_collisions, bool reimport) {
    std::vector<std::vector<std::unordered_map<std::string, std::string>>> data = json::parse(selectedCoursesJson);
    std::vector<std::vector<std::vector<int>>> schedulesCRN = this->exportSchedulesAsVectors(this->importAndPopulate(data, max_collisions, reimport));
    std::string schedulesCRNJson = json(schedulesCRN).dump();
    return schedulesCRNJson;
}

/* Wrapper for populate that takes in 2D vector of course dictionaries ({string, string}) and returns 2D vector of Schedules */
std::vector<std::vector<Schedule>> Scheduler::importAndPopulate(std::vector<std::vector<std::unordered_map<std::string, std::string>>> selectedCourses, int max_collisions, bool reimport) {
    // this is a wrapper around populate that first imports non-existing course instances, and then calls populate with a newly built selectedCourses with just CRNs
    std::vector<std::vector<int>> selectedCoursesCRN(selectedCourses.size());
    for (long unsigned int i = 0; i < selectedCourses.size(); ++i) {
        this->importCourseInstanceDictionaries(selectedCourses[i], reimport);
        for (long unsigned int j = 0; j < selectedCourses[i].size(); ++j) {
            selectedCoursesCRN[i].push_back(this->getCRN(selectedCourses[i][j]));
        }
    }
    return this->populate(selectedCoursesCRN, max_collisions);
}

/* The algorithm that computes, sorted by bins, the schedules with the same number of course collisions as the bin index.
By capping max_collisions, the algorithm automatically throws away schedules with too many collisions, improving efficiency.

This function takes in a 2D vector of CRNs (requires courses to be imported already) and returns 2D vector of Schedules.

selectedCourses: 1st dimension organizes the CRN of course instances by the course the instance it belongs to */
std::vector<std::vector<Schedule>> Scheduler::populate(std::vector<std::vector<int>> selectedCourses, int max_collisions) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<Schedule>> schedules; // binned by number of collisions
    schedules.push_back({Schedule()}); // initialize the algorithm with an empty schedule

    for (const auto& courseGroup : selectedCourses) {
        // interates through each "course" the user has selected, with each course being a vector of instances selected for that course
        std::vector<std::vector<Schedule>> newSchedules(max_collisions + 1);

        for (auto& scheduleBin : schedules) {
            for (auto& schedule : scheduleBin) {
                // iterate through all schedules - it doesn't matter which bin it is in, we just go through all of them!

                for (const auto& courseInstanceCRN : courseGroup) {
                    // this loop expands the size of schedule exponentially by multiplying its size by the # of sections of each course!
                    CourseInstance* courseInstance = this->allCourseInstances[courseInstanceCRN];
                    bool collides = collidesWithSchedule(schedule, courseInstance);

                    // decide if to keep going with this schedule
                    if (schedule.collisions == max_collisions && collides) {
                        continue;
                    }

                    Schedule scheduleCopy = schedule;
                    if (collides) {
                        ++scheduleCopy.collisions;
                    }
                    scheduleCopy.courseInstances.push_back(courseInstance); // Add the course instance to the schedule
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

/* returns true if the course has any collision with any of the courses with the schedule */
bool Scheduler::collidesWithSchedule(Schedule schedule, CourseInstance* courseInstance) {
    for (const auto& existingCourseInstance : schedule.courseInstances) {
        if (collides(existingCourseInstance, courseInstance)) {
            return true;
        }
    }
    return false;
}

/* returns whether two courses have any conflict. Runs in O(N + M) time where N and M are the number of timeblocks for each course, respectively */
bool Scheduler::collides(CourseInstance* courseInstance1, CourseInstance* courseInstance2) {
    long unsigned int i = 0;
    long unsigned int j = 0;
    while (i != courseInstance1->linearTimeBlocks.size() && j != courseInstance2->linearTimeBlocks.size()) {
        if (courseInstance1->linearTimeBlocks[i] < courseInstance2->linearTimeBlocks[j]) {
            ++i;
            if (j & 1) {
                return true;
            }
        } else if (courseInstance1->linearTimeBlocks[i] == courseInstance2->linearTimeBlocks[j]) {
            ++i;
            ++j;
            if ((i & 1) == (j & 1)) {
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
