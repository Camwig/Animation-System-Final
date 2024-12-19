#pragma once
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <graphics/primitive.h>
#include <graphics/scene.h>
#include <graphics/skinned_mesh_instance.h>

#include "ModelMesh.h"
#include "motion_clip_player.h"

#include "BlendTree.h"
#include "ClipNode.h"
#include "Linear2Blend.h"

#include <maths/math_utils.h>

//Struct to define the playableAnim
struct PlayableAnim_new
{
	std::string Anim_Name_;
	std::string Anim_Pathway_;

	float Anim_min_speed_;
	float Anim_max_speed_;
	float Anim_speed;

	gef::Animation* Anim_;
	MotionClipPlayer Anim_player_;
};

//Struct to define the AnimatedModel
struct AnimatedModel_new
{
	std::string Model_Name_;
	char* Model_PathWay_;

	std::map<std::string, PlayableAnim_new> Anim_map;
};

class ThreeDimensional_Animation
{
public:
	ThreeDimensional_Animation();
	~ThreeDimensional_Animation();

	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform&);
	//Update to update a blend tree animation and regular animation are simply overloaded for the update
	void Update(float frameTime_, BlendTree& blend_tree,gef::SkinnedMeshInstance& player_, gef::SkeletonPose& blended_pose, float speed_);
	void Update(float frameTime_, gef::SkinnedMeshInstance& player_, AnimatedModel_new& anim_model, std::string anim_name);
	void SetAnimation(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string tex_name, std::string anim_name, gef::Skeleton& skeleton_, gef::SkinnedMeshInstance* player_, AnimatedModel_new& anim_model, float speed_);
	void CallAnimationSetup(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name, gef::Skeleton& skeleton_, AnimatedModel_new& anim_model, gef::SkinnedMeshInstance& player_, float speed_);
};

