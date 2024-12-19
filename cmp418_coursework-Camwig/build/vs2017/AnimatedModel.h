#pragma once
#include <string>
#include <map>
#include "PlayableAnim.h"
class AnimatedModel
{
public:
	std::string Model_Name_;
	char* Model_PathWay_;
	
	std::map<std::string, PlayableAnim> Anim_map;
};

