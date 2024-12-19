#include "ClipNode.h"
#include "BlendTree.h"

ClipNode::ClipNode(BlendTree* _tree) :
	BlendNode(_tree)
{
	//Intitialise the clip player 
	if (tree_)
		clip_player_.Init(tree_->bind_pose);
	//Set the clip to loop
	clip_player_.set_looping(true);
}

void ClipNode::SetClip(const gef::Animation* anim)
{
	//Set the clip to the given animation
	clip_player_.set_clip(anim);
}

void ClipNode::StartInternal()
{
	//Initialise the animation time
	clip_player_.set_anim_time(0.f);
}

bool ClipNode::UpdateInternal(float deltaTime)
{
	bool valid = false;
	//if the clip player has a clip
	if (clip_player_.clip())
	{
		//Update the clip player
		clip_player_.Update(deltaTime, tree_->bind_pose);
		//Set the output pose to the clip player pose
		OutputPose_ = clip_player_.pose();
		valid = true;
	}
	return valid;
}