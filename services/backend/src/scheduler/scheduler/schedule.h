#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "../common/course.h"

class Schedule {
    public:
        Schedule();
        std::vector<int> getCourseCRNs();
        friend std::ostream& operator<<(std::ostream&, const Schedule&);

        std::vector<CourseInstance*> courseInstances;
        int collisions;
};

#endif
