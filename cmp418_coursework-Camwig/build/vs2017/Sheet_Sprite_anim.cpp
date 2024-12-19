#include "Sheet_Sprite_anim.h"

#include "load_texture.h"
#include "load_json.h"
#include <fstream>

Sheet_Sprite_anim::Sheet_Sprite_anim()
{
	//Initialise the variables that need to be
	text_atlas = new TextureAtlas;
}

Sheet_Sprite_anim::~Sheet_Sprite_anim()
{
	//Clean up the pointer
	delete text_atlas;
	text_atlas = NULL;
}

void Sheet_Sprite_anim::Update(int frame, gef::Sprite* sprite_, gef::Vector2 position_, std::map<std::string, gef::Matrix33>& Transforms_for_bone_)
{
	//Update the given frame of animation
	std::string name = string_name_ + std::to_string(frame);
	SetSpriteSizeAndPositionForFrame(sprite_, position_.x, position_.y,text_atlas, name);
}

gef::Sprite* Sheet_Sprite_anim::Render(gef::Sprite* sprite, gef::Matrix33& transform, std::string part, gef::Vector2 Position)
{
	return sprite;
}

gef::Sprite* Sheet_Sprite_anim::SetupAnimation(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts, std::string* WhichAnim1, float scale_)
{
	//Initalise the variables
	scale = scale_;

	bone_parts_.push_back("Default");
	bone_parts = bone_parts_;

	//Set up the rig transform
	SetupRig(&rig_transform_m_, Position, scale);

	//Set the texture atlas
	text_atlas = ReadTextureAtlasFromJSON(tex_document);

	//Set the order of the animation
	run_order = SetupOrder(ske_document);

	//Re-organise the subtextures into the correct order
	text_atlas->subtex_atlas = ReOrganiseSubtextures();

	return sprite_;
}

std::map<int, std::string> Sheet_Sprite_anim::SetupOrder(rapidjson::Document& ske_document)
{
	//Get the order of the frames to display
	const rapidjson::Value& order_array = ske_document["armature"][0]["skin"][0]["slot"][0]["display"];
	std::map<int,std::string> new_order;
	
	//Loop through array
	for (int order_num = 0; order_num < (int)order_array.Size(); ++order_num)
	{
		//Insert the frames into the map in the order they are to be rendered
		new_order.insert(std::make_pair(order_num, order_array[order_num]["name"].GetString()));
	}
	
	return new_order;
}

std::map<std::string, TexData> Sheet_Sprite_anim::ReOrganiseSubtextures()
{
	std::map<std::string,TexData> new_map;
	bool got_string = false;
	for (int i = 0; i < run_order.size(); i++)
	{
		//For every item in the subtexture atlas
		for (auto j : text_atlas->subtex_atlas)
		{
			//If the current item from subtexture atlas is equal to the current item in the run order
			if (text_atlas->subtex_atlas.at(j.first).name_ == run_order[i])
			{
				//Insert it into a new map
				new_map.insert(std::make_pair(j.first, j.second));
			}

			//Sets the name of animation so that we can simply add the frame number to the name later
			//to get whichever frame of animation it needs
			if (!got_string)
			{
				int string_length_ = j.first.size();
				int new_length = string_length_ - 1;
				string_name_ = j.first.substr(0, new_length);
				got_string = true;
			}
		}
	}
	return new_map;
}