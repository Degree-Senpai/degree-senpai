#include "scheduler.h"

void printSchedules(std::vector<std::vector<Schedule>> schedules) {
    int size = 0;
    for (const auto& scheduleBin : schedules) {
        size += scheduleBin.size();
    }
    std::cout << "final computed schedules (" << size << "): \n"; 
    for (const auto& scheduleBin : schedules) {
        for (const auto& schedule : scheduleBin) {
            std::cout << "  " << schedule << "\n";
        }
    }
    std::cout << std::endl;
}

int main() {
    Scheduler scheduler;
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

    std::vector<std::vector<Schedule>> schedules = scheduler.populate({{20001}, {10001, 10002}, {41001, 41002, 41003, 41004}, {42000, 42001, 42002, 42003, 42004}}, 4);
    printSchedules(schedules);

    Scheduler scheduler2;
    schedules = scheduler2.populate({
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
    printSchedules(schedules);

    schedules = scheduler2.populate({
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
    printSchedules(schedules);
}
