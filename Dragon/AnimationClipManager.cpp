#include "AnimationClipManager.h"

void AnimationClipManager::ExportXML(string path)
{
	//导出片段数据到XML文件
}

void AnimationClipManager::ImportXML(string path)
{
	//从XML文件导入片段数据
}

AnimationClip * AnimationClipManager::GetAnimationClip(string clipName)
{
	for (vector<AnimationClip*>::const_iterator i = m_clips.cbegin(); i != m_clips.cend(); i++)
		if ((*i)->m_clipName == clipName)
			return (*i);
	return nullptr;
}
