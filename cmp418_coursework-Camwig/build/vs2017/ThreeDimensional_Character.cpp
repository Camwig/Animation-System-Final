#include "ThreeDimensional_Character.h"

ThreeDimensional_Character::ThreeDimensional_Character():	
	mesh_(NULL),
	player_(NULL),
	skeleton(NULL),
	anim_(NULL),
	curr_pose(NULL)
{
	scale = 0.0f;
	speed_ = 0.0f;
}


ThreeDimensional_Character::~ThreeDimensional_Character()
{
	delete player_;
	player_ = NULL;

	delete mesh_;
	mesh_ = NULL;

	delete anim_;
	anim_ = NULL;

	delete curr_pose;
	curr_pose = NULL;
}

void ThreeDimensional_Character::Setup(gef::Scene& Model_scene, gef::Platform* platform_, std::string model_name,int Skeleton_num,float scale_, gef::Vector4 Rotation,gef::Vector4 Translation)
{
	//Sets the scale of this character
	scale = scale_;
	//Set the model name and the name of its file
	model_name_ = model_name;
	std::string model_scene_name = model_name + "/" + model_name + ".scn";

	model_mesh_ = new ModelMesh();

	//Sets up the given model scene and its materials
	Model_scene.ReadSceneFromFile(*platform_, model_scene_name.c_str());
	Model_scene.CreateMaterials(*platform_);

	//Initialise a new instantiation of Anim_manager
	anim_ = new Anim_manager;

	//Set the mesh
	mesh_ = model_mesh_->CreateMeshData(&Model_scene, *platform_);

	//Set the skeleton
	skeleton = GetFirstSkeleton(&Model_scene);

	if (skeleton)
	{
		//Set the playes mesh and skeleton
		player_ = new gef::SkinnedMeshInstance(*skeleton);
		player_->set_mesh(mesh_);
		player_->UpdateBoneMatrices(player_->bind_pose());

		// output skeleton joint names
		for (int joint_num = 0; joint_num < skeleton->joints().size(); ++joint_num)
		{
			std::string bone_name;
			Model_scene.string_id_table.Find(skeleton->joint(joint_num).name_id, bone_name);
		}

		// animated model is scaled down to match the size of the physics ragdoll
		gef::Matrix44 player_transform;
		//Sets the players scale and transform
		player_transform.Scale(gef::Vector4(scale, scale, scale));
		player_->set_transform(player_transform);
	}

	//If player is not null
	if (player_)
	{
		//Set the players transform, rotation and scale
		gef::Matrix44 player_transform;
		gef::Matrix44 player_scale;
		gef::Matrix44 player_rotate;
		gef::Matrix44 player_translate;

		player_transform.SetIdentity();
		player_scale.SetIdentity();
		player_rotate.SetIdentity();
		player_translate.SetIdentity();

		player_scale.Scale(gef::Vector4(scale, scale, scale, 1.0f));

		//Checks if the rotational value is not zero
		if (Rotation.x() != 0.0f)
		{
			//If it is not set the new rotation value
			player_rotate.RotationX(gef::DegToRad(Rotation.x()));
		}

		if (Rotation.y() != 0.0f)
		{
			player_rotate.RotationY(gef::DegToRad(Rotation.y()));
		}

		if (Rotation.z() != 0.0f)
		{
			player_rotate.RotationZ(gef::DegToRad(Rotation.z()));
		}

		//Set the new translation for the player
		player_translate.SetTranslation(gef::Vector4(Translation.x(), Translation.y(), Translation.z(), Translation.z()/*25.0f, -25.0f, -100.0f, 1.0f*/));

		//Set the players new transform
		player_transform = player_scale * player_rotate * player_translate;

		player_->set_transform(player_transform);
	}

	//Set the anim models name and file pathway

	char* model_file_path_ = new char[model_scene_name.length()];

	strcpy(model_file_path_, model_scene_name.c_str());

	anim_model_.Model_Name_ = model_name_;
	anim_model_.Model_PathWay_ = model_file_path_;
}

void ThreeDimensional_Character::Init(gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name)
{
	//Call the animations setup
	anim_->SetupAnim(model_mesh_, Model_scene, platform_,anim_name,*skeleton,player_,anim_model_, speed_);
}

void ThreeDimensional_Character::TreeUpdate(float frametime, std::string tree_name)
{
	//Update the blend tree animation
	anim_->Update(frametime,Map_o_blendtrees_.at(tree_name),*player_,blended_pose,speed_);
	//Set the current pose to that of the blended pose
	curr_pose = &blended_pose;
}

void ThreeDimensional_Character::AnimUpdate(float frametime, std::string anim_name)
{
	//Updates the given animation
	anim_->Update(frametime,*player_,anim_model_,anim_name);
	//Sets the current pose to that of the animations that is currently playing
	curr_pose = &anim_model_.Anim_map.at(anim_name).Anim_player_.pose();
}

void ThreeDimensional_Character::AddBlendTree(std::string tree_name)
{
	//Create a new Blend tree and add it to the map of blend trees
	BlendTree* temp_tree = new BlendTree(player_->bind_pose());
	Map_o_blendtrees_.insert(std::make_pair(tree_name,*temp_tree));
	
	delete temp_tree;
	temp_tree = NULL;
}

void ThreeDimensional_Character::InitBlendTree(std::string tree_name, std::string Anim1_name_, std::string Anim2_name_)
{
	if (player_ && player_->bind_pose().skeleton())
	{
		//Intialise the given blend tree with the players current pose
		Map_o_blendtrees_.at(tree_name).Init(player_->bind_pose());

		//Set the two clips of the animations given
		ClipNode* clip_1 = new ClipNode(&Map_o_blendtrees_.at(tree_name));
		clip_1->SetClip(anim_model_.Anim_map.at(Anim2_name_.c_str()).Anim_);

		ClipNode* clip_2 = new ClipNode(&Map_o_blendtrees_.at(tree_name));
		clip_2->SetClip(anim_model_.Anim_map.at(Anim1_name_.c_str()).Anim_);

		//Setup the blend between these two clips
		Linear2Blend* l2b = new Linear2Blend(&Map_o_blendtrees_.at(tree_name));
		//Set the blends speed variable
		l2b->SetVariable(0, "speed");
		//Intialise the speed variable
		Map_o_blendtrees_.at(tree_name).variables["speed"] = speed_;

		//Set the blends inputs to be the two clips
		l2b->SetInput(0, clip_1);
		l2b->SetInput(1, clip_2);

		//Give the outputs input the blend value
		Map_o_blendtrees_.at(tree_name).output_.SetInput(0, l2b);

		//Start the blend tree
		Map_o_blendtrees_.at(tree_name).Start();
	}
}

gef::Skeleton* ThreeDimensional_Character::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}

void ThreeDimensional_Character::UpdateCurrentPoseBoneMatrices_()
{
	//Updates the bone matrices based of the current pose
	player_->UpdateBoneMatrices(*curr_pose);
}

void ThreeDimensional_Character::SetupRagdoll(bool& is_ragdoll_simulating, btDiscreteDynamicsWorld* dynamics_world_)
{
	//Sets up the ragdoll animation
	anim_->SetupAnim(dynamics_world_, anim_model_.Model_Name_, is_ragdoll_simulating, player_);
}

void ThreeDimensional_Character::CallUpdateRagdoll(bool is_ragdoll_simulating_)
{
	//Calls the ragdolls animation update
	anim_->Update(is_ragdoll_simulating_, player_, curr_pose);
}

//Getters and setters for certain variables

float ThreeDimensional_Character::GetAnimMinSpeed(std::string anim_name)
{
	return anim_model_.Anim_map.at(anim_name).Anim_min_speed_;
}
void ThreeDimensional_Character::SetAnimMinSpeed(std::string anim_name,float new_speed)
{
	anim_model_.Anim_map.at(anim_name).Anim_min_speed_ = new_speed;
}

float ThreeDimensional_Character::GetAnimMaxSpeed(std::string anim_name)
{
	return anim_model_.Anim_map.at(anim_name).Anim_max_speed_;
}
void ThreeDimensional_Character::SetAnimMaxSpeed(std::string anim_name, float new_speed)
{
	anim_model_.Anim_map.at(anim_name).Anim_max_speed_ = new_speed;
}

gef::SkinnedMeshInstance* ThreeDimensional_Character::GetPlayer()
{
	return player_;
}

float ThreeDimensional_Character::GetSpeed()
{
	return speed_;
}
void ThreeDimensional_Character::SetSpeed(float new_speed)
{
	speed_ = new_speed;
}