#pragma once
#include <string>

using std::string;

class Shader;
struct GraphicsBuffer;

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Drawer
*	�衡������������Ҫ���л��Ƶ��������̳иó�����
*
*/
class Drawer
{
protected:
	GraphicsBuffer *m_buffers;
	Shader *m_shader;

public:
	Drawer(string shaderName);

	virtual void Draw() = 0;
	virtual void PublicSet() = 0;

	Shader* GetShader();
};
