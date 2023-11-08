#include "Course.h"

Schedule::Schedule() {
    this->collisions = 0;
}

void Schedule::addCourseInstance(std::shared_ptr<CourseInstance> courseInstance) {
    courseInstances.push_back(courseInstance);
}

std::vector<std::shared_ptr<CourseInstance>> Schedule::getCourseInstances() {
    return courseInstances;
}



CourseInstance::CourseInstance(std::string name, int crn, std::vector<std::shared_ptr<TimeBlock>> timeBlocks) {
    this->name = name;
    this->crn = crn;
    this->timeBlocks = timeBlocks;
}

CourseInstance::CourseInstance(std::string name, int crn, std::vector<int> linearTimeBlocks) {
    this->name = name;
    this->crn = crn;
    this->linearTimeBlocks = linearTimeBlocks;
}



TimeBlock::TimeBlock(int day, int begin, int end) {
    this->day = day;
    this->begin = begin;
    this->end = end;
}



std::ostream& operator<<(std::ostream& os, const CourseInstance& s) {
    os << "CourseInstance " << s.name << " (" << s.crn << ") with time blocks: ";
    for (const auto& timeBlock : s.timeBlocks) {
        os << *timeBlock;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TimeBlock& s) {
    os << "TimeBlock - day: " << s.day << " time: " << s.begin << " to " << s.end << " ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Schedule& s) {
    os << "Schedule with " << s.collisions << " collisions, course instances: \n";
    for (const auto& courseInstance : s.courseInstances) {
        os << "    " << *courseInstance << "\n";
    }
    return os;
}
