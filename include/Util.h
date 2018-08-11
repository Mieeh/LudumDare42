#pragma once

#include<core\vector2.h>
using namespace bear::core;

#include<string>

static std::string positionToStringKey(Vector2i v) {
	std::string result = "";
	result += std::to_string(v.x) + ", ";
	result += std::to_string(v.y);
	return result;
}