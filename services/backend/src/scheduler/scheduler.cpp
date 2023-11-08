#include "Scheduler.h"

Scheduler::Scheduler() {  // Default constructor with initializer list
}

void Scheduler::populate(std::vector<std::vector<CourseInstance>> selectedCourses) {
    // selectedCourses is a 2D vector: 1st dimension organizes course instances by the course such that the same courses are grouped together

    std::vector<Schedule> schedules;

    for (const auto& courseGroup : selectedCourses) {
        // interates through each "course" the user has selected, with each course being a vector of sections selected of that course
        // (note that I put course in quotations since it is not technically a course object - just a list of sections!)
        std::vector<Schedule> newSchedules;

        for (auto& schedule : schedules) {
            for (const auto& courseInstance : courseGroup) {
                // this loop is what expands the size of schedule exponentially by multiplying its size by the number of sections
                // of each course!

                // check collision and decide if to keep going with this schedule
                Schedule scheduleCopy = schedule;
                scheduleCopy.addCourseInstance(courseInstance); // Add section to the schedule
                newSchedules.push_back(scheduleCopy); // Append new schedule to the list if it fulfills previous criteria
            }
        }

        schedules = newSchedules; // Assign the new schedules to the original list
    }
}

bool Scheduler::collidesWithSchedule(Schedule schedule, CourseInstance courseInstance) {
    for (const auto& existingCourseInstance : schedule.getCourseInstances()) {
        if (collides(existingCourseInstance, courseInstance)) {
            return true;
        }
    }
    return false;
}

bool Scheduler::collides(CourseInstance courseInstance1, CourseInstance courseInstance2) {

}
