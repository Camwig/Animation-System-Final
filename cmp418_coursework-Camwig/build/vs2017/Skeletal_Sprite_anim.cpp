#include "Skeletal_Sprite_anim.h"

#include <fstream>
#include "load_texture.h"
#include "load_json.h"


Skeletal_Sprite_anim::Skeletal_Sprite_anim()
{
	text_atlas = new TextureAtlas;
}
Skeletal_Sprite_anim::~Skeletal_Sprite_anim()
{
	delete text_atlas;
	text_atlas = NULL;
}

std::map <std::string, SkinSlot> Skeletal_Sprite_anim::ReadSkinSlotsDataFromJSON(rapidjson::Document& ske_document)
{
	//Reads the skin slots data from the ske file
	const rapidjson::Value& slots_array = ske_document["armature"][0]["skin"][0]["slot"];

	std::map<std::string, SkinSlot> slots;

	for (int slot_num = 0; slot_num < (int)slots_array.Size(); ++slot_num)
	{
		SkinSlot* slot = new SkinSlot();
		slot->name_ = slots_array[slot_num]["name"].GetString();
		slot->part_name_ = slots_array[slot_num]["display"][0]["name"].GetString();
		slot->x_ = slots_array[slot_num]["display"][0]["transform"]["x"].GetFloat();
		slot->y_ = slots_array[slot_num]["display"][0]["transform"]["y"].GetFloat();
		if (slot->rot_ = slots_array[slot_num]["display"][0]["transform"].HasMember("skY"))
		{
			slot->rot_ = slots_array[slot_num]["display"][0]["transform"]["skY"].GetFloat();
		}
		else
		{
			slot->rot_ = 0;
		}

		slot->BuildTransform();

		slots.insert(std::make_pair(slot->name_, *slot));

		delete slot;
	}

	return slots;

}

std::map<std::string, Animation> Skeletal_Sprite_anim::ReadAnimationDataFromJSON(rapidjson::Document& ske_document)
{
	//Reads the animation data from the ske file
	const rapidjson::Value& anims_array = ske_document["armature"][0]["animation"];

	std::map<std::string, Animation> anims;

	for (int anim_num = 0; anim_num < (int)anims_array.Size(); ++anim_num)
	{
		Animation* anim = new Animation();
		anim->name = anims_array[anim_num]["name"].GetString();
		anim->duration = anims_array[anim_num]["duration"].GetFloat();
		//Checks if the current element has bones
		if (anims_array[anim_num].HasMember("bone"))
		{
			//Loops through each bone of the current element
			const rapidjson::Value& anim_bones_array = ske_document["armature"][0]["animation"][anim_num]["bone"];

			for (int it = 0; it < anim_bones_array.Size(); ++it)
			{
				BoneKey* bKey = new BoneKey;

				//Gets the bone keys name
				bKey->name = anim_bones_array[it]["name"].GetString();

				//If the bone key has a translation frame get that
				if (anim_bones_array[it].HasMember("translateFrame"))
				{
					const rapidjson::Value& anim_bones_trans_array = ske_document["armature"][0]["animation"][anim_num]["bone"][it]["translateFrame"];

					//For each of the bone keys translation frames
					for (int it2 = 0; it2 < anim_bones_trans_array.Size(); ++it2)
					{
						TranslationKey* trans_key = anim_pars->ReadTranslationKeyDataFromJSON(anim_bones_trans_array[it2]);
						//Adds the translation keys to the bone
						bKey->translation_keys.push_back(trans_key);

					}
				}
				else //If no translation frames were found
				{
					//Set them default values
					TranslationKey* trans_key = new TranslationKey;
					trans_key->start_time = 0.0f;
					trans_key->time_to_next_key = 0.0f;
					trans_key->translation = gef::Vector2(0, 0);
					bKey->translation_keys.push_back(trans_key);
				}

				//If the bone key has a rotation frame get that
				if (anim_bones_array[it].HasMember("rotateFrame"))
				{
					const rapidjson::Value& anim_bones_rot_array = ske_document["armature"][0]["animation"][anim_num]["bone"][it]["rotateFrame"];

					//For each of the bone keys rotation frames
					for (int it3 = 0; it3 < anim_bones_rot_array.Size(); ++it3)
					{
						RotationKey* rot_key = anim_pars->ReadRotationKeyDataFromJSON(anim_bones_rot_array[it3]);
						//Adds the rotation keys to the bone
						bKey->rotation_keys.push_back(rot_key);
					}
				}
				else //If no rotation frames were found
				{
					//Set them default values
					RotationKey* rot_key = new RotationKey;
					rot_key->rotation = 0.0f;
					rot_key->start_time = 0.0f;
					rot_key->time_to_next_key = 0.0f;
					bKey->rotation_keys.push_back(rot_key);
				}

				//Add the bone key to the animation
				anim->bone_keys.push_back(bKey);
			}
			//Push the animation to the map
			anims.insert(std::make_pair(anim->name, *anim));

			delete anim;
		}
	}

	return anims;
}

std::map<std::string, Bone> Skeletal_Sprite_anim::ReadBonesFromJSON(rapidjson::Document& ske_document)
{
	//Reads the bones from the ske document
	const rapidjson::Value& bones_array = ske_document["armature"][0]["bone"];

	std::map<std::string, Bone> bones;

	for (int bone_num = 0; bone_num < (int)bones_array.Size(); ++bone_num)
	{
		//Set each of the bones variable in accordance to the file
		Bone* bone = new Bone();
		bone->name_ = bones_array[bone_num]["name"].GetString();

		if (bones_array[bone_num].HasMember("parent"))
		{
			bone->parent_name_ = bones_array[bone_num]["parent"].GetString();
		}
		else
		{
			//If no value was found give it a default one
			bone->parent_name_ = "none";
		}

		if (bones_array[bone_num].HasMember("transform"))
		{
			bone->x_ = bones_array[bone_num]["transform"]["x"].GetFloat();
			bone->y_ = bones_array[bone_num]["transform"]["y"].GetFloat();
			bone->rot_ = bones_array[bone_num]["transform"]["skY"].GetFloat();
		}
		else
		{
			bone->x_ = 0.f;
			bone->y_ = 0.f;
			bone->rot_ = 0.f;

		}

		//Build the bones local transform
		bone->BuildLocalTransform();
		//Insert the bone into the mape
		bones.insert(std::make_pair(bone->name_, *bone));

		delete bone;
	}

	//Set the parent of each bone
	SetParentsOfBones(&bones);

	return bones;
}

std::vector<std::string> Skeletal_Sprite_anim::ReadInOrder(rapidjson::Document& ske_document)
{
	const rapidjson::Value& slots_array = ske_document["armature"][0]["slot"];

	std::vector<std::string> newVec;
	//For each of the slots
	for (int slot_num = 0; slot_num < (int)slots_array.Size(); ++slot_num)
	{
		//add the name of it to the vector in the correct order of drawing
		newVec.push_back(slots_array[slot_num]["name"].GetString());
	}

	return newVec;
}

void Skeletal_Sprite_anim::SetParentsOfBones(std::map<std::string, Bone>* bones)
{
	for (auto bone_ : *bones)
	{
		//If the bone has a parent
		if (bone_.second.parent_name_ != "none");
		{
			std::string parent_name, name;
			parent_name = bone_.second.parent_name_;
			name = bone_.first;
			for (auto i : *bones)
			{
				//If the bones parent is found
				if (i.first == parent_name)
				{
					//Set that bone to be the parent of the correct bone
					bones->at(name).parent_ = &bones->at(i.first);
				}
			}
		}
	}

	for (auto bone_ : *bones)
	{
		if (bone_.second.parent_name_ != "none")
		{
			//Set the world transform of the bones with parents to be based off their parents transfor
			bones->at(bone_.first).world_transform_m = bones->at(bone_.first).local_transform_m * bones->at(bone_.first).parent_->local_transform_m;
		}
		else
		{
			bones->at(bone_.first).world_transform_m = bones->at(bone_.first).local_transform_m;
		}
	}
}

void Skeletal_Sprite_anim::CalculateWorldBoneTransform(Animation* anim, int current_frame)
{
	if (anim != NULL)
	{
		//Set intial start time to 0
		float start_time_T = 0.f;

		float start_time_R = 0.f;

		//Loop through each of the bone keys
		for (int i = 0; i < anim->bone_keys.size(); i++)
		{
			//check if bone has translation or rotation

			for (int j = 0; j < anim->bone_keys[i]->translation_keys.size(); j++)
			{
				//set each bone 
				anim->bone_keys[i]->translation_keys[j]->start_time = start_time_T;
				start_time_T += anim->bone_keys[i]->translation_keys[j]->time_to_next_key;
			}

			start_time_T = 0.0f;


			for (int l = 0; l < anim->bone_keys[i]->rotation_keys.size(); l++)
			{
				//set each bone 
				anim->bone_keys[i]->rotation_keys[l]->start_time = start_time_R;
				start_time_R += anim->bone_keys[i]->rotation_keys[l]->time_to_next_key;
			}

			start_time_R = 0.0f;
		}
	}

	for (auto& bone : bones_)
	{

		gef::Matrix33 translation, rotation;
		translation.SetIdentity();
		rotation.SetIdentity();

		float localX = bone.second.x_;
		float localY = bone.second.y_;
		float localRot = bone.second.rot_;

		rotation.Rotate(gef::DegToRad(localRot));
		translation.SetTranslation(gef::Vector2(localX, localY));
		bone.second.local_transform_m = rotation * translation;

		if (anim != NULL)
		{
			//loop through all bone keys in the animatiom
			for (auto& bKey : anim->bone_keys)
			{
				// If bone name mathces bone key name
				if (bone.second.name_ == bKey->name)
				{
					//calculate rotation from rotation keys
					for (int currKey = 0; currKey < bKey->rotation_keys.size(); currKey++)
					{
						//loop through all rotation keys
						int nextKey = currKey >= bKey->rotation_keys.size() - 1 ? 0 : currKey + 1;
						if (current_frame >= bKey->rotation_keys[currKey]->start_time && current_frame < bKey->rotation_keys[nextKey]->start_time)
						{
							//Get lerp between rotation data
							float time = (current_frame - bKey->rotation_keys[currKey]->start_time) / (bKey->rotation_keys[nextKey]->start_time - bKey->rotation_keys[currKey]->start_time);
							localRot += (anim_pars->lerp(bKey->rotation_keys[currKey]->rotation, bKey->rotation_keys[nextKey]->rotation, time));
						}
						//End of loop
						else if (currKey == nextKey)
						{
							localRot += bKey->rotation_keys[currKey]->rotation;
						}
					}

					//Calculate translation from translation keys
					for (int currKey = 0; currKey < bKey->translation_keys.size(); currKey++)
					{
						//loop through all rotation keys
						int nextKey = currKey >= bKey->translation_keys.size() - 1 ? 0 : currKey + 1;
						if (current_frame >= bKey->translation_keys[currKey]->start_time && current_frame < bKey->translation_keys[nextKey]->start_time)
						{
							//Get lerp between translation key data
							float time = (current_frame - bKey->translation_keys[currKey]->start_time) / (bKey->translation_keys[nextKey]->start_time - bKey->translation_keys[currKey]->start_time);
							gef::Vector2 lerp_anim_pos;
							lerp_anim_pos.x = anim_pars->lerp(bKey->translation_keys[currKey]->translation.x, bKey->translation_keys[nextKey]->translation.x, time);
							lerp_anim_pos.y = anim_pars->lerp(bKey->translation_keys[currKey]->translation.y, bKey->translation_keys[nextKey]->translation.y, time);
							localX += lerp_anim_pos.x;
							localY += lerp_anim_pos.y;
						}
						//end of loop
						else if (currKey == nextKey)
						{
							localX += bKey->translation_keys[currKey]->translation.x;
							localY += bKey->translation_keys[currKey]->translation.y;
						}
					}

					break;

				}
			}
		}
		//Set the bones local transform and rotation values
		rotation.Rotate(gef::DegToRad(localRot));
		translation.SetTranslation(gef::Vector2(localX, localY));
		bone.second.New_local_transform_m = rotation * translation;

	}

	for (auto& bone : bones_)
	{
		for (auto& child_bone : bones_)
		{
			//If the bone has a child
			if (bone.second.name_ == child_bone.second.parent_name_)
			{
				//Update the bones child based off its new transform
				child_bone.second.world_transform_m = child_bone.second.New_local_transform_m * bone.second.world_transform_m;
			}
		}
	}

}

void Skeletal_Sprite_anim::Update(int frame, gef::Sprite* sprite_, gef::Vector2 position_, std::map<std::string, gef::Matrix33>& Transforms_for_bone_)
{
	//Calculate the bone transforms based of the animation and the frame
	CalculateWorldBoneTransform(&new_anim.at(WhichAnim_), frame);
}

gef::Sprite* Skeletal_Sprite_anim::Render(gef::Sprite* sprite, gef::Matrix33& transform, std::string part, gef::Vector2 Position)
{
	//Set the transform for the current bone being rendered
	gef::Matrix33 sprite_offset_transform_m;
	gef::Matrix33 world_bone_transforming_m;
	gef::Matrix33 sub_texture_transform_m;
	gef::Matrix33 local_home_transform_m;

	sprite_offset_transform_m = skin_slots.at(part).transform_m_;
	world_bone_transforming_m = bones_.at(part).world_transform_m;
	std::string part_name = skin_slots.at(part).part_name_;

	sub_texture_transform_m = text_atlas->subtex_atlas.at(part_name).transform_m_;

	local_home_transform_m = bones_.at(part).local_transform_m;
	gef::Matrix33 Result = sub_texture_transform_m * sprite_offset_transform_m * world_bone_transforming_m * rig_transform_m_;
	transform = Result;

	//Sets the position,rotation and size of the bone sprite being rendered
	SetSpriteSizeAndPositionForFrame(sprite, Position.x, Position.y, text_atlas, part_name);

	return sprite;
}

gef::Sprite* Skeletal_Sprite_anim::SetupAnimation(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts, std::string* WhichAnim1, float scale_)
{
	//If the character has a specific animation to play
	if (WhichAnim1 != NULL)
	{
		//Sets it to the same name
		WhichAnim_ = *WhichAnim1;
	}

	//Gets the correct order of the parts
	std::vector<std::string> new_parts = ReadInOrder(ske_document);

	auto it = std::next(new_parts.begin(), new_parts.size());
	std::move(new_parts.begin(), it, std::back_inserter(bone_parts_));

	//Sets them to the bone parts
	bone_parts = bone_parts_;

	new_parts.erase(new_parts.begin(),it);

	scale = scale_;
	//Sets up the rig and its transform
	SetupRig(&rig_transform_m_, Position, scale);

	anim_pars = new Animation_Parser;

	//Reads all the appropriate date
	skin_slots = ReadSkinSlotsDataFromJSON(ske_document);
	bones_ = ReadBonesFromJSON(ske_document);

	new_anim = ReadAnimationDataFromJSON(ske_document);

	text_atlas = ReadTextureAtlasFromJSON(tex_document);

	return sprite_;
}
