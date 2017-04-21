#include "Activity.h"

Activity::Activity(string id) : ViewGroup(id, ivec2(0, 0), View::Dimension::MATCH_PARENT, View::Dimension::MATCH_PARENT)
{

}

void Activity::AcceptEvent(ivec2 mousePosition, MouseMotion mouseMotion)
{
	Event e;
	e.m_mousePosition = mousePosition;
	e.m_mouseMotion = mouseMotion;
	DispatchEvent(e);
}

void Activity::AcceptEvent(int keyCode, KeyMotion keyMotion)
{
	Event e;
	e.m_keyCode = keyCode;
	e.m_keyMotion = keyMotion;
	DispatchEvent(e);
}

DragonActivity::DragonActivity(string id) : Activity(id)
{
	m_texView = new TextView("texView", ivec2(10, 10), "DragonEngine-v0.1");
	m_btn = new Button("btn", ivec2(10, 50), "SelefResize-Button");
	//m_clipBar = new ClipBar("clpBar", 100, ivec2(10, 100));
	AddView(m_texView);
	AddView(m_btn);
	//AddView(m_clipBar);
}
