#include "BlendTree.h"

BlendTree::BlendTree(const gef::SkeletonPose& bind_pose_) :
	output_(NULL)
{
	variables.clear();
	Init(bind_pose_);

	output_ = this;
}

BlendTree::~BlendTree()
{
}

void BlendTree::Init(const gef::SkeletonPose& bind_pose_)
{
	//Sets the default bind pose
	bind_pose = bind_pose_;
	output_.OutputPose_ = bind_pose;
}

bool BlendTree::Update(float deltaTime)
{
	//Updates the output node
	bool valid = output_.Update(deltaTime);
	return valid;
}

void BlendTree::Start()
{
	//Initilalises the output node
	output_.Start();
}