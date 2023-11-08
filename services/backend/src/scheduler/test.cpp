#include "scheduler.h"

int main() {
    Scheduler scheduler;

    // generate time blocks
    std::vector<std::shared_ptr<TimeBlock>> datastructures1timeblocks;
    datastructures1timeblocks.push_back(std::make_shared<TimeBlock>(1, 720, 830));
    datastructures1timeblocks.push_back(std::make_shared<TimeBlock>(4, 720, 830));

    std::vector<int> datastructures1timeblocksnew = {2280, 2390, 6600, 6710};
    // make course instance
    std::shared_ptr<CourseInstance> datastructures1 = std::make_shared<CourseInstance>("data structures", 20001, datastructures1timeblocksnew);

    // generate time blocks
    std::vector<std::shared_ptr<TimeBlock>> cs1timeblocks;
    cs1timeblocks.push_back(std::make_shared<TimeBlock>(0, 660, 770));
    cs1timeblocks.push_back(std::make_shared<TimeBlock>(3, 660, 770));

    std::vector<int> cs1timeblocksnew = {840, 950, 5160, 5270};
    // make course instance
    std::shared_ptr<CourseInstance> cs1 = std::make_shared<CourseInstance>("computer science", 10001, cs1timeblocksnew);

    // generate time blocks
    std::vector<std::shared_ptr<TimeBlock>> cs2timeblocks;
    cs2timeblocks.push_back(std::make_shared<TimeBlock>(1, 660, 770));
    cs2timeblocks.push_back(std::make_shared<TimeBlock>(4, 660, 770));

    std::vector<int> cs2timeblocksnew = {2280, 2390, 6600, 6710};
    // make course instance
    std::shared_ptr<CourseInstance> cs2 = std::make_shared<CourseInstance>("computer science", 10002, cs2timeblocksnew);

    std::vector<std::vector<std::shared_ptr<CourseInstance>>> courseInstances;

    std::vector<std::shared_ptr<CourseInstance>> datastructures;
    datastructures.push_back(datastructures1);

    std::vector<std::shared_ptr<CourseInstance>> cs;
    cs.push_back(cs1);
    cs.push_back(cs2);

    courseInstances.push_back(datastructures);
    courseInstances.push_back(cs);

    scheduler.populate(courseInstances, 5);
}
