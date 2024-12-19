#pragma once

#include "Sprite_anim.h"

class Sheet_Sprite_anim : public Sprite_anim
{
public:
	Sheet_Sprite_anim();
	~Sheet_Sprite_anim();

	//Virtual functions from Sprite_anim
	gef::Sprite* SetupAnimation(gef::Platform*, gef::Sprite*, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts, std::string* WhichAnim1, float scale_) override;
	void Update(int frame, gef::Sprite* sprite_, gef::Vector2 position_,std::map<std::string, gef::Matrix33>& Transforms_for_bone_) override;
	gef::Sprite* Render(gef::Sprite* sprite, gef::Matrix33& transform, std::string part, gef::Vector2 Position) override;

private:
	TextureAtlas* text_atlas;

	//Map of the order of animation frames
	std::map<int, std::string> run_order;

	//Functions to organise the order of the animation frames
	std::map<int, std::string> SetupOrder(rapidjson::Document&);
	std::map< std::string,TexData> ReOrganiseSubtextures();

	//Vector of bone parts of the animation
	std::vector<std::string> bone_parts_;

	std::string string_name_;
	gef::Matrix33 rig_transform_m_;
};

