#pragma once
#include "BlendNode.h"

class Linear2Blend : public BlendNode
{
public:
	Linear2Blend(BlendTree* _tree);
	bool UpdateInternal(float deltaTime) override;
};

