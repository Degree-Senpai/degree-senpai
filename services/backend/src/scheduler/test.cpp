#include "scheduler.h"

int main() {
    Scheduler scheduler;

    // generate time blocks

    /*
    std::vector<std::shared_ptr<TimeBlock>> datastructures1timeblocks;
    datastructures1timeblocks.push_back(std::make_shared<TimeBlock>(1, 720, 830));
    datastructures1timeblocks.push_back(std::make_shared<TimeBlock>(4, 720, 830));

    std::shared_ptr<CourseInstance> datastructures1 = std::make_shared<CourseInstance>("data structures", 20001, std::vector<int>{2280, 2390, 6600, 6710});

    std::shared_ptr<CourseInstance> cs1 = std::make_shared<CourseInstance>("computer science", 10001, std::vector<int>{840, 950, 5160, 5270});
    std::shared_ptr<CourseInstance> cs2 = std::make_shared<CourseInstance>("computer science", 10002, std::vector<int>{2280, 2390, 6600, 6710});

    std::shared_ptr<CourseInstance> animation1 = std::make_shared<CourseInstance>("3D animation", 41001, std::vector<int>{840, 950, 5160, 5270});
    std::shared_ptr<CourseInstance> animation2 = std::make_shared<CourseInstance>("3D animation", 41002, std::vector<int>{960, 1070, 5280, 5390});
    std::shared_ptr<CourseInstance> animation3 = std::make_shared<CourseInstance>("3D animation", 41003, std::vector<int>{2280, 2390, 6600, 6710});
    std::shared_ptr<CourseInstance> animation4 = std::make_shared<CourseInstance>("3D animation", 41004, std::vector<int>{2340, 2390, 6660, 6770});

    std::shared_ptr<CourseInstance> graphic0 = std::make_shared<CourseInstance>("Graphics Storytelling", 42000, std::vector<int>{480, 590, 4800, 4910});
    std::shared_ptr<CourseInstance> graphic1 = std::make_shared<CourseInstance>("Graphics Storytelling", 42001, std::vector<int>{840, 950, 5160, 5270});
    std::shared_ptr<CourseInstance> graphic2 = std::make_shared<CourseInstance>("Graphics Storytelling", 42002, std::vector<int>{960, 1070, 5280, 5390});
    std::shared_ptr<CourseInstance> graphic3 = std::make_shared<CourseInstance>("Graphics Storytelling", 42003, std::vector<int>{2280, 2390, 6600, 6710});
    std::shared_ptr<CourseInstance> graphic4 = std::make_shared<CourseInstance>("Graphics Storytelling", 42004, std::vector<int>{2340, 2390, 6660, 6710});
    */

    std::unordered_map<std::string, std::string> datads;
    scheduler.importCourseInstances({
        {{"name", "data structures"}, {"crn", "20001"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "computer science I"}, {"crn", "10001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "computer science I"}, {"crn", "10002"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "3d animation"}, {"crn", "41001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "3d animation"}, {"crn", "41002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "3d animation"}, {"crn", "41003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "3d animation"}, {"crn", "41004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}},
        {{"name", "graphics storytelling"}, {"crn", "42000"}, {"timeBlocks", "480, 590, 4800, 4910"}},
        {{"name", "graphics storytelling"}, {"crn", "42001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "graphics storytelling"}, {"crn", "42002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "graphics storytelling"}, {"crn", "42003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "graphics storytelling"}, {"crn", "42004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}
        });

    /*
    std::vector<std::vector<std::shared_ptr<CourseInstance>>> courseInstances = {
        {datastructures1},
        {cs1, cs2},
        {animation1, animation2, animation3, animation4},
        {graphic0, graphic1, graphic2, graphic3, graphic4}
    };
    */

    scheduler.populate({{20001}, {10001, 10002}, {41001, 41002, 41003, 41004}, {42000, 42001, 42002, 42003, 42004}}, 4);

    Scheduler scheduler2;
    scheduler2.populate({
        {{{"name", "data structures"}, {"crn", "20001"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}},
        {{{"name", "computer science I"}, {"crn", "10001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "computer science I"}, {"crn", "10002"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}},
        {{{"name", "3d animation"}, {"crn", "41001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "3d animation"}, {"crn", "41002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "3d animation"}, {"crn", "41003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "3d animation"}, {"crn", "41004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}},
        {{{"name", "graphics storytelling"}, {"crn", "42000"}, {"timeBlocks", "480, 590, 4800, 4910"}},
        {{"name", "graphics storytelling"}, {"crn", "42001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "graphics storytelling"}, {"crn", "42002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "graphics storytelling"}, {"crn", "42003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}}
        }, 4);

    scheduler2.populate({
        {{{"name", "data structures"}, {"crn", "20001"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}},
        {{{"name", "computer science I"}, {"crn", "10001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "computer science I"}, {"crn", "10002"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}},
        {{{"name", "3d animation"}, {"crn", "41001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "3d animation"}, {"crn", "41002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "3d animation"}, {"crn", "41003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "3d animation"}, {"crn", "41004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}},
        {{{"name", "graphics storytelling"}, {"crn", "42000"}, {"timeBlocks", "480, 590, 4800, 4910"}},
        {{"name", "graphics storytelling"}, {"crn", "42001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "graphics storytelling"}, {"crn", "42002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "graphics storytelling"}, {"crn", "42003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "graphics storytelling"}, {"crn", "42004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}}
        }, 4);
}
