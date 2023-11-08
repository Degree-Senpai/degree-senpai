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

    std::vector<int> animation1tb = {840, 950, 5160, 5270};
    std::shared_ptr<CourseInstance> animation1 = std::make_shared<CourseInstance>("3D animation", 41001, animation1tb);
    std::vector<int> animation2tb = {960, 1070, 5280, 5390};
    std::shared_ptr<CourseInstance> animation2 = std::make_shared<CourseInstance>("3D animation", 41002, animation2tb);
    std::vector<int> animation3tb = {2280, 2390, 6600, 6710};
    std::shared_ptr<CourseInstance> animation3 = std::make_shared<CourseInstance>("3D animation", 41003, animation3tb);
    std::vector<int> animation4tb = {2340, 2390, 6660, 6710};
    std::shared_ptr<CourseInstance> animation4 = std::make_shared<CourseInstance>("3D animation", 41004, animation4tb);

    std::vector<int> graphic0tb = {480, 590, 4800, 4910};
    std::shared_ptr<CourseInstance> graphic0 = std::make_shared<CourseInstance>("Graphics Storytelling", 42000, graphic0tb);
    std::vector<int> graphic1tb = {840, 950, 5160, 5270};
    std::shared_ptr<CourseInstance> graphic1 = std::make_shared<CourseInstance>("Graphics Storytelling", 42001, graphic1tb);
    std::vector<int> graphic2tb = {960, 1070, 5280, 5390};
    std::shared_ptr<CourseInstance> graphic2 = std::make_shared<CourseInstance>("Graphics Storytelling", 42002, graphic2tb);
    std::vector<int> graphic3tb = {2280, 2390, 6600, 6710};
    std::shared_ptr<CourseInstance> graphic3 = std::make_shared<CourseInstance>("Graphics Storytelling", 42003, graphic3tb);
    std::vector<int> graphic4tb = {2340, 2390, 6660, 6710};
    std::shared_ptr<CourseInstance> graphic4 = std::make_shared<CourseInstance>("Graphics Storytelling", 42004, graphic4tb);

    std::vector<std::vector<std::shared_ptr<CourseInstance>>> courseInstances;

    std::vector<std::shared_ptr<CourseInstance>> datastructures;
    datastructures.push_back(datastructures1);

    std::vector<std::shared_ptr<CourseInstance>> cs;
    cs.push_back(cs1);
    cs.push_back(cs2);

    std::vector<std::shared_ptr<CourseInstance>> animation;
    animation.push_back(animation1);
    animation.push_back(animation2);
    animation.push_back(animation3);
    animation.push_back(animation4);

    std::vector<std::shared_ptr<CourseInstance>> graphic;
    graphic.push_back(graphic0);
    graphic.push_back(graphic1);
    graphic.push_back(graphic2);
    graphic.push_back(graphic3);
    graphic.push_back(graphic4);

    courseInstances.push_back(datastructures);
    courseInstances.push_back(cs);
    courseInstances.push_back(animation);
    courseInstances.push_back(graphic);

    scheduler.populate(courseInstances, 1);
}
