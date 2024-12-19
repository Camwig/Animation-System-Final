#pragma once
#include <animation/skeleton.h>
#include <vector>

class BlendNode;
class BlendTree;

//Struct for the BlendNode Input
struct BlendNodeInput
{
	BlendNode* node;

	BlendNodeInput() :
		node(nullptr)
	{

	}
};

class BlendNode
{
public:
	BlendNode(BlendTree* _tree);

	//Vector of inputs
	std::vector<BlendNodeInput>inputs_;
	//Vector of variables
	std::vector<std::string> variables_;
	gef::SkeletonPose OutputPose_;
	//pointer to the train
	BlendTree* tree_;

	bool Update(float deltaTime);
	void SetInput(int InputNum, BlendNode* BlendNode);
	void SetVariable(int variable_num, const std::string& variable);

	//Functions for updating the internal node
	virtual bool UpdateInternal(float deltaTime) = 0;
	virtual void StartInternal() {}

	void Start();
};

