#pragma once

#include "Anim_manager.h"
#include <maths/vector2.h>
#include <graphics/sprite_renderer.h>
#include "rapidjson\document.h"
#include "rapidjson/istreamwrapper.h"

namespace gef
{
	class SpriteRenderer;
}

class TwoDimensional_Character
{
public:
	TwoDimensional_Character();
	~TwoDimensional_Character();

	//Sets up the character
	void LoadCharacter(std::string,gef::Platform*,gef::Vector2 position_,float scale_);
	//Update the character
	void Update(std::string tex_string, int frame);
	//Render the correct part of the sprite
	gef::Sprite* Render(std::string tex_string,std::string part);
	//Render the updated animated character
	void RenderAnimation(std::string sprite_name_, gef::SpriteRenderer* sprite_renderer_);
	//Sets which animation to perform if there are multiple
	void SetWhichAnimation(std::string tex_string);
	//Creates an animation and added to the character
	void SetAnimation(std::string& tex_string, gef::Platform* platform_);
	//Updates the given animation
	void UpdateAnimation(float frame_time, std::string sprite_name_);
	//Sets the name of the animation and returns the variable
	std::string SetAnimationName(std::string name,std::string anim_name);
	//Loads and parses the data from the tex and ske file
	void Parse_the_ske_and_tex_docs_(std::string);
private:
	float Scale;
	int frame;
	float animation_timer_;
	gef::Texture* sprite_texture_;
	gef::Vector2 Position;
	rapidjson::Document rapidjson_doc_tex;
	rapidjson::Document rapidjson_doc_ske;
	std::map<std::string, Anim_manager*> animations;
	gef::Sprite* sprite_;
	std::string Type;
	std::vector<std::string> bone_parts;
	std::map<std::string, gef::Matrix33> Transforms_for_bone_;
	gef::Matrix33* Transform;
	std::string* WhichAnim_;
	//gef::Skeleton* skeleton;
};

