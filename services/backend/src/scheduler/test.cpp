#include "scheduler.h"

int main() {
    Scheduler scheduler;

    // generate time blocks
    std::vector<std::shared_ptr<TimeBlock>> datastructures1timeblocks;
    datastructures1timeblocks.push_back(std::make_shared<TimeBlock>(1, 720, 830));
    datastructures1timeblocks.push_back(std::make_shared<TimeBlock>(4, 720, 830));

    std::shared_ptr<CourseInstance> datastructures1 = std::make_shared<CourseInstance>("data structures", 20001, std::vector<int>{2280, 2390, 6600, 6710});

    std::shared_ptr<CourseInstance> cs1 = std::make_shared<CourseInstance>("computer science", 10001, std::vector<int>{840, 950, 5160, 5270});
    std::shared_ptr<CourseInstance> cs2 = std::make_shared<CourseInstance>("computer science", 10002, std::vector<int>{2280, 2390, 6600, 6710});

    std::shared_ptr<CourseInstance> animation1 = std::make_shared<CourseInstance>("3D animation", 41001, std::vector<int>{840, 950, 5160, 5270});
    std::shared_ptr<CourseInstance> animation2 = std::make_shared<CourseInstance>("3D animation", 41002, std::vector<int>{960, 1070, 5280, 5390});
    std::shared_ptr<CourseInstance> animation3 = std::make_shared<CourseInstance>("3D animation", 41003, std::vector<int>{2280, 2390, 6600, 6710});
    std::shared_ptr<CourseInstance> animation4 = std::make_shared<CourseInstance>("3D animation", 41004, std::vector<int>{2340, 2390, 6660, 6710});

    std::shared_ptr<CourseInstance> graphic0 = std::make_shared<CourseInstance>("Graphics Storytelling", 42000, std::vector<int>{480, 590, 4800, 4910});
    std::shared_ptr<CourseInstance> graphic1 = std::make_shared<CourseInstance>("Graphics Storytelling", 42001, std::vector<int>{840, 950, 5160, 5270});
    std::shared_ptr<CourseInstance> graphic2 = std::make_shared<CourseInstance>("Graphics Storytelling", 42002, std::vector<int>{960, 1070, 5280, 5390});
    std::shared_ptr<CourseInstance> graphic3 = std::make_shared<CourseInstance>("Graphics Storytelling", 42003, std::vector<int>{2280, 2390, 6600, 6710});
    std::shared_ptr<CourseInstance> graphic4 = std::make_shared<CourseInstance>("Graphics Storytelling", 42004, std::vector<int>{2340, 2390, 6660, 6710});

    std::vector<std::vector<std::shared_ptr<CourseInstance>>> courseInstances = {
        {datastructures1},
        {cs1, cs2},
        {animation1, animation2, animation3, animation4},
        {graphic0, graphic1, graphic2, graphic3, graphic4}
    };

    scheduler.populate(courseInstances, 5);
}
