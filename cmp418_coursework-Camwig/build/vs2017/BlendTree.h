#pragma once
#include <map>
#include <string>
#include <animation/skeleton.h>
#include <vector>
#include "OutputNode.h"

class BlendTree
{
public:
	BlendTree(const gef::SkeletonPose& bind_pose_);
	~BlendTree();
	void Init(const gef::SkeletonPose& pose_);
	bool Update(float deltaTime);
	void Start();

	//Output node
	OutputNode output_;
	//binded pose
	gef::SkeletonPose bind_pose;
	//Map of variables
	std::map<std::string, float> variables;

};

