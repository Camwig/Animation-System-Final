#include "ThisHereAnimation.h"

ThisHereAnimation::ThisHereAnimation()
{
}

ThisHereAnimation::~ThisHereAnimation()
{
}

gef::Animation* ThisHereAnimation::LoadAnimation(const char* anim_scene_filename, const char* anim_name, gef::Platform& platform_)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform_, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}

//IN CHARACTER!!!
//void ThisHereAnimation::Setup(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_,std::string model_name)
//{
//	blend_tree_ = new BlendTree;
//
//	// if there is mesh data in the scene, create a mesh to draw from the first mesh
//	mesh_ = ModelMesh_->CreateMeshData(Model_scene, *platform_);//GetFirstMesh(model_scene_);
//
//	// get the first skeleton in the scene
//	skeleton = ModelMesh_->CreateSkeleton(Model_scene);//GetFirstSkeleton(model_scene_);
//
//	if (skeleton)
//	{
//		player_ = new gef::SkinnedMeshInstance(*skeleton);
//		blended_pose = player_->bind_pose();
//		player_->set_mesh(mesh_);
//	}
//
//	speed_ = 0.0f;
//
//	anim_blend = 0.f;
//
//	char* model_file_path_ = new char[model_name.length()];
//
//	strcpy(model_file_path_, model_name.c_str());
//
//	anim_model_->Model_Name_ = model_name;
//	anim_model_->Model_PathWay_ = model_file_path_;
//}

void ThisHereAnimation::SetAnimation(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string tex_name,std::string anim_name,gef::Skeleton& skeleton_,gef::SkinnedMeshInstance* player_, AnimatedModel_new& anim_model,float speed_)
{
	MotionClipPlayer* new_anim_player_ = new MotionClipPlayer;
	gef::Animation* new_anim = new gef::Animation;

	if (&skeleton_)
	{
		new_anim_player_->Init(player_->bind_pose());
	}

	new_anim = LoadAnimation(tex_name.c_str(),"",*platform_);

	if (new_anim)
	{
		new_anim_player_->set_clip(new_anim);
		new_anim_player_->set_looping(true);
		new_anim_player_->set_anim_time(0.0f);
	}

	float min_speed = 1.f;
	float max_speed = 1.f;

	//ALL OF THIS STUFF SHOULD BE CHANGED TO HOLD THE PLAYABLE ANIM STUFF HERE

	//PlayableAnim* play_anim_ = new PlayableAnim;

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

	//This should then be given to the Character

	//Need to change the kind of anim_model that it takes in
	anim_model.Anim_map.insert(std::make_pair(play_anim_.Anim_Name_, play_anim_));

	//delete play_anim_;
	//play_anim_ = NULL;
}

void ThisHereAnimation::CallAnimationSetup(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name, gef::Skeleton& skeleton_, AnimatedModel_new& anim_model, gef::SkinnedMeshInstance& player_, float speed_)
{
	SetAnimation(ModelMesh_, Model_scene, platform_, anim_name, anim_name, skeleton_, &player_,anim_model,speed_);
}


void ThisHereAnimation::Update(float frameTime_, BlendTree& blend_tree,gef::SkinnedMeshInstance& player_, gef::SkeletonPose& blended_pose, float speed_)
{
	//IN CHARACTER!!!---------------------
	if (&player_)
	{
		//blend_tree_->variables[""]

		//Need to fix this so it does not just do the default instead the default of the blend tree
		blend_tree.variables["idle_anim_"] = speed_;


		blend_tree.Update(frameTime_);
		blended_pose = blend_tree.output_.OutputPose_;

		player_.UpdateBoneMatrices(blended_pose);
	}
	//IN CHARACTER!!!---------------------

	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (&player_)
	{
		gef::Matrix44 player_transform;
		gef::Matrix44 player_scale;
		gef::Matrix44 player_rotate;
		gef::Matrix44 player_translate;

		player_transform.SetIdentity();
		player_scale.SetIdentity();
		player_rotate.SetIdentity();
		player_translate.SetIdentity();

		player_scale.Scale(gef::Vector4(0.3f, 0.3f, 0.3f, 1.0f));
		player_rotate.RotationY(gef::DegToRad(45.0f));
		player_translate.SetTranslation(gef::Vector4(25.0f, -25.0f, -100.0f, 1.0f));

		player_transform = player_scale * player_rotate * player_translate;

		player_.set_transform(player_transform);
	}
}

//IN CHARACTER!!!
//void ThisHereAnimation::InitBlendTree(std::string Anim1_name_, std::string Anim2_name_)
//{
//	if (player_ && player_->bind_pose().skeleton())
//	{
//		blend_tree_->Init(player_->bind_pose());
//
//		ClipNode* idle_clip = new ClipNode(blend_tree_);
//		idle_clip->SetClip(anim_model_->Anim_map.at(Anim2_name_.c_str()).Anim_);
//
//		ClipNode* walk_clip = new ClipNode(blend_tree_);
//		walk_clip->SetClip(anim_model_->Anim_map.at(Anim1_name_.c_str()).Anim_);
//
//		Linear2Blend* l2b = new Linear2Blend(blend_tree_);
//		l2b->SetVariable(0, "idle_anim_");
//
//		l2b->SetInput(0, idle_clip);
//		l2b->SetInput(1, walk_clip);
//
//		blend_tree_->output_.SetInput(0, l2b);
//
//		blend_tree_->Start();
//	}
//}
