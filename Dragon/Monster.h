#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Monster
*	描　　述：游戏逻辑：怪物类
*
*/
class Monster : public GameSpirit
{
private:
	Model *m_model;
	AnimationModelDrawer *m_drawer;

public:
	Monster(void)
	{
		m_model = MeshManager::LoadModel("E:\\GameDevelop\\Model\\Monster_Animation\\monster.FBX");
		m_drawer = AnimationModelDrawer::Create(m_model, &m_transform);
	}

	void Move()
	{

	}
};