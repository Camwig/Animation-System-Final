#include "ThreeDimensional_Animation.h"

ThreeDimensional_Animation::ThreeDimensional_Animation()
{
}

ThreeDimensional_Animation::~ThreeDimensional_Animation()
{
}

gef::Animation* ThreeDimensional_Animation::LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform& platform_)
{
	//Loads the animation from file
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	//Read the scene from file
	if (anim_scene.ReadSceneFromFile(platform_, anim_scene_filename))
	{
		//Checks for an animation of that same node
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		//If there was an animation found
		if (anim_node_iter != anim_scene.animations.end())
			//Set the animation to the one found
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}

void ThreeDimensional_Animation::SetAnimation(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string tex_name,std::string anim_name,gef::Skeleton& skeleton_,gef::SkinnedMeshInstance* player_, AnimatedModel_new& anim_model,float speed_)
{
	//Create new instances of the motion clipplayer and animation
	MotionClipPlayer* new_anim_player_ = new MotionClipPlayer;
	gef::Animation* new_anim = new gef::Animation;

	//If the skeleton is not null
	if (&skeleton_)
	{
		//Initialise the motion clip player
		new_anim_player_->Init(player_->bind_pose());
	}

	//Load the animation
	new_anim = LoadAnimation(tex_name.c_str(),"",*platform_);

	//If an animation was found
	if (new_anim)
	{
		//Set the motion clip player to play the animation loaded
		new_anim_player_->set_clip(new_anim);
		new_anim_player_->set_looping(true);
		new_anim_player_->set_anim_time(0.0f);
	}

	//Add the new animation to the anim models animations

	float min_speed = 1.f;
	float max_speed = 1.f;

	PlayableAnim_new play_anim_;

	char* anim_file_path_ = new char[anim_name.length()];

	strcpy(anim_file_path_, anim_name.c_str());

	play_anim_.Anim_Pathway_ = tex_name;
	play_anim_.Anim_Name_ = anim_name;

	play_anim_.Anim_max_speed_ = max_speed;
	play_anim_.Anim_min_speed_ = min_speed;
	play_anim_.Anim_speed = speed_;
	play_anim_.Anim_player_ = *new_anim_player_;
	play_anim_.Anim_ = new_anim;

	anim_model.Anim_map.insert(std::make_pair(play_anim_.Anim_Name_, play_anim_));
}

void ThreeDimensional_Animation::CallAnimationSetup(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name, gef::Skeleton& skeleton_, AnimatedModel_new& anim_model, gef::SkinnedMeshInstance& player_, float speed_)
{
	//Calls the animation setup
	SetAnimation(ModelMesh_, Model_scene, platform_, anim_name, anim_name, skeleton_, &player_,anim_model,speed_);
}

void ThreeDimensional_Animation::Update(float frameTime_, gef::SkinnedMeshInstance& player_, AnimatedModel_new& anim_model,std::string anim_name)
{
	//Updates the given animation
	anim_model.Anim_map.at(anim_name).Anim_player_.Update(frameTime_, player_.bind_pose());
}

void ThreeDimensional_Animation::Update(float frameTime_, BlendTree& blend_tree,gef::SkinnedMeshInstance& player_, gef::SkeletonPose& blended_pose, float speed_)
{
	//If player is not null
	if (&player_)
	{
		blend_tree.variables["speed"] = speed_;
		//Update the blend tree
		blend_tree.Update(frameTime_);
		//Set the blended pose to that of the output pose of the blend tree
		blended_pose = blend_tree.output_.OutputPose_;
		//Update the players bone matrices with the blended pose
		player_.UpdateBoneMatrices(blended_pose);
	}
}
