#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

class CourseInstance;
class TimeBlock;

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
        CourseInstance(std::string name, int crn, std::vector<std::shared_ptr<TimeBlock>> timeBlocks);
        CourseInstance(std::string name, int crn, std::vector<int> linearTimeBlocks);

        std::vector<std::shared_ptr<TimeBlock>> timeBlocks;
        std::vector<int> linearTimeBlocks;

        std::string name;
        std::string id;
        std::string professor;
        std::string location;
        int crn;

        friend std::ostream& operator<<(std::ostream&, const CourseInstance&);
};

class TimeBlock {
    public:
        TimeBlock(int beginSinceEpoch, int endSinceEpoch);
        TimeBlock(int day, int begin, int end);

        int day;
        int begin;
        int end;

        int computedBeginHour;
        int computedBeginMinute;
        int computedEndHour;
        int computedEndMinute;

        friend std::ostream& operator<<(std::ostream&, const TimeBlock&);
};

#endif
