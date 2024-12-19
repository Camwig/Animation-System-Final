#include "Linear2Blend.h"
#include "BlendTree.h"

Linear2Blend::Linear2Blend(BlendTree* _tree)
	:BlendNode(_tree)
{
	//Set the inputs and variables size
	inputs_.resize(2);
	variables_.resize(1);
}

bool Linear2Blend::UpdateInternal(float deltaTime)
{
	//Set the blend value to be that of the first variable (Which will be the speed)
	float blend_value = tree_->variables[variables_[0]];
	//Set the output pose to be the blend of the two input animations
	OutputPose_.Linear2PoseBlend(inputs_[0].node->OutputPose_, inputs_[1].node->OutputPose_, blend_value);

	return true;
}
