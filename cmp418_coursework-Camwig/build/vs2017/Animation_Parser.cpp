#include "Animation_Parser.h"


TranslationKey* Animation_Parser::ReadTranslationKeyDataFromJSON(const rapidjson::Value& array_pos)
{
	//Init new translation key
	TranslationKey* trans_key = new TranslationKey();

	//Get time to next key
	float time_to_next_key = array_pos["duration"].GetFloat();

	//Check it has "x" and "y" data fields
	bool hasTranslationX = array_pos.HasMember("x");
	bool hasTranslationY = array_pos.HasMember("y");

	//if it does get that data, otherwise inti to 0 through the use of ?
	float translationX = hasTranslationX ? array_pos["x"].GetFloat() : 0;
	float translationY = hasTranslationY ? array_pos["y"].GetFloat() : 0;

	//set time to next key and translation data
	trans_key->time_to_next_key = time_to_next_key;
	trans_key->translation = gef::Vector2(translationX, translationY);

	//Return newly populated translation key data
	return trans_key;
}

RotationKey* Animation_Parser::ReadRotationKeyDataFromJSON(const rapidjson::Value& array_pos)
{
	//Init new rotation key
	RotationKey* rot_key = new RotationKey();

	//Get time to next key
	float time_to_next_key = array_pos["duration"].GetFloat();

	//check it has rotation data field
	bool hasRotation = array_pos.HasMember("rotate");

	//If it does, get that data,otherwise init to 0
	float rotatation = hasRotation ? array_pos["rotate"].GetFloat() : 0;

	//set time to next key and rotation data
	rot_key->time_to_next_key = time_to_next_key;
	rot_key->rotation = rotatation;

	//return newly populated rotation key data
	return rot_key;
}

Animation* Animation_Parser::ReadAnimationFromJSON(const rapidjson::Value& array_pos)
{
	//Init new animation
	Animation* anim = new Animation();

	//Set anim duration, name and render
	anim->duration = array_pos["duration"].GetFloat();
	anim->name = array_pos["name"].GetString();
	anim->render = true;

	//check it has bones Skeleton-based
	bool hasBones = array_pos.HasMember("bone");
	if (hasBones)
	{
		//Begin parsing bone data
		const rapidjson::Value& bone_array = array_pos["bone"];
		for (int bone_num = 0; bone_num < bone_array.Size(); bone_num++)
		{
			BoneKey* bone_key = new BoneKey();
			bone_key->name = bone_array[bone_num]["name"].GetString();

			//If there are translation keys, get their data
			if (bone_array[bone_num].HasMember("translateFrame"))
			{
				for (int i = 0; i < bone_array[bone_num]["translateFrame"].Size(); i++)
				{
					bone_key->translation_keys.push_back(ReadTranslationKeyDataFromJSON(bone_array[bone_num]["translateFrame"][i]));
				}
			}

			//If there are rotation keys, get their data
			if (bone_array[bone_num].HasMember("rotateFrame"))
			{
				for (int i = 0; i < bone_array[bone_num]["rotateFrame"].Size(); i++)
				{
					bone_key->rotation_keys.push_back(ReadRotationKeyDataFromJSON(bone_array[bone_num]["rotateFrame"][i]));
				}
			}

			anim->bone_keys.push_back(bone_key);
		}
	}
	return anim;
}

gef::Vector2 Animation_Parser::lerp(gef::Vector2 start_pos, gef::Vector2 end_pos, float time)
{
	//lerp X and Y
	float x = (1.f - time) * start_pos.x + time * end_pos.x;
	float y = (1.f - time) * start_pos.y + time * end_pos.y;

	//return lerp values
	return gef::Vector2(x, y);
}

float Animation_Parser::lerp(float start_angle,float end_angle,float time)
{
	//Get diffrence in angles
	float angle_diff = end_angle - start_angle;

	//Init angle var
	float angle;

	//Check if we can skip around
	if (angle_diff > 180)
	{
		angle_diff -= 360;
	}
	else if (angle_diff < -180)
	{
		angle += 360;
	}

	//return lerp vakues
	angle = start_angle + time * angle_diff;
	return angle;
}