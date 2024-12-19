#pragma once

#include "maths/math_utils.h"
#include <maths/vector2.h>
#include <maths/matrix33.h>
#include "rapidjson\document.h"
#include <string>
#include <vector>

//Struct Defining the Translation Keys
struct TranslationKey
{
	float time_to_next_key;
	float start_time;
	gef::Vector2 translation;
};

//Struct defining the Rotation Keys
struct RotationKey
{
	float time_to_next_key;
	float start_time;
	float rotation;
};

//Struct Defining the BoneKeys
struct BoneKey
{
	std::string name;
	std::vector<TranslationKey*> translation_keys;
	std::vector<RotationKey*> rotation_keys;
};

//Struct Defining the animation
struct Animation
{
	float duration;
	std::string name;
	std::vector<BoneKey*> bone_keys;
	bool render;
};


class Animation_Parser
{
public:
	TranslationKey* ReadTranslationKeyDataFromJSON(const rapidjson::Value&);
	RotationKey* ReadRotationKeyDataFromJSON(const rapidjson::Value&);
	Animation* ReadAnimationFromJSON(const rapidjson::Value&);

	gef::Vector2 lerp(gef::Vector2, gef::Vector2,float);
	float lerp(float,float,float);
};

