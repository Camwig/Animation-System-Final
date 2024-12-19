#pragma once

#include "Sheet_Sprite_anim.h"
#include "Skeletal_Sprite_anim.h"
#include "ThreeDimensional_Animation.h"
#include "ragdoll.h"

namespace gef
{
	class Platform;
}

class Anim_manager
{
public:
	Anim_manager();
	~Anim_manager();

	//The 2D animation uses the same kind animation but blend trees, ragdolls and standard 3D animation all update diffrently so overloaded functions
	//were utilised.

	//2D Animation
	void Update(int frame_, gef::Sprite* sprite_,gef::Vector2 position_, std::map<std::string, gef::Matrix33>& Transforms_for_bone_);
	//BlendTree 3D Animation
	void Update(float frameTime_, BlendTree& blend_tree, gef::SkinnedMeshInstance& player_, gef::SkeletonPose& blended_pose, float speed_);
	//Standard 3D Animation
	void Update(float frameTime_, gef::SkinnedMeshInstance& player_, AnimatedModel_new& anim_model, std::string anim_name);
	//Ragdoll 3D Animation
	void Update(bool is_ragdoll_simulating_, gef::SkinnedMeshInstance* player_, const gef::SkeletonPose* curr_pose);

	//2D Animation
	void SetupAnim(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts,std::string& type_,std::string* WhichAnim,float scale_);
	//3D Animation (Both Blendtress and standard 3D animation use the same Setup)
	void SetupAnim(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name, gef::Skeleton& skeleton_, gef::SkinnedMeshInstance* player_, AnimatedModel_new& anim_model, float speed_);
	//Ragdoll 3D Animation
	void SetupAnim(btDiscreteDynamicsWorld* dynamics_world_, std::string model_name, bool& is_ragdoll_simulating_, gef::SkinnedMeshInstance* player_);

	//2D Sprite Render which is needed as Sprite animation needs to perform extra steps before rendering to screen
	void Sprite_Render(gef::Sprite* sprite_, gef::Matrix33& transform, std::string part, gef::Vector2 Position);

	//Getters and setters for private functions

	float GetFrameRate();
	void SetFrameRate(float new_framerate_);

	float GetDuration();
	void SetDuration(float new_duration_);

	std::string* GetWhichAnim();
	void SetWhichAnim(std::string new_WhichAnim_);

private:
	Sprite_anim* sprite_animation_;
	ThreeDimensional_Animation* ThreeD_animation_;
	Ragdoll* ragdoll_;

	float FrameRate;
	float Duration;
	std::string* WhichAnim_;
};

