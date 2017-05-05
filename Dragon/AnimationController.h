#pragma once
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "AnimationClipManager.h"

class AnimationController
{
	//动画状态机信息
	SkeletonModel *m_model;
	AnimationModelDrawer *m_drawer;
	AnimationClipManager *m_clipManager;	//片段管理器
	AnimationClip *m_currentClip;			//当前动画片段
	BoneManager *m_boneManager;				//根骨骼
	vector<mat4> m_boneTransform;

	float m_spf;							//每帧所需时间
	float m_curTime;						//当前帧积累时间
	int m_keyCount;							//动画帧总数
	float m_curKey;							//当前帧
	bool m_isPlay;							//是否播放

public:
	AnimationController(AnimationModelDrawer *drawer, SkeletonModel *model);
	void Pause(void) { m_isPlay = false; }
	void Play(void) { m_isPlay = true; }
	void Move(void);
};