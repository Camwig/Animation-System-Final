#pragma once
#include "BlendNode.h"
#include "motion_clip_player.h"

class ClipNode : public BlendNode
{
public:
	ClipNode(BlendTree* _tree);
	void SetClip(const gef::Animation* anim);
	void StartInternal() override;
	bool UpdateInternal(float deltaTime) override;

	MotionClipPlayer clip_player_;
};

