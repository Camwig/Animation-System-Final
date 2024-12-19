#pragma once
#include "Sprite_anim.h"
#include <Animation_Parser.h>

//Struct to hold the definition of skin slots
struct SkinSlot
{
	std::string name_;
	std::string part_name_;
	float x_;
	float y_;
	float rot_;

	gef::Matrix33 transform_m_;

	//Function to Build the transform of the skin slot
	void BuildTransform()
	{
		gef::Matrix33 rotation_m;
		gef::Matrix33 translation_m;

		rotation_m.SetIdentity();
		translation_m.SetIdentity();
		transform_m_.SetIdentity();

		rotation_m.Rotate(gef::DegToRad(rot_));
		translation_m.SetTranslation(gef::Vector2(x_, y_));

		transform_m_ = rotation_m * translation_m;
	}
};

//Struct to hold the definition of Bones
struct Bone
{
	Bone* parent_;
	std::string name_;
	std::string parent_name_;

	float x_;
	float y_;
	float rot_;

	gef::Matrix33 local_transform_m;
	gef::Matrix33 New_local_transform_m;
	gef::Matrix33 world_transform_m;

	//Function to Build the transform of the Bones
	void BuildLocalTransform()
	{
		gef::Matrix33 rotation_m;
		gef::Matrix33 translation_m;

		rotation_m.SetIdentity();
		translation_m.SetIdentity();
		local_transform_m.SetIdentity();

		rotation_m.Rotate(gef::DegToRad(rot_));
		translation_m.SetTranslation(gef::Vector2(x_, y_));

		local_transform_m = rotation_m * translation_m;
	}
};

class Skeletal_Sprite_anim : public Sprite_anim
{
public:
	Skeletal_Sprite_anim();
	~Skeletal_Sprite_anim();

	//Functions to setup the bones,skinslots and animation of the skeletal sprite
	std::map<std::string, SkinSlot> ReadSkinSlotsDataFromJSON(rapidjson::Document&);
	std::map<std::string, Bone> ReadBonesFromJSON(rapidjson::Document&);
	void SetParentsOfBones(std::map<std::string, Bone>*);
	void CalculateWorldBoneTransform(Animation*, int);
	std::map<std::string, Animation> ReadAnimationDataFromJSON(rapidjson::Document&);

	std::vector<std::string> ReadInOrder(rapidjson::Document&);

	virtual void Update(int frame, gef::Sprite* sprite_, gef::Vector2 position_, std::map<std::string, gef::Matrix33>& Transforms_for_bone_) override;
	virtual gef::Sprite* SetupAnimation(gef::Platform*, gef::Sprite*, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts, std::string* WhichAnim1, float scale_) override;
	gef::Sprite* Render(gef::Sprite* sprite, gef::Matrix33& transform, std::string part, gef::Vector2 Position) override;

private:
	TextureAtlas* text_atlas;
	//Map of animations
	std::map<std::string, Animation> new_anim;
	//Animation name to be played
	std::string WhichAnim_;

	Animation_Parser* anim_pars;
	gef::Matrix33 rig_transform_m_;
	gef::Matrix33 combined_transform_m_;
	std::map<std::string, SkinSlot> skin_slots;
	std::map<std::string, Bone> bones_;
	std::vector<std::string> bone_parts_;
};

