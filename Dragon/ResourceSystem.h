#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ResourceSysyem
*	�衡������������Ϸ�е�������Դ��������GameEntity��Դ��Mesh��Դ��Texture��Դ
*
*/

#include "MeshManager.h"
#include "TextureManager.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include <list>

using std::list;

class ResourceSystem
{
private:
	static ResourceSystem *m_instance;

private:
	list<GameEntity*> m_entityList;
	list<GameSpirit*> m_spiritList;
	list<Camera*> m_cameraList;
	list<Light*> m_lightList;
	MeshManager *m_meshManager;
	TextureManager *m_textureManager;

private:
	ResourceSystem();

public:
	static ResourceSystem* GetInstance(void);
	static void Register(GameEntity* entity);
	static void Register(GameSpirit *spirit);
	static void Register(Camera *camera);
	static void Register(Light *light);
	static Camera* GetMainCamera();
	static Light* GetLight();

public:
	void Move();
	

};