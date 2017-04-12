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
	view.Active();
	m_viewList.push_back(&view);
	view.ReFatherPosition(m_fatherPosition + m_position);
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

Button::Button(const string & id, vec2 position, int width, int height, string text) : View(id, position, width, height), m_text(nullptr)
{
	m_state = ButtonState::Normal;
	m_text = new TextView(id, position, text, 16);
	//计算TextView位置
	ivec2 dimension = m_text->GetDimension();
	//默认居中
	ivec2 texPosition;
	int offsetX = m_width - dimension.x;
	int offsetY = m_height - dimension.y;
	texPosition.x = (offsetX > 0 ? offsetX : 0) / 2.0 + m_position.x;
	texPosition.y = (offsetY > 0 ? offsetY : 0) / 2.0 + m_position.y;
	m_text->RePosition(texPosition);
}

Button::Button(const string & id, vec2 position, string text) : View(id, position, 0, 0), m_text(nullptr)
{
	m_state = ButtonState::Normal;
	m_text = new TextView(id, position, text, 16);
	
	ivec2 dimension = m_text->GetDimension();
	//Button宽高
	m_width = dimension.x + 2 * m_lrOffset;
	m_height = dimension.y + 2 * m_ubOffset;

	//计算TextView位置
	//默认居中
	ivec2 texPosition;
	int offsetX = m_width - dimension.x;
	int offsetY = m_height - dimension.y;
	texPosition.x = (offsetX > 0 ? offsetX : 0) / 2.0 + m_position.x;
	texPosition.y = (offsetY > 0 ? offsetY : 0) / 2.0 + m_position.y;
	m_text->RePosition(texPosition);
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

void Button::Active(void)
{
	ButtonDrawer::Create(this, m_text);
}

TextView::TextView(const string &id, vec2 position, string string, int fontSize, vec3 color) : View(id, position, 0, 0), m_str(string)
{
	int width, height;
	FontRender::GetDimension(m_str, width, height);
	ReSize(width, height);
	m_fontSize = fontSize;
}

void TextView::Active(void)
{
	TextViewDrawer::Create(this);
}
