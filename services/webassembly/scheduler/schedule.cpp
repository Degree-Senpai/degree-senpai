#include "schedule.h"

Schedule::Schedule() {
    this->collisions = 0;
}

std::vector<int> Schedule::getCourseCRNs() {
    std::vector<int> crns;
    for (auto& courseInstance : this->courseInstances) {
        crns.push_back(courseInstance->crn);
    }
    return crns;
}

std::ostream& operator<<(std::ostream& os, const Schedule& s) {
    os << "Schedule with " << s.collisions << " collisions, course instances: \n";
    for (const auto& courseInstance : s.courseInstances) {
        os << "    " << *courseInstance << "\n";
    }
    return os;
}
