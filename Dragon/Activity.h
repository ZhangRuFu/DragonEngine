#pragma once
#include "UI.h"

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
	Button *m_btn;
	//ClipBar *m_clipBar;

public:
	DragonActivity(string id);
};