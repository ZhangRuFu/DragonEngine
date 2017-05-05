#include "AnimationController.h"
#include "ResourceSystem.h"
#include "Time.h"

AnimationController::AnimationController(AnimationModelDrawer * drawer, SkeletonModel * model)
{
	m_drawer = drawer;
	m_model = model;
	m_spf = 1.0 / 30 * 1000;		//ms
	m_curTime = 0;
	m_boneManager = m_model->GetBoneManager();
	m_boneTransform.resize(m_boneManager->GetBoneCount());
	m_drawer->UpdateBoneTransform(&m_boneTransform);
	m_curKey = 0;
	m_keyCount = model->GetAnimationKeyCount();
	m_isPlay = false;
	ResourceSystem::Register(this);
}

void AnimationController::Move(void)
{
	//每帧，计算骨骼Transform，交给Drawer
	if (!m_isPlay)
		return;
	m_curTime += Time::GetDeltaTime();
	if (m_curTime < m_spf)
		return;
	m_curTime = 0;
	m_curKey++;
	if (m_curKey > m_keyCount)
		m_curKey -= m_keyCount;

	m_boneManager->UpdateSkeleton(m_curKey, m_boneTransform);
}
