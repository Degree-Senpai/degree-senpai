#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <type_traits>

// Helper trait to check if T is a std::vector
template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};


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

template<typename T>
std::string recursiveVecToString(const T& vec) {
    std::ostringstream oss;
    if constexpr (is_vector<T>::value) {
        oss << "[";
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            if (it != vec.begin()) {
                oss << ", ";
            }
            oss << recursiveVecToString(*it); // Recursively print elements of the vector
        }
        oss << "]";
    } else {
        oss << vec; // Base case: just print the value
    }
    return oss.str();
}

std::vector<std::string> stringToVec(const std::string str);

std::vector<int> stringToIntVec(const std::string str);

#endif
