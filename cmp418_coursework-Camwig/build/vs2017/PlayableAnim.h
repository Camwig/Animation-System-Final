#pragma once
#include <string>
#include "motion_clip_player.h"
class PlayableAnim
{
public:
	std::string Anim_Name_;
	std::string Anim_Pathway_;

	float Anim_min_speed_;
	float Anim_max_speed_;
	float Anim_speed;

	gef::Animation* Anim_;
	MotionClipPlayer Anim_player_;
};

