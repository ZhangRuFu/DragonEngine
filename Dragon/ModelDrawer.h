#pragma once

#include <map>

#include "Drawer.h"
#include "Model.h"
#include "Transform.h"
#include "CommonType.h"

using std::map;


/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ModelDrawer
*	描　　述：继承自Drawer，适用于静态模型的渲染器
*
*/
class ModelDrawer : public Drawer
{
private:
	StaticModel *m_mesh;
	Transform *m_transform;

	static map<StaticModel*, GraphicsBuffer*> m_buffersMap;

protected:
	ModelDrawer(Model *mesh, Transform *transform, string shaderName = "AssimpModel") : Drawer(shaderName)
	{
		m_mesh = dynamic_cast<StaticModel*>(mesh);
		m_transform = transform;
		m_buffers = LoadGraphicsBuffer(m_mesh);
	}

public:
	virtual void Draw();
	virtual void PublicSet();
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::Entity; }

	static ModelDrawer* Create(Model *mesh, Transform *transform);
	static GraphicsBuffer* LoadGraphicsBuffer(StaticModel *mesh);		//避免重复给相同Model开辟缓冲对象
};