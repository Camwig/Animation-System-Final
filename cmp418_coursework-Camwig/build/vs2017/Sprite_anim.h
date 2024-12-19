#pragma once
#include <vector>
#include "rapidjson\document.h"
#include "rapidjson/istreamwrapper.h"
#include <graphics/sprite.h>
#include <system/application.h>
#include "maths/matrix33.h"
#include <map>
#include <string>

//Struct to define the Texture data
struct TexData
{
	const char* name_;
	float width_;
	float height_;
	float frame_height_;
	float frame_width_;
	float x_;
	float y_;
	float frame_x_;
	float frame_y_;

	gef::Matrix33 transform_m_;
	gef::Matrix33 translate_m_;
	gef::Matrix33 scale_m_;

	//Build the transform of the data
	void BuildTransform()
	{
		transform_m_.SetIdentity();
		translate_m_.SetIdentity();
		scale_m_.SetIdentity();

		float x = width_ * 0.5f - (frame_width_ * 0.5f + frame_x_);
		float y = height_ * 0.5f - (frame_height_ * 0.5f + frame_y_);

		transform_m_.SetTranslation(gef::Vector2(x, y));
		scale_m_.Scale(gef::Vector2(width_, height_));

		transform_m_ = scale_m_ * translate_m_;
	}
};

//Struct to define the Texture atlas
struct TextureAtlas
{
	const char* name_;
	float width_;
	float height_;

	std::vector<TexData> subtextures;
	std::map<std::string, TexData> subtex_atlas;
};

class Sprite_anim
{
public:
	Sprite_anim();
	~Sprite_anim();

	//Functions that remain consistent
	TextureAtlas* ReadTextureAtlasFromJSON(rapidjson::Document&);
	TexData* ReadSubtextureFromJSON(const rapidjson::Value&);
	void SetSpriteSizeAndPositionForFrame(gef::Sprite*, float, float, TextureAtlas*, std::string);
	void SetupRig(gef::Matrix33* rig_transform_m_, gef::Vector2 sprite_pos_, float scale);

	//Functions that may need to be overwritten based off which type of animation is being played
	virtual gef::Sprite* SetupAnimation(gef::Platform*, gef::Sprite*, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts,std::string* WhichAnim1, float scale_);
	virtual void Update(int frame, gef::Sprite* sprite_, gef::Vector2 position_,std::map<std::string, gef::Matrix33>& Transforms_for_bone_);
	virtual gef::Sprite* Render(gef::Sprite* sprite);
	virtual gef::Sprite* Render(gef::Sprite* sprite, gef::Matrix33& transform,std::string part, gef::Vector2 Position);

	//Both skeletal and sprite make use of scale
	//Could add more overlapping variables here
	float scale;

};

