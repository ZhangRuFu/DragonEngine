#pragma once
#include "Entity.h"
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "RenderSystem.h"
#include "MeshManager.h"

/*
*	ÒýÇæ°æ±¾£ºDragon Engine v0.1;
*	Àà¡¡¡¡Ãû£ºMonster
*	Ãè¡¡¡¡Êö£ºÓÎÏ·Âß¼­£º¹ÖÎïÀà
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