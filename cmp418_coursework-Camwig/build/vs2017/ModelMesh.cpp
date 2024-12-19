#include "ModelMesh.h"

ModelMesh::ModelMesh()
{

}

ModelMesh::~ModelMesh()
{

}

gef::Mesh* ModelMesh::CreateMeshData(gef::Scene* scene,gef::Platform& platform_)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		if (scene->mesh_data.size() > 0)
			//Create the mesh from the mesh data within the scene
			mesh = scene->CreateMesh(platform_, scene->mesh_data.front());
	}
	return mesh;
}

gef::Skeleton* ModelMesh::CreateSkeleton(gef::Scene* scene, int skeleton_num)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		if (scene->skeletons.size() > 0)
			//Create the skeleton from one of the skeletons in the scene
			skeleton = GetItemFromList(scene->skeletons,skeleton_num);
	}
	return skeleton;
}

gef::Skeleton* ModelMesh::GetItemFromList(std::list<gef::Skeleton*> List, int position)
{
	//Returns a specific item from a certain position in the given list
	std::list<gef::Skeleton*>::iterator it = List.begin();
	for (int i = 0; i < position; i++) {
	++it;
	}
	return *it;
}