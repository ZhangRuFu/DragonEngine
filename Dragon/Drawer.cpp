#include "Drawer.h"
#include "RenderSystem.h"

Drawer::Drawer(string shaderName)
{
	m_shader = RenderSystem::LoadShader(shaderName);
}

void Drawer::Register(void)
{
	RenderSystem::Register(this);
}

Shader * Drawer::GetShader()
{
	return m_shader;
}
