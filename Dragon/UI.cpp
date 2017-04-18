#include "UI.h"
#include "UIDrawer.h"
#include <GLM\gtx\transform.hpp>

View::View(const string &id, vec2 position, int width, int height) : m_id(id), m_position(position)
{
	using std::numeric_limits;
	m_drawer = nullptr;
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

void View::RegisterDrawer(void)
{
	if(m_drawer != nullptr)
		m_drawer->Register();
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
	view.RegisterDrawer();
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
	m_drawer = new ButtonDrawer(this);
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
	m_drawer = new ButtonDrawer(this);
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


TextView::TextView(const string &id, vec2 position, string string, int fontSize, vec3 color) : View(id, position, 0, 0), m_str(string)
{
	int width, height;
	FontRender::GetDimension(m_str, width, height);
	ReSize(width, height);
	m_fontSize = fontSize;
	m_drawer = new TextViewDrawer(this);
}

ClipBar::ClipBar(string id, float len, vec2 position, int width, int height) : View(id, position, width, height)
{
	m_length = len;
	m_start = 0;
	m_end = m_length - 1;
	char tex[20];
	sprintf(tex, "Length: %.2f", len);
	m_lenText = new TextView(id, vec2(ClipBarMeasure::m_leftOffset, ClipBarMeasure::m_lenToTop), tex, 16);
	m_lenText->ReFatherPosition(ivec2(position.x, position.y));
	sprintf(tex, "Start: %d", 0);
	m_startText = new TextView(id, vec2(ClipBarMeasure::m_leftOffset, m_lenText->GetPositionY() + m_lenText->GetHeight() + ClipBarMeasure::m_startToLenDistence), tex, 16);
	m_startText->ReFatherPosition(ivec2(position.x, position.y));
	sprintf(tex, "End: %.2f", len - 1);
	m_endText = new TextView(id, vec2(0, m_startText->GetPositionY()), tex, 16);
	m_endText->RePositionX(width - m_endText->GetWidth() - ClipBarMeasure::m_rightOffset);
	m_endText->ReFatherPosition(ivec2(position.x, position.y));
	m_startButton = new Button(id, vec2(ClipBarMeasure::m_leftOffset - ClipBarMeasure::m_slideLen / 2.0, height - ClipBarMeasure::m_slideLen - ClipBarMeasure::m_slideToBottom), ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_startButton->ReFatherPosition(ivec2(position.x, position.y));
	m_minPositionX = m_startButton->GetPositionX();
	m_endButton = new Button(id, vec2(ClipBarMeasure::m_leftOffset + ClipBarMeasure::m_axisLen - ClipBarMeasure::m_slideLen / 2.0, m_startButton->GetPositionY()), ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_endButton->ReFatherPosition(ivec2(position.x, position.y));
	m_maxPositionX = m_endButton->GetPositionX();
	ClipButtonListener *listener = new ClipButtonListener(this);
	m_startButton->SetMouseListener(listener);
	m_endButton->SetMouseListener(listener);
	m_drawer = new ClipBarDrawer(this);
}

void ClipBar::SetStartValue(float value)
{
	char str[20];
	sprintf(str, "Start:%.2f", value);
	m_start = value;
	m_startText->SetText(str);
}

void ClipBar::SetEndValue(float value)
{
	char str[20];
	sprintf(str, "End:%.2f", value);
	m_end = value;
	m_endText->SetText(str);
}

bool ClipBar::DispatchEvent(Event & ievent)
{
	if (!View::DispatchEvent(ievent))
		return false;

	//在View区域
	if (m_mouseListener != nullptr)
		m_mouseListener->onMouse(*this, ievent);

	//在本控件区域内
	m_startButton->DispatchEvent(ievent);
	m_endButton->DispatchEvent(ievent);
	return true;
}

ListView::ListView(string id, vec2 position, int width, int height) : View(id, position, width, height)
{
	m_drawer = new ListViewDrawer(this);
}

void ListView::AddItem(ListItem * item)
{
	if (item == nullptr)
		return;
	ivec2 fPosition = GetAbsolutePosition();
	item->ReFatherPosition(fPosition);
	item->ReSize(m_width, 60);
	item->RePosition(ivec2(0, 0));
	m_items.push_back(item);
}

ClipItem::ClipItem(string clipName, float start, float end)
{
	m_clipName = clipName;
	m_start = start;
	m_end = end;

	char str[20];
	m_texClip = new TextView(m_id, vec2(0, 0), clipName, 16);
	m_texClip->ReFatherPosition(m_position);
	sprintf(str, "Start:%f", m_start);
	m_texStart = new TextView(m_id, vec2(0, m_texClip->GetHeight()), str, 16);
	m_texStart->ReFatherPosition(m_position);
	sprintf(str, "Start:%f", m_end);
	m_texEnd = new TextView(m_id, vec2(m_texStart->GetWidth() + 20, m_texClip->GetHeight()), str, 16);
	m_texStart->ReFatherPosition(m_position);
	m_drawer = new ClipItemDrawer(this);
}

ListItem::ListItem(string id, vec2 position, int width, int height) : View(id, position, width, height)
{
}
