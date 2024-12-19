#pragma once


#include "Sheet_Sprite_anim.h"
#include "Skeletal_Sprite_anim.h"
#include "ThisHereAnimation.h"
#include "ragdoll.h"
//#include <graphics/sprite.h>

//#include "rapidjson\document.h"
//#include "rapidjson/istreamwrapper.h"

namespace gef
{
	class Platform;
}

class Anim
{
public:
	Anim();
	~Anim();

	//void Init(std::string, gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string, int frame);
	
	//TextureAtlas* ReadTextureAtlasFromJSON(rapidjson::Document& tex_document);
	//TexData* ReadSubtextureFromJSON(const rapidjson::Value&);
	//void SetSpriteSizeAndPositionForFrame(gef::Sprite*, float, float, int);

	//void Load_sprite_and_texture_(gef::Platform*, gef::Sprite*);
	//void Load_sprite_and_texture_2(gef::Platform*, gef::Sprite*, std::string tex_string);
	//TextureAtlas* text_atlas;

	void Update(int frame_, gef::Sprite* sprite_,gef::Vector2 position_, std::map<std::string, gef::Matrix33>& Transforms_for_bone_);
	void Update(float frameTime_, BlendTree& blend_tree, gef::SkinnedMeshInstance& player_, gef::SkeletonPose& blended_pose, float speed_);
	void Update(bool is_ragdoll_simulating_, gef::SkinnedMeshInstance* player_, MotionClipPlayer* Anim_player_);

	//void Setup_2D_anim(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string);
	//void SetupAnimSheet2D(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document);

	void SetupAnim(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string, rapidjson::Document& tex_document, rapidjson::Document& ske_document, gef::Vector2 Position, std::vector<std::string>& bone_parts,std::string& type_,std::string* WhichAnim);
	/*ModelMesh *,                        gef::Scene *,            gef::Platform *,          std::string,           gef::Skeleton,            AnimatedModel, gef::SkinnedMeshInstance, float)*/
	void SetupAnim(ModelMesh* ModelMesh_, gef::Scene* Model_scene, gef::Platform* platform_, std::string anim_name, gef::Skeleton& skeleton_, gef::SkinnedMeshInstance* player_, AnimatedModel_new& anim_model, float speed_);
	void SetupAnim(btDiscreteDynamicsWorld* dynamics_world_, std::string model_name, bool& is_ragdoll_simulating_, gef::SkinnedMeshInstance* player_);


	void Render(gef::Sprite* sprite_, gef::Matrix33& transform, std::string part, gef::Vector2 Position);

	float FrameRate;
	float Duration;
	std::string* WhichAnim_;
private:
	//gef::Texture* sprite_texture_;
	Sprite_anim* sprite_animation_;
	ThisHereAnimation* ThreeD_animation_;
	Ragdoll* ragdoll_;
	gef::Matrix33* Sprite_transform_;
	std::vector<std::string>* Bone_parts;
	//void Setup_2D_anim(gef::Platform* platform_, gef::Sprite* sprite_, std::string tex_string);
	//int frame_;

	//gef::Sprite* sprite;
};

