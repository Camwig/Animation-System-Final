#include "Sprite_anim.h"

Sprite_anim::Sprite_anim()
{

}


Sprite_anim::~Sprite_anim()
{

}

TextureAtlas* Sprite_anim::ReadTextureAtlasFromJSON(rapidjson::Document& tex_document)
{
	//Give the texture atlas the data read from the tex document
	TextureAtlas* tex_atlas = new TextureAtlas();
	tex_atlas->name_ = tex_document["name"].GetString();
	tex_atlas->width_ = tex_document["width"].GetFloat();
	tex_atlas->height_ = tex_document["height"].GetFloat();

	const rapidjson::Value& subtexture_array = tex_document["SubTexture"];
	//Loop through the subtextures of the texture atlas
	for (int subtex_num = 0; subtex_num < (int)subtexture_array.Size(); ++subtex_num)
	{
		//Read the subtexture data and add them to the texture atlas
		TexData* texdata = ReadSubtextureFromJSON(subtexture_array[subtex_num]);
		tex_atlas->subtex_atlas.insert(std::make_pair(texdata->name_, *texdata));
		delete texdata;
	}

	return tex_atlas;
}

TexData* Sprite_anim::ReadSubtextureFromJSON(const rapidjson::Value& subtecture_array)
{
	//Read the texture from the tex document to define the subtexture atlas
	TexData* subTextData = new TexData;
	subTextData->name_ = subtecture_array["name"].GetString();
	subTextData->width_ = subtecture_array["width"].GetFloat();
	subTextData->height_ = subtecture_array["height"].GetFloat();
	subTextData->x_ = subtecture_array["x"].GetFloat();
	subTextData->y_ = subtecture_array["y"].GetFloat();

	if (subtecture_array.HasMember("frameX"))
	{
		subTextData->frame_x_ = subtecture_array["frameX"].GetFloat();
	}
	else
	{
		//If the subtexture does not hold any data for this variable set it to zero
		subTextData->frame_x_ = 0;
	}

	if (subtecture_array.HasMember("frameY"))
	{
		subTextData->frame_y_ = subtecture_array["frameY"].GetFloat();
	}
	else
	{
		subTextData->frame_y_ = 0;
	}

	if (subtecture_array.HasMember("frameWidth"))
	{
		subTextData->frame_width_ = subtecture_array["frameWidth"].GetFloat();
	}
	else
	{
		subTextData->frame_width_ = subTextData->width_;
	}

	if (subtecture_array.HasMember("frameHeight"))
	{
		subTextData->frame_height_ = subtecture_array["frameHeight"].GetFloat();
	}
	else
	{
		subTextData->frame_height_ = subTextData->height_;
	}

	//Build the transform of the sub texture
	subTextData->BuildTransform();

	return subTextData;
}

void Sprite_anim::SetSpriteSizeAndPositionForFrame(gef::Sprite* sprite, float screen_x, float screen_y, TextureAtlas* texture_atlas, std::string subtext_)
{
	//Read the data from the subtexture based off the part given
	float width = texture_atlas->subtex_atlas.at(subtext_).width_;
	float height = texture_atlas->subtex_atlas.at(subtext_).height_;
	float x = texture_atlas->subtex_atlas.at(subtext_).x_;
	float y = texture_atlas->subtex_atlas.at(subtext_).y_;
	float frame_width = texture_atlas->subtex_atlas.at(subtext_).frame_width_;
	float frame_height = texture_atlas->subtex_atlas.at(subtext_).frame_height_;
	float frame_x = texture_atlas->subtex_atlas.at(subtext_).frame_x_;
	float frame_y = texture_atlas->subtex_atlas.at(subtext_).frame_y_;

	//Set the sprites values such as it size and uv position
	sprite->set_width(width);
	sprite->set_height(height);
	sprite->set_uv_width(width / texture_atlas->width_);
	sprite->set_uv_height(height / texture_atlas->height_);

	float u = x / texture_atlas->width_;
	float v = y / texture_atlas->height_;
	sprite->set_uv_position(gef::Vector2(u, v));

	float sprite_x = width * 0.5f - (frame_width * 0.5f + frame_x);
	float sprite_y = height * 0.5f - (frame_height * 0.5f + frame_y);

	//Set the spites new position
	sprite->set_position(gef::Vector4(screen_x + sprite_x, screen_y + sprite_y, 0.0f));
}

void Sprite_anim::SetupRig(gef::Matrix33* rig_transform_m_, gef::Vector2 sprite_pos_, float scale)
{
	//Setup the rig transform based off the scale and position
	rig_transform_m_->SetIdentity();
	rig_transform_m_->Scale(gef::Vector2(scale, scale));
	rig_transform_m_->SetTranslation(gef::Vector2(sprite_pos_.x, sprite_pos_.y));
}

//Functions to be overloaded based of the type of animation
gef::Sprite* Sprite_anim::SetupAnimation(gef::Platform*, gef::Sprite* sprite, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts,std::string* WhichAnim1, float scale_)
{
	return sprite;
}

void Sprite_anim::Update(int frame, gef::Sprite* sprite_, gef::Vector2 position_, std::map<std::string, gef::Matrix33>& Transforms_for_bone_)
{
}

gef::Sprite* Sprite_anim::Render(gef::Sprite* sprite)
{
	return sprite;
}

gef::Sprite* Sprite_anim::Render(gef::Sprite* sprite,gef::Matrix33& transform, std::string part, gef::Vector2 Position)
{
	return sprite;
}
