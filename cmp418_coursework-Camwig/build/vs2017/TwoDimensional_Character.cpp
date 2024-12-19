#include "TwoDimensional_Character.h"

#include <fstream>
#include "load_texture.h"
#include "load_json.h"

TwoDimensional_Character::TwoDimensional_Character() :
	sprite_texture_(NULL),sprite_(NULL),Transform(NULL),WhichAnim_(NULL)
{
}

TwoDimensional_Character::~TwoDimensional_Character()
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

void TwoDimensional_Character::LoadCharacter(std::string tex_string, gef::Platform* platform_, gef::Vector2 position_, float scale_)
{
	//Loads and sets up every variable
	animation_timer_ = 0;
	frame = 0;

	sprite_ = new gef::Sprite();

	Position.x = position_.x;
	Position.y = position_.y;

	std::string tex_string_temp = tex_string + "_tex.png";
	Scale = scale_;

	sprite_texture_ = CreateTextureFromPNG(tex_string_temp.c_str(), *platform_);

	sprite_->set_texture(sprite_texture_);

	//Call the function to parse the ske and tex documents
	Parse_the_ske_and_tex_docs_(tex_string);
}

void TwoDimensional_Character::Parse_the_ske_and_tex_docs_(std::string tex_string)
{
	//Parses the documents

	std::string tex_string_temp = tex_string + "_tex.json";
	char* JSON_Doc = LoadJSON(tex_string_temp.c_str());
	std::ifstream ifs(tex_string_temp.c_str());
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson_doc_tex.ParseStream(isw);

	tex_string_temp = tex_string + "_ske.json";
	char* JSON_Doc_2 = LoadJSON(tex_string_temp.c_str());
	std::ifstream ifs2(tex_string_temp.c_str());
	rapidjson::IStreamWrapper isw2(ifs2);
	rapidjson_doc_ske.ParseStream(isw2);
}

void TwoDimensional_Character::SetAnimation(std::string& tex_string, gef::Platform* platform_)
{
	Anim_manager* new_animantion;
	new_animantion = new Anim_manager();

	//Sets up the 2D animation
	new_animantion->SetupAnim(platform_, sprite_, tex_string, rapidjson_doc_tex, rapidjson_doc_ske, Position, bone_parts, Type, WhichAnim_,Scale);

	//Checks if the character has mutliple animations from file and sets it one of them
	if (new_animantion->GetWhichAnim() != NULL)
	{
		std::string new_string = tex_string;
		new_string = tex_string + "_" + *new_animantion->GetWhichAnim();
		//Inserts the new animation into this map
		animations.insert(std::make_pair(new_string, new_animantion));
	}
	else //If there is not multiple animations and only one we just add the animation as the name
	{
		//Inserts the new animation into this map
		animations.insert(std::make_pair(tex_string, new_animantion));
	}
}

void TwoDimensional_Character::Update(std::string tex_string, int frame)
{
	//Calls a particular animation to be updated
	animations.at(tex_string)->Update(frame, sprite_,Position, Transforms_for_bone_);
}

void TwoDimensional_Character::UpdateAnimation(float frame_time, std::string sprite_name_)
{
	//Inciments the animation timer
	animation_timer_ += frame_time;

	if (animation_timer_ >= (1.f / animations.at(sprite_name_)->GetFrameRate()))
	{
		//Incriments the frame and resets the animation timer
		frame++;
		animation_timer_ = 0;
	}

	//If the current frame is greater or equal to the duration
	if (frame >= animations.at(sprite_name_)->GetDuration())
	{
		//Reset frame
		frame = 0;
	}

	Update(sprite_name_, frame);
}

void TwoDimensional_Character::RenderAnimation(std::string sprite_name_,gef::SpriteRenderer* sprite_renderer_)
{
	//Checks the type of animation being played

	//If its skeletal the drawn sprite should be drawn in accordance with a transform matrix
	//If it is a sheet animation it should not
	if (Type == "Sheet")
	{
		//Loops through the bone parts of the character
		for (auto part : bone_parts)
		{
			//Draws the sprite
			sprite_renderer_->DrawSprite(*Render(sprite_name_, part));
		}
	}
	else
	{
		//Loops through the bone parts of the character

		//Bone parts is wrong
		for (auto part : bone_parts)
		{
			//Draws the sprite
			sprite_ = Render(sprite_name_, part);
			gef::Matrix33 this_trans = *Transform;
			sprite_renderer_->DrawSprite(*sprite_, this_trans);
		}
	}
}

gef::Sprite* TwoDimensional_Character::Render(std::string tex_string, std::string part)
{
	//Updates the sprite and the transform
	gef::Matrix33 NewTransform;
	animations.at(tex_string)->Sprite_Render(sprite_, NewTransform, part,Position);
	Transform = &NewTransform;
	return sprite_;
}

std::string TwoDimensional_Character::SetAnimationName(std::string name, std::string anim_name)
{
	name = name + "_" + anim_name;
	return name;
}

void TwoDimensional_Character::SetWhichAnimation(std::string tex_string)
{
	WhichAnim_ = new std::string(tex_string);
}