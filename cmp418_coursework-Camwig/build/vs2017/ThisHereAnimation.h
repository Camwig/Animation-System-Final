#pragma once
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <graphics/primitive.h>
#include <graphics/scene.h>

#include <graphics/skinned_mesh_instance.h>

#include "ModelMesh.h"
#include "motion_clip_player.h"
//#include "AnimatedModel.h"

#include "BlendTree.h"
#include "ClipNode.h"
#include "Linear2Blend.h"

#include <maths/math_utils.h>

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

struct AnimatedModel_new
{
	std::string Model_Name_;
	char* Model_PathWay_;

	//Should be a map of Anims
	//Just need to be able to access its Animation varibale
	std::map<std::string, PlayableAnim_new> Anim_map;
};

class ThisHereAnimation
{
public:
	ThisHereAnimation();
	~ThisHereAnimation();

	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform&);

	//void Setup(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string model_name);
	//void InitBlendTree(std::string Anim1_name_, std::string Anim2_name_);
	void Update(float frameTime_, BlendTree& blend_tree,gef::SkinnedMeshInstance& player_, gef::SkeletonPose& blended_pose, float speed_);

	void SetAnimation(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string tex_name, std::string anim_name, gef::Skeleton& skeleton_, gef::SkinnedMeshInstance* player_, AnimatedModel_new& anim_model, float speed_);

	void CallAnimationSetup(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name, gef::Skeleton& skeleton_, AnimatedModel_new& anim_model, gef::SkinnedMeshInstance& player_, float speed_);

	//class gef::Mesh* mesh_;
	//gef::SkinnedMeshInstance* player_;

	//MotionClipPlayer anim_player_;

	//AnimatedModel* anim_model_;

	//gef::Animation* walk_anim_;
	//MotionClipPlayer walk_anim_player;

	//gef::Animation* run_anim_;
	//MotionClipPlayer run_anim_player;

	//gef::Animation* idle_anim_;
	//MotionClipPlayer idle_anim_player;

	//gef::SkeletonPose blended_pose;

	//gef::Skeleton* skeleton;

	//BlendTree* blend_tree_;

	float min_walk_speed;
	float max_walk_speed;
	float walk_speed = 1.f;

	float min_run_speed;
	float max_run_speed;
	float run_speed = 4.f;;

	//float anim_blend;

	//float speed_;

	//Can use maps to store multiple of these
};

