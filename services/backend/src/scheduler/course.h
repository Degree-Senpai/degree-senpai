#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <string>
#include <memory>

class Course {
    public:
        Course();

        std::string name;
        std::string id; // this just needs to be unique, preferably just the name but casefold
        std::string description;
        std::vector<std::shared_ptr<CourseInstance>> courseInstances;
};

class CourseInstance {
    public:
        CourseInstance();     
};

class TimeBlock {
    public:
        TimeBlock();

        int day;
        int begin;
        int end;

        // computed for sake of performance
        int length;
        int absoluteBegin;
        int absoluteEnd;
};

class Schedule {
    private:
        std::vector<CourseInstance> courseInstances;

    public:
        Schedule();

        void addCourseInstance(CourseInstance);
        std::vector<CourseInstance> getCourseInstances(void);

        int collisions;
};

#endif