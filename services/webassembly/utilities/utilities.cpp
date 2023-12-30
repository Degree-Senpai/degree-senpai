#include "utilities.h"

std::vector<std::string> stringToVec(const std::string str) {
    std::vector<std::string> result;
    std::string holding = "";
    for (char c : str) {
        if (c == ',') {
            result.push_back(holding);
            holding = "";
        } else if (c != ' ') {
            holding += c;
        }
    }
    result.push_back(holding);
    return result;
}

std::vector<int> stringToIntVec(const std::string str) {
    std::vector<int> result;
    std::string holding = "";
    for (char c : str) {
        if (c == ',') {
            result.push_back(stoi(holding));
            holding = "";
        } else if (c != ' ') {
            holding += c;
        }
    }
    result.push_back(stoi(holding));
    return result;
}
