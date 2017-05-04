#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：AnimationModelDrawer
*	描　　述：继承自Drawer，适用于骨骼动画的渲染器
*
*/
#include "Drawer.h"
#include "Transform.h"
#include <map>

class SkeletonModel;
class Model;

using std::map;

class AnimationModelDrawer : public Drawer
{
private:
	SkeletonModel *m_model;
	Transform *m_transform;
	vector<mat4> *m_boneTransform;

	static map<SkeletonModel*, GraphicsBuffer*> m_buffersMap;

protected:
	AnimationModelDrawer(Model *model, Transform *transform, string shaderName = "AnimationModel");

public:
	void UpdateBoneTransform(vector<mat4> *boneTransform) { m_boneTransform = boneTransform; }

	virtual void Draw(void);
	virtual void PublicSet(void);
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::Entity; }

	static AnimationModelDrawer* Create(Model *model, Transform *transform);
	static GraphicsBuffer* LoadGraphicsBuffer(SkeletonModel *model);
};