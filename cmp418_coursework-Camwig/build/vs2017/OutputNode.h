#pragma once
#include "BlendNode.h"

class OutputNode : public BlendNode
{
public:
	OutputNode(BlendTree* _tree);
	bool UpdateInternal(float) override;
};

