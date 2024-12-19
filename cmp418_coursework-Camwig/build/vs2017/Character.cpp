#include "Character.h"

#include <fstream>
#include "load_texture.h"
#include "load_json.h"

Character::Character() :
	sprite_texture_(NULL),sprite_(NULL)
{
	//Transform = new gef::Matrix33;
	Transform = NULL;
	WhichAnim_ = NULL;
}

Character::~Character()
{
	delete sprite_texture_;
	sprite_texture_ = NULL;

	delete sprite_;
	sprite_ = NULL;

	delete Transform;
	Transform = NULL;

	delete WhichAnim_;
	WhichAnim_ = NULL;
}

void Character::LoadCharacter(std::string tex_string, gef::Platform* platform_)
{

	sprite_ = new gef::Sprite();

	sprite_->set_width(256.0f);
	sprite_->set_height(512.0f);

	Position.x = platform_->width() * 0.5f;
	Position.y = platform_->height() * 0.5f;

	std::string tex_string_temp = tex_string + "_tex.png";

	sprite_texture_ = CreateTextureFromPNG(tex_string_temp.c_str(), *platform_);

	sprite_->set_texture(sprite_texture_);

	Sprite_anim* new_sheet;
	new_sheet = new Sprite_anim();
	new_sheet->Load_sprite_and_texture_3(tex_string, rapidjson_doc_tex, rapidjson_doc_ske);

	delete new_sheet;

	//SetAnimation(tex_string,platform_);
}

void Character::SetAnimation(std::string& tex_string, gef::Platform* platform_)
{
	Anim* new_animantion;
	new_animantion = new Anim();

	new_animantion->SetupAnim(platform_, sprite_, tex_string, rapidjson_doc_tex, rapidjson_doc_ske, Position, bone_parts, Type, WhichAnim_);

	if (new_animantion->WhichAnim_ != NULL)
	{
		tex_string = tex_string + "_" + *new_animantion->WhichAnim_;
		animations.insert(std::make_pair(tex_string, new_animantion));
	}
	else
	{
		animations.insert(std::make_pair(tex_string, new_animantion));
	}
}

void Character::SetWhichAnimation(std::string tex_string)
{
	//WhichAnim_ = new std::string;
	WhichAnim_ = new std::string(tex_string);
	//WhichAnim_ = &tex_string;
}

void Character::Update(std::string tex_string, int frame)
{
	animations.at(tex_string)->Update(frame, sprite_,Position, Transforms_for_bone_);
}

gef::Sprite* Character::Render(std::string tex_string, std::string part)
{
	//return sprite_;
	gef::Matrix33 NewTransform;
	//gef::Sprite new_sprite;
	animations.at(tex_string)->Render(sprite_, NewTransform, part,Position);
	Transform = &NewTransform;
	return sprite_;
}