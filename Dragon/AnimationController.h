#pragma once
#include "Model.h"
#include "AnimationModelDrawer.h"
#include "AnimationClipManager.h"

class AnimationController
{
	//����״̬����Ϣ
	SkeletonModel *m_model;
	AnimationModelDrawer *m_drawer;
	AnimationClipManager *m_clipManager;	//Ƭ�ι�����
	AnimationClip *m_currentClip;			//��ǰ����Ƭ��
	BoneManager *m_boneManager;				//������
	vector<mat4> m_boneTransform;

	float m_spf;							//ÿ֡����ʱ��
	float m_curTime;						//��ǰ֡����ʱ��
	int m_keyCount;							//����֡����
	float m_curKey;							//��ǰ֡

public:
	AnimationController(AnimationModelDrawer *drawer, SkeletonModel *model);
	void Move(void);
};