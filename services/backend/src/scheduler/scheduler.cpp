#include <iostream>
#include <chrono>
#include "Scheduler.h"

Scheduler::Scheduler() {  // Default constructor with initializer list
}

std::vector<std::vector<Schedule>> Scheduler::populate(std::vector<std::vector<std::shared_ptr<CourseInstance>>> selectedCourses, int max_collisions) {
    // selectedCourses is a 2D vector: 1st dimension organizes course instances by the course the instance it belongs to

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<Schedule>> schedules; // binned by number of collisions
    schedules.push_back({Schedule()});

    for (const auto& courseGroup : selectedCourses) {
        // interates through each "course" the user has selected, with each course being a vector of sections selected for that course
        // (note that I put course in quotations since it is not technically a course object - just a list of sections!)
        std::vector<std::vector<Schedule>> newSchedules(max_collisions);
        std::cout << "looking at coursegroup " << courseGroup[0]->name << std::endl;

        for (auto& scheduleBin : schedules) {
            for (auto& schedule : scheduleBin) {
                // iterate through all schedules - it doesn't matter which bin it is in, we just go through all of them!

                for (const auto& courseInstance : courseGroup) {
                    // this loop expands the size of schedule exponentially by multiplying its size by the # of sections of each course!
                    bool collides = collidesWithSchedule(schedule, courseInstance);
                    if (schedule.collisions == max_collisions && collides) {
                        continue;
                    }

                    // check collision and decide if to keep going with this schedule
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

    std::cout << "final computed schedules: \n"; 
    for (const auto& scheduleBin : schedules) {
        for (const auto& schedule : scheduleBin) {
            std::cout << "  " << schedule << "\n";
        }
    }
    std::cout << std::endl;
    return schedules;
}

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
