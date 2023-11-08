#include <iostream>
#include "Scheduler.h"

Scheduler::Scheduler() {  // Default constructor with initializer list
}

std::vector<Schedule> Scheduler::populate(std::vector<std::vector<std::shared_ptr<CourseInstance>>> selectedCourses, int max_collisions) {
    // selectedCourses is a 2D vector: 1st dimension organizes course instances by the course such that the same courses are grouped together

    std::vector<Schedule> schedules;
    schedules.push_back(Schedule());

    for (const auto& courseGroup : selectedCourses) {
        // interates through each "course" the user has selected, with each course being a vector of sections selected of that course
        // (note that I put course in quotations since it is not technically a course object - just a list of sections!)
        std::vector<Schedule> newSchedules;
        std::cout << "looking at coursegroup " << courseGroup[0]->name << std::endl;

        for (auto& schedule : schedules) {
            for (const auto& courseInstance : courseGroup) {
                // this loop is what expands the size of schedule exponentially by multiplying its size by the number of sections
                // of each course!
                bool collides = collidesWithSchedule(schedule, courseInstance);
                if (schedule.collisions == max_collisions && collides) {
                    continue;
                }

                // check collision and decide if to keep going with this schedule
                Schedule scheduleCopy = schedule;
                if (collides) {
                    ++scheduleCopy.collisions;
                }
                scheduleCopy.addCourseInstance(courseInstance); // Add section to the schedule
                newSchedules.push_back(scheduleCopy); // Append new schedule to the list if it fulfills previous criteria
            }
        }

        schedules = newSchedules; // Assign the new schedules to the original list
    }
    std::cout << "final computed schedules: \n"; 
    for (const auto& schedule : schedules) {
        std::cout << "  " << schedule << "\n";
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
