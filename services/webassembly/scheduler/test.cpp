#include <nlohmann/json.hpp>
#include "scheduler.h"
#include "../utilities/utilities.h"

using json = nlohmann::json;

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
    scheduler.importCourseInstanceDictionaries({
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
    schedules = scheduler2.importAndPopulate({
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

    schedules = scheduler2.importAndPopulate({
        {{{"name", "data structures"}, {"crn", "20001"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}},
        {{{"name", "computer science I"}, {"crn", "10001"}, {"timeBlocks", "2390, 2500, 6710, 6820"}},
        {{"name", "computer science I"}, {"crn", "10002"}, {"timeBlocks", "2280, 2390, 6600, 6710"}}}
    }, 4, true);
    std::cout << "edge cases (20001 + 10001 have touching timeblocks, 20001 and 10002 have same exact timeblocks) " << std::endl;
    printSchedules(schedules);

    std::vector<std::vector<std::unordered_map<std::string, std::string>>> data = {
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
        {{"name", "graphics storytelling"}, {"crn", "42004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}}};

    std::string scheduleDataJson = scheduler2.populateAndExport(json(data).dump(), 4);
    std::cout << "JSON RETURN: \n" << scheduleDataJson << std::endl;

    std::vector<std::vector<std::unordered_map<std::string, std::string>>> data2 = {
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
        {{"name", "graphics storytelling"}, {"crn", "42004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}},
        {{{"name", "introduction to ecse"}, {"crn", "50000"}, {"timeBlocks", "480, 590, 4800, 4910"}},
        {{"name", "introduction to ecse"}, {"crn", "50001"}, {"timeBlocks", "840, 950, 5160, 5270"}},
        {{"name", "introduction to ecse"}, {"crn", "50002"}, {"timeBlocks", "960, 1070, 5280, 5390"}},
        {{"name", "introduction to ecse"}, {"crn", "50003"}, {"timeBlocks", "2280, 2390, 6600, 6710"}},
        {{"name", "introduction to ecse"}, {"crn", "50004"}, {"timeBlocks", "2340, 2390, 6660, 6770"}}},
        {{{"name", "introduction to electronics"}, {"crn", "51000"}, {"timeBlocks", "540, 590, 4800, 4910"}},
        {{"name", "introduction to electronics"}, {"crn", "51001"}, {"timeBlocks", "900, 950, 5220, 5270"}},
        {{"name", "introduction to electronics"}, {"crn", "51002"}, {"timeBlocks", "1020, 1070, 5340, 5390"}},
        {{"name", "introduction to electronics"}, {"crn", "51003"}, {"timeBlocks", "2340, 2390, 6660, 6710"}},
        {{"name", "introduction to electronics"}, {"crn", "51004"}, {"timeBlocks", "2400, 2390, 6720, 6770"}}},
        {{{"name", "data science"}, {"crn", "22000"}, {"timeBlocks", "4800, 5030"}},
        {{"name", "data science"}, {"crn", "22001"}, {"timeBlocks", "5220, 5390"}},
        {{"name", "data science"}, {"crn", "22002"}, {"timeBlocks", "5340, 5510"}},
        {{"name", "data science"}, {"crn", "22003"}, {"timeBlocks", "6660, 6830"}},
        {{"name", "data science"}, {"crn", "22004"}, {"timeBlocks", "6720, 6890"}}},
        {{{"name", "data mining"}, {"crn", "23001"}, {"timeBlocks", "6660, 6830"}},
        {{"name", "data mining"}, {"crn", "23002"}, {"timeBlocks", "6720, 6890"}}}};

    std::cout << "\nMAJOR TEST!\n" << std::endl;
    std::vector<std::vector<Schedule>> schedulesCRN2 = scheduler2.importAndPopulate(data2, 10);
    int totalSize = 0;
    for (auto& e : schedulesCRN2) {
        totalSize += e.size();
    }
    std::cout << "generated " << totalSize << " schedules" << std::endl;
    //std::cout << "JSON RETURN: \n" << scheduleDataJson2 << std::endl;
}
