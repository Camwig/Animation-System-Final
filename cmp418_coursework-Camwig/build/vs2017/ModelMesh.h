#pragma once
#include <graphics/mesh.h>
#include <graphics/skinned_mesh_instance.h>
#include <graphics/primitive.h>
#include <graphics/scene.h>

class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();
	gef::Mesh* CreateMeshData(gef::Scene*,gef::Platform&);
	gef::Skeleton* CreateSkeleton(gef::Scene* scene,int skeleton_num);
private:
	gef::Skeleton* GetItemFromList(std::list<gef::Skeleton*>,int position);
};

