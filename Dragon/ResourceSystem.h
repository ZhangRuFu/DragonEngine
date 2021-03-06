#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ResourceSysyem
*	描　　述：管理游戏中的所有资源，包括：GameEntity资源，Mesh资源，Texture资源
*
*/

#include "MeshManager.h"
#include "TextureManager.h"
#include "Entity.h"
#include "Camera.h"
#include "Light.h"
#include <list>

using std::list;

class DragonEngine;
class AnimationController;

class ResourceSystem
{
private:
	static ResourceSystem *m_instance;

private:
	list<GameEntity*> m_entityList;
	list<GameSpirit*> m_spiritList;
	list<Camera*> m_cameraList;
	list<Light*> m_lightList;
	list<AnimationController*> m_animationControllers;
	MeshManager *m_meshManager;
	TextureManager *m_textureManager;
	DragonEngine *m_engine;

private:
	ResourceSystem(DragonEngine *engine);

public:
	static ResourceSystem* GetInstance(DragonEngine *engine);
	static void Register(GameEntity* entity);
	static void Register(GameSpirit *spirit);
	static void Register(Camera *camera);
	static void Register(Light *light);
	static void Register(AnimationController *animationController);
	static Camera* GetMainCamera();
	static Light* GetLight();

public:
	void Move();
	

};