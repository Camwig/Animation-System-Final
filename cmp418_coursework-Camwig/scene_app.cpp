#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/primitive.h>
#include <graphics/font.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/scene.h>
#include <system/debug_log.h>
#include "ragdoll.h"


SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	model_scene_(NULL),
	primitive_builder_(NULL),
	primitive_renderer_(NULL),
	dynamics_world_(NULL),
	solver_(NULL),
	overlapping_pair_cache_(NULL),
	dispatcher_(NULL),
	debug_drawer_(NULL),
	floor_mesh_(NULL),
	sphere_mesh_(NULL),
	sphere_rb_(NULL),
	character_(NULL),
	Sprite_character_(NULL),
	Sprite_character_2_(NULL)
{
}

void SceneApp::Init()
{
	//Create the intitial variables
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	InitFont();
	SetupCamera();
	SetupLights();

	//Declares the name of each animated character
	Model_name_ = "xbot";
	sprite_name_2_ = "boy-attack";	
	sprite_name_ = "Dragon";

	//Sets the name of each of the skeletal animations for the dragon character
	std::string ske_anim_1 = "stand";
	std::string ske_anim_2 = "walk";

	//Sets the position of the character on screen
	gef::Vector2 pos = gef::Vector2(platform_.width() * 0.25f, platform_.height() * 0.5f);

	//Sets up the sprite character
	Sprite_character_ = new TwoDimensional_Character();
	Sprite_character_->LoadCharacter(sprite_name_, &platform_, pos, 0.40f);

	Sprite_character_->SetWhichAnimation(ske_anim_1);
	Sprite_character_->SetAnimation(sprite_name_, &platform_);

	Sprite_character_->SetWhichAnimation(ske_anim_2);
	Sprite_character_->SetAnimation(sprite_name_, &platform_);

	//Declares the sprite animation names
	sprite_anim = Sprite_character_->SetAnimationName(sprite_name_, ske_anim_1);
	sprite_anim_2_ = Sprite_character_->SetAnimationName(sprite_name_, ske_anim_2);

	//Intialise this character at the intial frame zero
	Sprite_character_->Update(sprite_anim_2_, 0);

	//Sets up the second sprite character
	Sprite_character_2_ = new TwoDimensional_Character();
	gef::Vector2 pos_2 = gef::Vector2(platform_.width() * 0.75f, platform_.height() * 0.5f);
	Sprite_character_2_->LoadCharacter(sprite_name_2_, &platform_, pos_2, 0.40f);
	Sprite_character_2_->SetAnimation(sprite_name_2_, &platform_);
	Sprite_character_2_->Update(sprite_name_2_, 0);

	//Intialises the scene
	model_scene_ = new gef::Scene();

	//Intialises the 3D Character
	character_ = new ThreeDimensional_Character();

	//Declares what skeleton in the scene the character should use along with their position and rotation
	int skel_num = 0;
	gef::Vector4 Rot = gef::Vector4(0.0f,0.0f,0.0f,0.0f);
	gef::Vector4 New_pos = gef::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	float scale_ = 0.01f;

	//Sets up the 3D Character
	character_->Setup(*model_scene_, &platform_, Model_name_, skel_num, scale_,Rot,New_pos);

	//Intialise the animations for the 3D Character
	character_->Init(model_scene_, &platform_, AnimToLoad3);
	character_->Init(model_scene_, &platform_, AnimToLoad);

	//Intialises the Blend tree in the character
	character_->AddBlendTree(tree_name_);
	character_->InitBlendTree(tree_name_,AnimToLoad,AnimToLoad3);

	primitive_builder_ = new PrimitiveBuilder(platform_);
	primitive_renderer_ = new PrimitiveRenderer(platform_);

	InitPhysicsWorld();
	CreateRigidBodies();

	//Setup the ragdoll of the 3D Character after the physics of the world have been created
	character_->SetupRagdoll(is_ragdoll_simulating_, dynamics_world_);
}

void SceneApp::CleanUp()
{
	//Cleans up the scene

	CleanUpRigidBodies();

	CleanUpPhysicsWorld();

	CleanUpFont();

	delete primitive_renderer_;
	primitive_renderer_ = NULL;

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete model_scene_;
	model_scene_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;
}

bool SceneApp::Update(float frame_time)
{
	//Sets the fps counter
	fps_ = 1.0f / frame_time;

	// read input devices
	if (input_manager_)
	{
		input_manager_->Update();

		// controller input
		gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		if (controller_manager)
		{
		}

		// keyboard input
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			//Sets if the ragdoll is simulating
			if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
			{
				is_ragdoll_simulating_ = !is_ragdoll_simulating_;
			}

			float multiplier = 1.f;

			if (keyboard->IsKeyDown(keyboard->KC_LSHIFT)) {
				multiplier = 5.f;
			}

			//Checks if the speed is exceeding the walking animation speed and if it is not increase the speed
			if (keyboard->IsKeyDown(keyboard->KC_W)) {
				character_->SetSpeed((character_->GetSpeed() >= character_->GetAnimMinSpeed(AnimToLoad)) ? character_->GetAnimMinSpeed(AnimToLoad) : character_->GetSpeed() + 0.02f * multiplier);
			}

			//Checks if the character speed is at least greater than zero and if it is decrease the speed
			if (keyboard->IsKeyDown(keyboard->KC_S)) {
				character_->SetSpeed((character_->GetSpeed() <= 0) ? 0 : character_->GetSpeed() - 0.02f * multiplier);
			}
		}
	}

	//Update the 2D Characters
	Sprite_character_->UpdateAnimation(frame_time, sprite_anim_2_);
	Sprite_character_2_->UpdateAnimation(frame_time, sprite_name_2_);

	//Update the 3D Character if its player is not null
	if (character_->GetPlayer())
	{
		//Update the blend tree animation
		character_->TreeUpdate(frame_time, tree_name_);
		//Updates the players current poses bone matrices
		character_->UpdateCurrentPoseBoneMatrices_();
	}

	//Update the physics world and rigid bodies
	UpdatePhysicsWorld(frame_time);
	UpdateRigidBodies();

	//Update the ragodll of the 3D Character
	character_->CallUpdateRagdoll(is_ragdoll_simulating_);

	return true;
}

void SceneApp::Render()
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(camera_fov_, (float)platform_.width() / (float)platform_.height(), near_plane_, far_plane_);
	view_matrix.LookAt(camera_eye_, camera_lookat_, camera_up_);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	// draw meshes here
	renderer_3d_->Begin();

	//Draw the 3D Character mesh
	if (character_->GetPlayer())
	{
		renderer_3d_->DrawSkinnedMesh(*character_->GetPlayer(), character_->GetPlayer()->bone_matrices());
	}

	renderer_3d_->DrawMesh(floor_gfx_);
	renderer_3d_->DrawMesh(sphere_gfx_);

	if (dynamics_world_)
		dynamics_world_->debugDrawWorld();

	renderer_3d_->End();

	sprite_renderer_->Begin(false);
	//Draw the 2D Character sprites
	Sprite_character_->RenderAnimation(sprite_anim_2_,sprite_renderer_);
	Sprite_character_2_->RenderAnimation(sprite_name_2_, sprite_renderer_);

	DrawHUD();
	sprite_renderer_->End();
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void SceneApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void SceneApp::SetupCamera()
{
	// initialise the camera settings
	camera_eye_ = gef::Vector4(0.0f, 2.0f, 5.0f);
	camera_lookat_ = gef::Vector4(0.0f, 0.0f, 0.0f);
	camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camera_fov_ = gef::DegToRad(45.0f);
	near_plane_ = 0.01f;
	far_plane_ = 100.0f;
}

void SceneApp::InitPhysicsWorld()
{
	/// collision configuration contains default setup for memory , collision setup . Advanced users can create their own configuration .
	btDefaultCollisionConfiguration* collision_configuration = new btDefaultCollisionConfiguration();

	/// use the default collision dispatcher . For parallel processing you can use a diffent dispatcher(see Extras / BulletMultiThreaded)
	dispatcher_ = new btCollisionDispatcher(collision_configuration);

	/// btDbvtBroadphase is a good general purpose broadphase . You can also try out btAxis3Sweep .
	overlapping_pair_cache_ = new btDbvtBroadphase();

	/// the default constraint solver . For parallel processing you can use a different solver (see Extras / BulletMultiThreaded)
	solver_ = new btSequentialImpulseConstraintSolver;

	dynamics_world_ = new btDiscreteDynamicsWorld(dispatcher_, overlapping_pair_cache_, solver_, collision_configuration);
	dynamics_world_->setGravity(btVector3(0, -9.8f, 0));


	debug_drawer_ = new GEFDebugDrawer(renderer_3d_);
	debug_drawer_->setDebugMode(btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawFrames);
	dynamics_world_->setDebugDrawer(debug_drawer_);
}

void SceneApp::CleanUpPhysicsWorld()
{
	delete debug_drawer_;
	debug_drawer_ = NULL;

	for (int i = dynamics_world_->getNumConstraints() - 1; i >= 0; i--)
	{
		btTypedConstraint* constraint = dynamics_world_->getConstraint(i);
		dynamics_world_->removeConstraint(constraint);
		delete constraint;
	}


	// remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamics_world_->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamics_world_->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamics_world_->removeCollisionObject(obj);
		delete obj;
	}

	// delete collision shapes
	for (int j = 0; j < collision_shapes_.size(); j++)
	{
		btCollisionShape* shape = collision_shapes_[j];
		collision_shapes_[j] = 0;
		delete shape;
	}

	// delete dynamics world
	delete dynamics_world_;

	// delete solver
	delete solver_;

	// delete broadphase
	delete overlapping_pair_cache_;

	// delete dispatcher
	delete dispatcher_;

	dynamics_world_ = NULL;
	solver_ = NULL;
	overlapping_pair_cache_ = NULL;
	dispatcher_ = NULL;

	// next line is optional : it will be cleared by the destructor when the array goes out of scope
	collision_shapes_.clear();
}

void SceneApp::UpdatePhysicsWorld(float delta_time)
{
	const btScalar simulation_time_step = 1.0f / 60.0f;
	const int max_sub_steps = 1;
	dynamics_world_->stepSimulation(simulation_time_step, max_sub_steps);
}

//Can get rid of the sphere that falls!---------------------------------------

void SceneApp::CreateRigidBodies()
{
	//the ground is a cube of side 100 at position y = 0.
	{
		btVector3 groundHalfExtents(btScalar(50.), btScalar(1.), btScalar(50.));
		btCollisionShape* groundShape = new btBoxShape(groundHalfExtents);

		collision_shapes_.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -groundHalfExtents.y(), 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamics_world_->addRigidBody(body);

		floor_mesh_ = primitive_builder_->CreateBoxMesh(gef::Vector4(groundHalfExtents.x(), groundHalfExtents.y(), groundHalfExtents.z()));
		floor_gfx_.set_mesh(floor_mesh_);
		floor_gfx_.set_transform(btTransform2Matrix(groundTransform));
	}

	{
		//create a dynamic rigidbody

		const btScalar  sphereRadius = 1.f;
		btCollisionShape* colShape = new btSphereShape(sphereRadius);

		collision_shapes_.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamics_world_->addRigidBody(body);

		sphere_rb_ = body;
		sphere_mesh_ = primitive_builder_->CreateSphereMesh(sphereRadius, 20, 20);
		sphere_gfx_.set_mesh(sphere_mesh_);
		sphere_gfx_.set_transform(btTransform2Matrix(startTransform));
	}
}

void SceneApp::CleanUpRigidBodies()
{
	delete sphere_mesh_;
	sphere_mesh_ = NULL;
	delete floor_mesh_;
	floor_mesh_ = NULL;
}

void SceneApp::UpdateRigidBodies()
{
	if (sphere_rb_)
	{
		btTransform world_transform;
		sphere_rb_->getMotionState()->getWorldTransform(world_transform);
		sphere_gfx_.set_transform(btTransform2Matrix(world_transform));
	}
}
