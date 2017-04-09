#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：AnimationModelDrawer
*	描　　述：继承自Drawer，适用于骨骼动画的渲染器
*
*/
#include "Drawer.h"
#include "Model.h"
#include "Transform.h"

class AnimationModelDrawer : public Drawer
{
private:
	SkeletonModel *m_model;
	Transform *m_transform;

	static map<SkeletonModel*, GraphicsBuffer*> m_buffersMap;

protected:
	AnimationModelDrawer(Model *model, Transform *transform, string shaderName = "AnimationModel") : Drawer(shaderName)
	{
		m_model = dynamic_cast<SkeletonModel*>(model);
		m_transform = transform;
		m_buffers = LoadGraphicsBuffer(m_model);
	}

public:
	virtual void Draw(void);
	virtual void PublicSet(void);
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::Entity; }

	static AnimationModelDrawer* Create(Model *model, Transform *transform);
	static GraphicsBuffer* LoadGraphicsBuffer(SkeletonModel *model);
};