#include "BlendNode.h"
#include "BlendTree.h"

BlendNode::BlendNode(BlendTree* _tree) :
	tree_(_tree)
{
	if (tree_)
		OutputPose_ = tree_->bind_pose;
}

bool BlendNode::Update(float deltaTime)
{
	// initialise check that all inputs are valid
	bool all_inputs_valid = true;
	for (int input_num = 0; input_num < inputs_.size(); ++input_num)
	{
		// recursively call BlendNode::Update to perform depth-first traversal
		bool input_valid = false;
		if (inputs_[input_num].node != nullptr)
			input_valid = inputs_[input_num].node->Update(deltaTime);
		// check the output of each input node is valid
		if (!input_valid)
			all_inputs_valid = false;
	}
	// post-order processing starts here
	bool output_valid = false;
	// only process this node is all inputs are valid
	if (all_inputs_valid)
		output_valid = UpdateInternal(deltaTime);
	return output_valid;
}

void BlendNode::SetInput(int InputNum, BlendNode* BlendNode)
{
	//SetInput will assign an existing BlendNode to a
	//particular input for this node.
	if (BlendNode && InputNum < inputs_.size())
	{
		inputs_[InputNum].node = BlendNode;
	}
}

void BlendNode::Start()
{
	//Loops through every input node 
	for (int input_num = 0; input_num < inputs_.size(); input_num++)
	{
		//Starts the node intialisation
		BlendNodeInput& input = inputs_[input_num];
		if (input.node)
			input.node->Start();
	}
	StartInternal();
}

void BlendNode::SetVariable(int variable_num , const std::string& variable)
{
	//Loops through the variable vector and adds the variable to the vector
	if (variable.size() > 0 && variable_num < variables_.size())
	{
		variables_[variable_num] = variable;
	}
}

