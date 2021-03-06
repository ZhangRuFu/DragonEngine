#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"
#include "AnimationController.h"

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Monster
*	描　　述：游戏逻辑：怪物类
*
*/
class Monster : public GameSpirit
{
private:
	SkeletonModel *m_model;
	AnimationModelDrawer *m_drawer;
	AnimationController *m_animationController;

public:
	Monster(void)
	{
		m_model = dynamic_cast<SkeletonModel*>(MeshManager::LoadModel("E:\\GameDevelop\\Model\\Monster_Animation\\monster.FBX"));
		m_drawer = AnimationModelDrawer::Create(m_model, &m_transform);
		m_animationController = new AnimationController(m_drawer, m_model);
	}

	void Move()
	{

	}
};