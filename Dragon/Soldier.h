#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "ModelDrawer.h"
#include "RenderSystem.h"

/*
*	ÒýÇæ°æ±¾£ºDragon Engine v0.1;
*	Àà¡¡¡¡Ãû£ºSoldier
*	Ãè¡¡¡¡Êö£ºÓÎÏ·Âß¼­£ºÊ¿±øÀà
*
*/
class Soldier : public GameSpirit
{
private:
	Model *m_mesh;
	ModelDrawer *m_drawer;

public:
	Soldier(void)
	{
		m_mesh = MeshManager::LoadModel("E:\\GameDevelop\\Model\\nanosuit\\nanosuit.obj");
		m_drawer = new ModelDrawer(m_mesh, &m_transform);
	}

	void Move()
	{

	}
};