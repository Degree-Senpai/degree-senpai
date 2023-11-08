#include "course.h"

Schedule::Schedule() {
    this->collisions = 0;
}

void Schedule::addCourseInstance(std::shared_ptr<CourseInstance> courseInstance) {
    courseInstances.push_back(courseInstance);
}

std::vector<std::shared_ptr<CourseInstance>> Schedule::getCourseInstances() {
    return courseInstances;
}

std::vector<int> Schedule::getCourseCRNs() {
    std::vector<int> crns;
    for (auto& courseInstance : this->courseInstances) {
        crns.push_back(courseInstance->crn);
    }
    return crns;
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
    this->begin = begin + day * 1440;
    this->end = end + day * 1440;

    this->computedBeginHour = (this->begin % 1440) / 60;
    this->computedBeginMinute = this->begin % 60;
    this->computedEndHour = (this->end % 1440) / 60;
    this->computedEndMinute = this->end % 60;
}

TimeBlock::TimeBlock(int beginSinceEpoch, int endSinceEpoch) {
    this->begin = beginSinceEpoch;
    this->end = endSinceEpoch;

    this->day = this->begin / 1440;
    this->computedBeginHour = (this->begin % 1440) / 60;
    this->computedBeginMinute = this->begin % 60;
    this->computedEndHour = (this->end % 1440) / 60;
    this->computedEndMinute = this->end % 60;
}



std::ostream& operator<<(std::ostream& os, const CourseInstance& s) {
    os << "CourseInstance " << s.name << " (" << s.crn << ") with time blocks: ";
    for (long unsigned int i = 0; i < s.linearTimeBlocks.size(); i += 2) {
        os << TimeBlock(s.linearTimeBlocks[i], s.linearTimeBlocks[i + 1]);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TimeBlock& s) {
    os << "TimeBlock - day: " << s.day << " time: " << s.computedBeginHour << ":" << s.computedBeginMinute
        << " to " << s.computedEndHour << ":" << s.computedEndMinute << " ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Schedule& s) {
    os << "Schedule with " << s.collisions << " collisions, course instances: \n";
    for (const auto& courseInstance : s.courseInstances) {
        os << "    " << *courseInstance << "\n";
    }
    return os;
}
