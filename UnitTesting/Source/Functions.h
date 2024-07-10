#pragma once
#include "JuceHeader.h"



bool arrEq(std::array<int, 12> a1, std::array<int, 12> a2){

	for (int i = 0; i < 12; i++) {
		if (a1[i] != a2[i]) {
			return false;
		}
	}
	return true;
}

bool inversionEq(std::array<int, 12> a1, std::array<int, 12> a2, int numCTs) {

	for (int i = 0; i < numCTs; i++) {
		if (a1[i] != a2[i]) {
			return false;
		}
	}
	return true;
}

std::string inversionToString(const std::array<int,12> arr, int numCTs) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < numCTs; ++i) {
        if (i > 0) oss << ", ";
        oss << arr[i];
    }
    oss << "]";
    return oss.str();
}


std::string chordToString(const std::array<int, 12> arr) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < 12; ++i) {
        if (i > 0) oss << ", ";
        oss << arr[i];
    }
    oss << "]";
    return oss.str();
}
