#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：SimpleModelDrawer
*	描　　述：继承自Drawer，适用于简单纯色模型的渲染器
*
*/
#include <map>
#include "Drawer.h"
#include "Model.h"
#include "Transform.h"
#include "CommonType.h"
using std::map;

class SimpleModelDrawer : public Drawer
{
private:
	StaticModel *m_mesh;
	Transform *m_transform;
	vec3 m_color;

private:
	static map<StaticModel*, GraphicsBuffer*> m_buffersMap;

public:
	SimpleModelDrawer(Model *mesh, Transform *transform, vec3 color, string shaderName = "SimpleModel") : Drawer(shaderName)
	{
		m_mesh = dynamic_cast<StaticModel*>(mesh);
		m_transform = transform;
		m_color = color;
		m_buffers = LoadGraphicsBuffer(m_mesh);
	}

public:
	void Draw();

public:
	void PublicSet();

public:
	static GraphicsBuffer* LoadGraphicsBuffer(StaticModel *mesh);
};