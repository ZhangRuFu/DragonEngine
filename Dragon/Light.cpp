#include "Light.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "CommonType.h"

Light::Light(Transform &transform, vec3 color) : m_color(color), GameSpirit(transform)
{
	m_mesh = MeshManager::LoadModel(BasicMesh::CUBE);
	m_drawer = SimpleModelDrawer::Create(m_mesh, &m_transform, m_color);
	m_isShow = true;
	ResourceSystem::Register(this);
}

vec3 Light::GetLightColor(void)
{
	return m_color;
}

void Light::CloseShow(void)
{
	m_isShow = false;
}

void Light::Move(void)
{
	if (InputSystem::isKeyDown(KEY_RIGHT))
		m_transform.Move(vec3(2, 0, 0));
	if (InputSystem::isKeyDown(KEY_UP))
		m_transform.Move(vec3(0, 2, 0));
}