#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

struct AnimationClip
{
	string m_clipName;
	float m_startKey;
	float m_endKey;
};

class AnimationClipManager
{
public:
	void AddClip(AnimationClip *clip) { m_clips.push_back(clip); };
	void ExportXML(string path);
	void ImportXML(string path);
	AnimationClip* GetAnimationClip(string clipName);
	int GetClipCount(void) { return m_clips.size(); }

private:
	vector<AnimationClip*> m_clips;
};
