#include "UI.h"
#include "Tiny2D.h"
#include <GLM\gtx\transform.hpp>

View::View(const string &id, ivec2 &position, int layoutWidth, int layoutHeight) : m_id(id), m_relativePosition(position)
{
	m_layoutWidth = layoutWidth;
	m_layoutHeight = layoutHeight;
	m_width = m_height = 0;
	m_mouseListener = nullptr;
	m_keyListener = nullptr;
	m_parentView = nullptr;
}

bool View::DispatchEvent(Event & ievent)
{
	//View的事件分发
	//是否在此View区域
	int startX = m_fatherPosition.x + m_relativePosition.x;
	int endX = startX + m_width;
	int startY = m_fatherPosition.y + m_relativePosition.y;
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

void View::OnMeasure(int fatherWidth, int fatherHeight)
{
	//宽度
	if (m_layoutWidth == Dimension::MATCH_PARENT)
		m_width = fatherWidth;
	else if (m_layoutWidth >= 0 )
		m_width = m_layoutWidth;
	//高度
	if (m_layoutHeight == Dimension::MATCH_PARENT)
		m_height = fatherHeight;
	else if (m_layoutHeight >= 0)
		m_height = m_layoutHeight;
}

void View::SetParentView(View * parent)
{
	m_parentView = parent;
	SetFatherPosition(m_parentView->GetAbsolutePosition());
}




//====================================================ViewGroup===========================================
bool ViewGroup::DispatchEvent(Event & ievent)
{
	//ViewGroup的事件分发
	int startX = m_fatherPosition.x + m_relativePosition.x;
	int endX = startX + m_width;
	int startY = m_fatherPosition.y + m_relativePosition.y;
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

void ViewGroup::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		(*i)->OnMeasure(m_width, m_height);
}

void ViewGroup::OnDraw(Tiny2D * paint)
{
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		(*i)->OnDraw(paint);
}

void ViewGroup::AddView(View *view)
{
	m_viewList.push_back(view);
	view->SetParentView(this);
}

View* ViewGroup::FindViewByID(string id)
{
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		if ((*i)->GetViewID() == id)
			return (*i);
	return nullptr;
}


TextView::TextView(const string id, ivec2 position, string text, int width, int height, TextAligin texAlign, vec3 color, int fontSize) : View(id, position, width, height), m_text(text)
{
	m_texAlign = texAlign;
	m_fontColor = color;
	m_fontSize = fontSize;
}

void TextView::OnMeasure(int fatherWidth, int fatherHeight)
{
	int texWidth, texHeight;
	FontRender::GetDimension(m_text, texWidth, texHeight);
	View::OnMeasure(fatherWidth, fatherHeight);
	if (m_layoutWidth == View::Dimension::WRAP_CONTENT)
		m_width = texWidth + 2 * LRPadding;
	if (m_layoutHeight == View::Dimension::WRAP_CONTENT)
		m_height = texHeight + 2 * TBPadding;
	//宽高小于父View宽高怎么办？？？？？？
	m_fontPosition.x = (m_width - texWidth) / 2.0;
	m_fontPosition.y = (m_height - texHeight) / 2.0;
}

void TextView::OnDraw(Tiny2D * paint)
{
	ivec2 texPosition = GetAbsolutePosition();
	texPosition.x += m_fontPosition.x;
	texPosition.y += m_fontPosition.y;
	paint->DrawText(m_text, texPosition, m_fontSize, m_fontColor);
}

Button::Button(const string &id, ivec2 position, string text, int width, int height) : TextView(id, position, text, width, height)
{
	m_state = ButtonState::Normal;
}

void Button::OnDraw(Tiny2D * paint)
{
	static glm::vec3 buttonColor = vec3(0.0f, 0.0f, 0.9f);
	static glm::vec3 offset = vec3(0.1f, 0.1f, 0.1f);
	vec3 color = buttonColor;
	if (m_state == ButtonState::Focus)
		color += offset;
	else if (m_state == ButtonState::Clicked)
		color -= offset;

	ivec2 position = GetAbsolutePosition();
	paint->DrawRect(position, m_width, m_height, color);
	TextView::OnDraw(paint);
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



ClipBar::ClipBar(string id, float len, ivec2 position, int width, int height) : View(id, position, width, height)
{
	m_length = len;
	m_start = 0;
	m_end = m_length - 1;
	char tex[20];
	sprintf(tex, "Length: %.2f", len);
	m_lenText = new TextView("", ivec2(ClipBarMeasure::m_leftOffset, ClipBarMeasure::m_lenToTop), tex);
	m_lenText->SetParentView(this);
	sprintf(tex, "Start: %d", 0);
	m_startText = new TextView("", ivec2(ClipBarMeasure::m_leftOffset, m_lenText->GetPositionY() + m_lenText->GetHeight() + ClipBarMeasure::m_startToLenDistence), tex);
	m_startText->SetParentView(this);
	sprintf(tex, "End: %.2f", len - 1);
	m_endText = new TextView(id, ivec2(0, m_startText->GetPositionY()), tex);
	m_endText->SetPositionX(width - m_endText->GetWidth() - ClipBarMeasure::m_rightOffset);
	m_endText->SetParentView(this);
	m_startButton = new Button("ClipBar.StartButton", ivec2(ClipBarMeasure::m_leftOffset - ClipBarMeasure::m_slideLen / 2.0, height - ClipBarMeasure::m_slideLen - ClipBarMeasure::m_slideToBottom), "", ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_startButton->SetParentView(this);
	m_minPositionX = m_startButton->GetPositionX();
	m_endButton = new Button("ClipBar.EndButton", vec2(ClipBarMeasure::m_leftOffset + ClipBarMeasure::m_axisLen - ClipBarMeasure::m_slideLen / 2.0, m_startButton->GetPositionY()), "", ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_endButton->SetParentView(this);
	m_maxPositionX = m_endButton->GetPositionX();
	ClipButtonListener *listener = new ClipButtonListener(this);
	m_startButton->SetMouseListener(listener);
	m_endButton->SetMouseListener(listener);
	m_axisPosition.x = ClipBarMeasure::m_leftOffset;
	m_axisPosition.y = m_startText->GetPositionY() + m_startText->GetHeight() + ClipBarMeasure::m_axisToStart;
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

void ClipBar::OnDraw(Tiny2D * paint)
{
	ivec2 rt = GetAbsolutePosition();
	paint->DrawRoundRect(rt, m_width, m_height, 15, vec3(0.7f, 0.1f, 0.2f));
	rt.x += m_axisPosition.x;
	rt.y += m_axisPosition.y;
	paint->DrawLine(vec2(rt.x, rt.y), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y));
	paint->DrawLine(vec2(rt.x, rt.y - 5), vec2(rt.x, rt.y + 5));
	paint->DrawLine(vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y - 5), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y + 5));
	
	m_startText->OnDraw(paint);
	m_endText->OnDraw(paint);
	m_lenText->OnDraw(paint);
	m_startButton->OnDraw(paint);
	m_endButton->OnDraw(paint);
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

//ListView::ListView(string id, vec2 position, int width, int height) : View(id, position, width, height)
//{
//	
//}
//
//void ListView::AddItem(ListItem * item)
//{
//	if (item == nullptr)
//		return;
//	ivec2 fPosition = GetAbsolutePosition();
//	item->ReFatherPosition(fPosition);
//	item->ReSize(m_width, 60);
//	item->RePosition(ivec2(0, 0));
//	m_items.push_back(item);
//}
//
//ClipItem::ClipItem(string clipName, float start, float end)
//{
//	m_clipName = clipName;
//	m_start = start;
//	m_end = end;
//
//	char str[20];
//	m_texClip = new TextView(m_id, vec2(0, 0), clipName, 16);
//	m_texClip->ReFatherPosition(m_position);
//	sprintf(str, "Start:%f", m_start);
//	m_texStart = new TextView(m_id, vec2(0, m_texClip->GetHeight()), str, 16);
//	m_texStart->ReFatherPosition(m_position);
//	sprintf(str, "Start:%f", m_end);
//	m_texEnd = new TextView(m_id, vec2(m_texStart->GetWidth() + 20, m_texClip->GetHeight()), str, 16);
//	m_texStart->ReFatherPosition(m_position);
//	m_drawer = new ClipItemDrawer(this);
//}
//
//ListItem::ListItem(string id, vec2 position, int width, int height) : View(id, position, width, height)
//{
//}
