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
	m_btnClip = new Button("btnClip", ivec2(10, 50), "AddClip");
	
	m_btnExport = new Button("btnExport", ivec2(10, 100), "Export");
	m_clipBar = new ClipBar("clpBar", 10, ivec2(10, 100));
	m_listView = new ListView("listView", ivec2(300, 10));

	//AddClipListener *addClipListener = new AddClipListener(m_clipBar, m_listView);
	//m_btnClip->SetMouseListener(addClipListener);

	m_listView->AddItem(new ClipItem("Walk", 10, 15));
	m_listView->AddItem(new ClipItem("Run", 5.5, 7.5));
	AddView(m_texView);
	AddView(m_btnClip);
	AddView(m_btnExport);
	AddView(m_clipBar);
	AddView(m_listView);
}

AddClipListener::AddClipListener(ClipBar * clip, ListView *lstView)
{
	m_clip = clip;
	m_lstView = lstView;
}
