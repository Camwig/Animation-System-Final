#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include <vector>
#include "primitive_builder.h"
#include "primitive_renderer.h"
#include "gef_debug_drawer.h"
#include "ThreeDimensional_Character.h"
#include "TwoDimensional_Character.h"
#include "btBulletDynamicsCommon.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class Scene;
	class Skeleton;
	class InputManager;
	class Animation;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();

private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();

	//Setup and update functions for the scene
	void SetupLights();
	void SetupCamera();

	void InitPhysicsWorld();
	void CleanUpPhysicsWorld();
	void UpdatePhysicsWorld(float delta_time);

	void CreateRigidBodies();
	void CleanUpRigidBodies();
	void UpdateRigidBodies();

	//Declarations of variables needed for running the 3D scene
	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3d_;
	gef::InputManager* input_manager_;
	gef::Font* font_;

	float fps_;

	gef::Scene* model_scene_;

	//Camera variable declarations
	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	//Dynamic rigid body scene variables for the floor and the sphere
	PrimitiveBuilder* primitive_builder_;
	PrimitiveRenderer* primitive_renderer_;

	btDiscreteDynamicsWorld* dynamics_world_;
	btSequentialImpulseConstraintSolver* solver_;
	btBroadphaseInterface* overlapping_pair_cache_;
	btCollisionDispatcher* dispatcher_;
	btAlignedObjectArray<btCollisionShape*> collision_shapes_;
	GEFDebugDrawer* debug_drawer_;

	gef::Mesh* floor_mesh_;
	gef::MeshInstance floor_gfx_;

	btRigidBody* sphere_rb_;
	gef::Mesh* sphere_mesh_;
	gef::MeshInstance sphere_gfx_;

	//Declaration for character variables
	ThreeDimensional_Character* character_;

	TwoDimensional_Character* Sprite_character_;
	TwoDimensional_Character* Sprite_character_2_;

	std::string Model_name_;
	std::string sprite_name_;
	std::string sprite_name_2_;

	const char* AnimToLoad3 = "xbot/xbot@idle.scn";
	const char* AnimToLoad = "ybot/ybot@walking_inplace.scn";
	const char* tree_name_ = "Idle_to_walk";

	std::string sprite_anim;
	std::string sprite_anim_2_;

	bool is_ragdoll_simulating_;
};

#endif