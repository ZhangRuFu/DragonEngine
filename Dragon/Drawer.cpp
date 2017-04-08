#include "Drawer.h"
#include "RenderSystem.h"

Drawer::Drawer(string shaderName)
{
	m_shader = RenderSystem::LoadShader(shaderName);
	RenderSystem::RegisterDrawer(this);
}

Shader * Drawer::GetShader()
{
	return m_shader;
}
