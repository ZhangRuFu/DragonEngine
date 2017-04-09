#pragma once
#include <string>
#include "CommonType.h"

using std::string;

class Shader;
struct GraphicsBuffer;

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Drawer
*	描　　述：所有需要进行绘制的组件必须继承该抽象类
*
*/
class Drawer
{
protected:
	GraphicsBuffer *m_buffers;
	Shader *m_shader;

public:
	Drawer(string shaderName);
	Shader* GetShader();
	virtual void Draw() = 0;
	virtual void PublicSet() = 0;
	virtual RenderLevel GetRenderLevel() = 0;

protected:
	void Register(void);
};
