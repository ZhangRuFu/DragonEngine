#pragma once
#include "UI.h"
#include <string>


class Tiny2D;

class Activity : public ViewGroup
{
public:
	Activity(string id);
	
	//Override
	void AcceptEvent(ivec2 mousePosition, MouseMotion mouseMotion);
	void AcceptEvent(int keyCode, KeyMotion keyMotion);
};

class DragonActivity : public Activity
{
protected:
	TextView *m_texView;
	Button *m_btnClip;
	Button *m_btnExport;
	ClipBar *m_clipBar;
	ListView *m_listView;

public:
	DragonActivity(string id);
};

class AddClipListener : public MouseListener
{
private:
	ClipBar *m_clip;
	ListView *m_lstView;

public:
	AddClipListener(ClipBar *clip, ListView *lstView);

	virtual bool onMouse(View &view, const Event &e)
	{
		if (e.m_mouseMotion == MouseMotion::LeftButtonDown)
		{
			float start = m_clip->GetStartValue();
			float end = m_clip->GetEndValue();
			ClipItem *item = new ClipItem("T", start, end);
			m_lstView->AddItem(item);
		}
		return true;
	}
};