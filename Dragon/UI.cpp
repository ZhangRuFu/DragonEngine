#include "UI.h"
#include "UIDrawer.h"
#include <GLM\gtx\transform.hpp>

View::View(const string &id, vec2 position, int width, int height) : m_id(id), m_position(position)
{
	m_width = width;
	m_height = height;
	m_mouseListener = nullptr;
	m_keyListener = nullptr;
}

bool View::DispatchEvent(Event & ievent)
{
	//View的事件分发
	//是否在此View区域
	int startX = m_fatherPosition.x + m_position.x;
	int endX = startX + m_width;
	int startY = m_fatherPosition.y + m_position.y;
	int endY = startY + m_height;
	if (ievent.m_mousePosition.x < startX || ievent.m_mousePosition.x > endX ||
		ievent.m_mousePosition.y < startY || ievent.m_mousePosition.y > endY)
		//不在View区域
		return false;

	//在View区域
	if (m_mouseListener != nullptr)
		m_mouseListener->onMouse(*this, ievent);
	return true;
}




//====================================================ViewGroup===========================================
bool ViewGroup::DispatchEvent(Event & ievent)
{
	//ViewGroup的事件分发
	int startX = m_fatherPosition.x + m_position.x;
	int endX = startX + m_width;
	int startY = m_fatherPosition.y + m_position.y;
	int endY = startY + m_height;
	if (ievent.m_mousePosition.x < startX || ievent.m_mousePosition.x > endX ||
		ievent.m_mousePosition.y < startY || ievent.m_mousePosition.y > endY)
		return false;

	bool isFind = false;
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		if (isFind = (*i)->DispatchEvent(ievent))
			break;

	if (!isFind && m_mouseListener != nullptr)
		m_mouseListener->onMouse(*this, ievent);

	return true;
}

void ViewGroup::AddView(View &view)
{
	m_viewList.push_back(&view);
	view.RePosit(m_fatherPosition + m_position);
	view.ReSize(m_width, m_height);
}

View* ViewGroup::FindViewByID(string id)
{
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		if ((*i)->GetViewID() == id)
			return (*i);
	return nullptr;
}

UIManager::UIManager(int screenWidth, int screenHeight) : ViewGroup("RootView", vec2(0, 0), screenWidth, screenHeight)
{
	s_instance = this;
	m_fatherPosition = vec2(0, 0);
}

void UIManager::AcceptEvent(ivec2 mousePosition, MouseMotion mouseMotion)
{
	Event e;
	e.m_mousePosition = mousePosition;
	e.m_mouseMotion = mouseMotion;
	DispatchEvent(e);
}

void UIManager::AcceptEvent(int keyCode, KeyMotion keyMotion)
{
	Event e;
	e.m_keyCode = keyCode;
	e.m_keyMotion = keyMotion;
}

glm::mat4 UIManager::GenProjection(void)
{
	return glm::ortho(0.0f, (float)s_instance->m_width, (float)-s_instance->m_height, 0.0f);
}

UIManager *UIManager::s_instance = nullptr;

Button::Button(const string & id, vec2 position, int width, int height) : View(id, position, width, height)
{
	m_state = ButtonState::Normal;
	m_btnDrawer = ButtonDrawer::Create(this);
}

bool Button::DispatchEvent(Event & ievent)
{
	bool isCapture = View::DispatchEvent(ievent);
	if (!isCapture)
	{
		m_state = ButtonState::Normal;
		return false;
	}
	if (ievent.m_mouseMotion == MouseMotion::MouseMove)
		m_state = ButtonState::Focus;
	else if (ievent.isMouseKeyDown())
		m_state = ButtonState::Clicked;
	else if(ievent.isMouseKeyUp())
		m_state = ButtonState::Normal;
	return true;

}

TextView::TextView(const string & id, vec2 position, int width, int height) : View(id, position, width, height)
{
	//m_textDrawer = new TextDrawer()
}
