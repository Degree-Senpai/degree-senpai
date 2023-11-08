#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include <iterator>

// templated functions must be defined inside header!
template <typename T>
std::string vecToString(const std::vector<T>& vec) {
    std::ostringstream oss;
    if (!vec.empty()) {
        // Convert all but the last element to avoid a trailing ","
        std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(oss, ", "));

        // Now add the last element with no delimiter
        oss << vec.back();
    }
    return oss.str();
};

std::vector<std::string> stringToVec(const std::string str);

std::vector<int> stringToIntVec(const std::string str);

#endif
