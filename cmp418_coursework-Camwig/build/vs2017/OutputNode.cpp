#include "OutputNode.h"

OutputNode::OutputNode(BlendTree* _tree) :
	BlendNode(_tree)
{
	//Set the size of the inputs
	inputs_.resize(1);
}

bool OutputNode::UpdateInternal(float deltaTime)
{
	//Set the output pose the input nodes output pose
	OutputPose_ = inputs_[0].node->OutputPose_;
	return true;
}
