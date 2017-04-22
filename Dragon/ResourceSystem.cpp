#include "ResourceSystem.h"
#include "DragonEngine.h"
#include <iostream>
using std::cout;
using std::endl;

ResourceSystem::ResourceSystem(DragonEngine *engine)
{
	m_engine = engine;
	m_meshManager = MeshManager::GetInstance();
	m_textureManager = TextureManager::GetInstance();
	if (m_meshManager && m_textureManager)
		cout << "DragonEngine->SUCCESS:资源系统初始化成功!" << endl;
}

ResourceSystem * ResourceSystem::GetInstance(DragonEngine *engine)
{
	if (m_instance == nullptr)
		m_instance = new ResourceSystem(engine);
	return m_instance;
}

void ResourceSystem::Register(GameEntity * entity)
{
	m_instance->m_entityList.push_back(entity);
}

void ResourceSystem::Register(GameSpirit * spirit)
{
	m_instance->m_spiritList.push_back(spirit);
}

void ResourceSystem::Register(Camera * camera)
{
	if (camera != nullptr)
		m_instance->m_cameraList.push_back(camera);
}

void ResourceSystem::Register(Light * light)
{
	if (light != nullptr)
		m_instance->m_lightList.push_back(light);
}

void ResourceSystem::Move()
{
	for (list<GameSpirit*>::iterator i = m_spiritList.begin(); i != m_spiritList.end(); i++)
		(*i)->Move();
	m_meshManager->Move();

}

Camera * ResourceSystem::GetMainCamera()
{
	if (m_instance->m_cameraList.empty())
		return nullptr;
	return *m_instance->m_cameraList.begin();
}

Light * ResourceSystem::GetLight()
{
	return *m_instance->m_lightList.begin();
}

ResourceSystem * ResourceSystem::m_instance = nullptr;