#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"

/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Monster
*	�衡��������Ϸ�߼���������
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
		m_drawer = new AnimationModelDrawer(m_model, &m_transform);
	}

	void Move()
	{

	}
};