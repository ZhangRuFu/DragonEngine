#pragma once

#include "Entity.h"
#include "Model.h"
#include "ModelDrawer.h"

class WoodPlane : public GameEntity
{
private:
	Model *m_model;
	ModelDrawer *m_drawer;

public:
	WoodPlane(void)
	{
		m_model = MeshManager::LoadModel(BasicMesh::PLANE);
		m_model->AddTexture("E:\\GameDevelop\\Asset\\Texture\\wood2.jpg");
		m_drawer = new ModelDrawer(m_model, &m_transform);
	}
};