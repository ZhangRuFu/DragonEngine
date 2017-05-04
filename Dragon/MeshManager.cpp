#include "MeshManager.h"
#include "MeshLoaderFactory.h"
#include "Model.h"

MeshManager::MeshManager(void)
{
	
	CreateBasicMesh();
}

Model * MeshManager::_LoadModel(string meshPath)
{
	//防止重复加载
	if (m_meshMap.find(meshPath) != m_meshMap.end())
		return m_meshes[m_meshMap[meshPath]];

	//加载模型
	MeshLoader *meshLoader = MeshLoaderFactory::GetMeshLoader(meshPath);
	//======================================Loader唯一可以避免重复加载、释放===================================
	Model *model = meshLoader->LoadModel();
	delete meshLoader;
	m_meshMap[meshPath] = m_meshes.size();
	m_meshes.push_back(model);
	return model;
}

Model * MeshManager::_LoadModel(BasicMesh basicMesh)
{
	Model *model = new StaticModel(*dynamic_cast<StaticModel*>(m_basicMeshes[basicMesh]));
	return model;
}

Model * MeshManager::LoadModel(string meshPath)
{
	return m_instance->_LoadModel(meshPath);
}

Model * MeshManager::LoadModel(BasicMesh basicMesh)
{
	return m_instance->_LoadModel(basicMesh);
}

MeshManager * MeshManager::GetInstance(void)
{
	if (m_instance == nullptr)
		m_instance = new MeshManager();
	return m_instance;
}

void MeshManager::CreateBasicMesh(void)
{
	m_basicMeshes.resize(BasicMesh::COUNT);
	float cubeVertices[] = {
		// Positions          // Normals           // Texture Coords
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
	};
	int rectFaceIndex[] = { 0, 1, 2, 2, 3, 0 };
	int cubeVertexGroupCount = sizeof(cubeVertices) / (8 * 4);
	vector<Vertex> *cubeVertex = new vector<Vertex>(cubeVertexGroupCount);
	for (int i = 0; i < cubeVertexGroupCount; i++)
	{
		Vertex vertex;
		for (int j = 0; j < 3; j++)
			vertex.m_position[j] = cubeVertices[i * 8 + j];
		for (int j = 0; j < 3; j++)
			vertex.m_normal[j] = cubeVertices[i * 8 + 3 + j];
		for (int j = 0; j < 2; j++)
			vertex.m_texCoord[j] = cubeVertices[i * 8 + 6 + j];
		(*cubeVertex)[i] = (vertex);
	}
	vector<unsigned int> cubeIndex;
	for (int i = 0; i < 6; i++)
		for (int j = 0;j < 6; j++)
			cubeIndex.push_back(i * 4 + rectFaceIndex[j]);
	StaticMesh *cubeSubMesh = new StaticMesh(cubeVertex, &vector<Texture*>(), &cubeIndex);
	Model *cubeMesh = new StaticModel(vector<StaticMesh*>(1, cubeSubMesh), vector<Texture*>());
	m_basicMeshes[BasicMesh::CUBE] = cubeMesh;

	float planeVertices[] = {
		// Positions			//Normal			// Texture Coords
		0.5f,  0.0f, 0.5f,		0.0f, 1.0f,	0.0f,	5.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 5.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	5.0f, 5.0f,
	};
	int planeVertexGroupCount = sizeof(planeVertices) / (8 * 4);
	vector<Vertex> *planeVertex = new vector<Vertex>(planeVertexGroupCount);
	for (int i = 0; i < planeVertexGroupCount; i++)
	{
		Vertex vertex;
		for (int j = 0; j < 3; j++)
			vertex.m_position[j] = planeVertices[i * 8 + j];
		for (int j = 0; j < 3; j++)
			vertex.m_normal[j] = planeVertices[i * 8 + 3 + j];
		for (int j = 0; j < 2; j++)
			vertex.m_texCoord[j] = planeVertices[i * 8 + 6 + j];
		(*planeVertex)[i] = (vertex);
	}
	vector<unsigned int> planeIndex;
	for (int i = 0; i < 6; i++)
		planeIndex.push_back(rectFaceIndex[i]);
	StaticMesh *planeSubMesh = new StaticMesh(planeVertex, &vector<Texture*>(), &planeIndex);
	Model *planeMesh = new StaticModel(vector<StaticMesh*>(1, planeSubMesh), vector<Texture*>());
	m_basicMeshes[BasicMesh::PLANE] = planeMesh;
}

MeshManager * MeshManager::m_instance = nullptr;