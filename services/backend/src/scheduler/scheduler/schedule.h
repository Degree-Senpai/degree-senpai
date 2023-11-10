#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "course.h"

class Schedule {
    public:
        Schedule();
        std::vector<int> getCourseCRNs();
        friend std::ostream& operator<<(std::ostream&, const Schedule&);

        std::vector<std::shared_ptr<CourseInstance>> courseInstances;
        int collisions;
};

#endif
