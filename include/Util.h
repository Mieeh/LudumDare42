#pragma once

#include<core\vector2.h>
using namespace bear::core;

#include<string>
#include<vector>
#include<map>

static std::string positionToStringKey(Vector2i v) {
	std::string result = "";
	result += std::to_string(v.x) + ", ";
	result += std::to_string(v.y);
	return result;
}

template<typename T>
static void clearVectorOfPointers(std::vector<T*>& vector) {
	for (auto it = vector.begin(); it != vector.end(); ++it) {
		delete *it;
	}
	vector.clear();
}

template <typename K, typename V>
static void clearMapOfPointers(std::map<K, V*>& map) {
	for (auto it = map.begin(); it != map.end(); ++it) {
		delete it->second;
	}
	map.clear();
}