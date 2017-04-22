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
	if (!HitView(ievent))
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

void View::OnPosit(int fatherX, int fatherY)
{
	m_fatherPosition.x = fatherX;
	m_fatherPosition.y = fatherY;
}

void View::SetParentView(View * parent)
{
	m_parentView = parent;
	SetFatherPosition(m_parentView->GetAbsolutePosition());
}

bool View::HitView(Event & ievent)
{
	int startX = m_fatherPosition.x + m_relativePosition.x;
	int endX = startX + m_width;
	int startY = m_fatherPosition.y + m_relativePosition.y;
	int endY = startY + m_height;
	if (ievent.m_mousePosition.x < startX || ievent.m_mousePosition.x > endX ||
		ievent.m_mousePosition.y < startY || ievent.m_mousePosition.y > endY)
		//不在View区域
		return false;
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

void ViewGroup::OnPosit(int fatherX, int fatherY)
{
	View::OnPosit(fatherX, fatherY);
	ivec2 aPosition = GetAbsolutePosition();
	for (list<View*>::iterator i = m_viewList.begin(); i != m_viewList.end(); i++)
		(*i)->OnPosit(aPosition.x, aPosition.y);
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
	FontRender::GetDimension(m_text, m_texWidth, m_texHeight);
	View::OnMeasure(fatherWidth, fatherHeight);
	if (m_layoutWidth == View::Dimension::WRAP_CONTENT)
		m_width = m_texWidth;
	if (m_layoutHeight == View::Dimension::WRAP_CONTENT)
		m_height = m_texHeight;
	
}

void TextView::OnPosit(int fatherX, int fatherY)
{
	View::OnPosit(fatherX, fatherY);
	m_fontPosition.x = m_fontPosition.y = 0;
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

void Button::OnPosit(int fatherX, int fatherY)
{
	TextView::OnPosit(fatherX, fatherY);
	m_fontPosition.x = (m_width - m_texWidth) / 2.0;
	m_fontPosition.y = (m_height - m_texHeight) / 2.0;
}

void Button::OnMeasure(int fatherWidth, int fatherHeight)
{
	TextView::OnMeasure(fatherWidth, fatherHeight);
	if (m_layoutWidth == View::Dimension::WRAP_CONTENT)
		m_width += LRPadding * 2;
	if (m_layoutHeight == View::Dimension::WRAP_CONTENT)
		m_height += TBPadding * 2;
	//宽高小于父View宽高怎么办？？？？？？
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
	m_end = m_length;
	m_minPositionX = m_maxPositionX = 0;
	char tex[20];
	sprintf(tex, "Length: %.1f", len);
	m_lenText = new TextView("", ivec2(0, 0), tex);
	m_lenText->SetParentView(this);
	sprintf(tex, "Start: %.1f", m_start);
	m_startText = new TextView("", ivec2(0, 0), tex);
	m_startText->SetParentView(this);
	sprintf(tex, "End: %.1f", m_end);
	m_endText = new TextView(id, ivec2(0, 0), tex);
	m_endText->SetParentView(this);

	m_startButton = new Button("ClipBar.StartButton", ivec2(0, 0), "", ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_startButton->SetParentView(this);
	
	m_endButton = new Button("ClipBar.EndButton", vec2(0, 0), "", ClipBarMeasure::m_slideLen, ClipBarMeasure::m_slideLen);
	m_endButton->SetParentView(this);
	ClipButtonListener *listener = new ClipButtonListener(this);
	m_startButton->SetMouseListener(listener);
	m_endButton->SetMouseListener(listener);
}

void ClipBar::SetStartValue(float value)
{
	char str[20];
	sprintf(str, "Start:%.1f", value);
	m_start = value;
	m_startText->SetText(str);
}

void ClipBar::SetEndValue(float value)
{
	char str[20];
	sprintf(str, "End:%.1f", value);
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

void ClipBar::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);
	m_lenText->OnMeasure(m_width, m_height);
	m_startText->OnMeasure(m_width, m_height);
	m_endText->OnMeasure(m_width, m_height);
	m_startButton->OnMeasure(m_width, m_height);
	m_endButton->OnMeasure(m_width, m_height);
}

void ClipBar::OnPosit(int fatherWidth, int fatherHeight)
{
	View::OnPosit(fatherWidth, fatherHeight);
	ivec2 aP = GetAbsolutePosition();
	m_lenText->OnPosit(aP.x, aP.y);
	m_lenText->SetPosition(ivec2(ClipBarMeasure::m_leftOffset, ClipBarMeasure::m_lenToTop));

	m_startText->OnPosit(aP.x, aP.y);
	m_startText->SetPosition(ivec2(ClipBarMeasure::m_leftOffset, m_lenText->GetPositionY() + m_lenText->GetHeight() + ClipBarMeasure::m_startToLenDistence));
	
	m_endText->OnPosit(aP.x, aP.y);
	m_endText->SetPosition(ivec2(m_width - m_endText->GetWidth() - ClipBarMeasure::m_rightOffset, m_startText->GetPositionY()));
	
	m_startButton->OnPosit(aP.x, aP.y);
	m_startButton->SetPosition(ivec2(ClipBarMeasure::m_leftOffset - ClipBarMeasure::m_slideLen / 2.0, GetHeight() - ClipBarMeasure::m_slideLen - ClipBarMeasure::m_slideToBottom));
	m_minPositionX = m_startButton->GetPositionX();
	m_startButton->AddPosition(m_start * (ClipBarMeasure::m_axisLen / m_length), 0.0);
	
	m_endButton->OnPosit(aP.x, aP.y);
	m_endButton->SetPosition(ivec2(ClipBarMeasure::m_leftOffset + ClipBarMeasure::m_axisLen - ClipBarMeasure::m_slideLen / 2.0, m_startButton->GetPositionY()));
	m_maxPositionX = m_endButton->GetPositionX();
	m_endButton->AddPosition(ClipBarMeasure::m_axisLen - m_end * (ClipBarMeasure::m_axisLen / m_length), 0.0);
	
	m_axisPosition.x = ClipBarMeasure::m_leftOffset;
	m_axisPosition.y = m_startText->GetPositionY() + m_startText->GetHeight() + ClipBarMeasure::m_axisToStart;
}

bool ClipBar::DispatchEvent(Event & ievent)
{
	if (!HitView(ievent))
		return false;

	//在本控件区域内
	if (m_startButton->DispatchEvent(ievent))
		return true;

	if (m_endButton->DispatchEvent(ievent))
		return true;

	static bool isDown = false;
	static ivec2 lastPosition;
	if (ievent.isMouseKeyDown())
	{
		isDown = true;
		lastPosition = ievent.m_mousePosition;
	}
	else if (ievent.isMouseKeyUp())
		isDown = false;

	if (isDown && ievent.m_mouseMotion == MouseMotion::MouseMove)
	{
		ivec2 curPostion = ievent.m_mousePosition;
		AddPosition(curPostion.x - lastPosition.x, curPostion.y - lastPosition.y);
		lastPosition = curPostion;
		ivec2 pP = m_parentView->GetAbsolutePosition();
		OnPosit(pP.x, pP.y);
	}
	return true;
}

ListView::ListView(string id, ivec2 position, int width, int height) : View(id, position, width, height)
{
	
}

void ListView::AddItem(ListItem * item)
{
	if (item == nullptr)
		return;
	m_items.push_back(item);
}

void ListView::OnDraw(Tiny2D * paint)
{
	paint->DrawRect(GetAbsolutePosition(), m_width, m_height, vec3(0.5, 0.2, 0.7));
	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
		(*i)->OnDraw(paint);
}

void ListView::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);
	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
		(*i)->OnMeasure(m_width - 2 * LRPadding, m_height);
}

void ListView::OnPosit(int fatherWidth, int fatherHeight)
{
	View::OnPosit(fatherWidth, fatherHeight);
	ivec2 aP = GetAbsolutePosition();
	int height = TBPadding;
	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
	{
		(*i)->SetPosition(ivec2(LRPadding, height));
		(*i)->OnPosit(aP.x, aP.y);
		height += (*i)->GetHeight() + 2 * TBPadding;
	}
}

bool ListView::DispatchEvent(Event & ievent)
{
	if (!HitView(ievent))
		return false;

	for (list<ListItem*>::iterator i = m_items.begin(); i != m_items.end(); i++)
		if((*i)->DispatchEvent(ievent))
			return true;

	static bool isDown = false;
	static ivec2 lastPosition;
	if (ievent.isMouseKeyDown())
	{
		isDown = true;
		lastPosition = ievent.m_mousePosition;
	}
	else if (ievent.isMouseKeyUp())
		isDown = false;

	if (isDown && ievent.m_mouseMotion == MouseMotion::MouseMove)
	{
		ivec2 curPostion = ievent.m_mousePosition;
		AddPosition(curPostion.x - lastPosition.x, curPostion.y - lastPosition.y);
		lastPosition = curPostion;
		ivec2 pP = m_parentView->GetAbsolutePosition();
		OnPosit(pP.x, pP.y);
	}
	return true;
}




//ClipItem
ClipItem::ClipItem(string clipName, float start, float end)
{
	m_clipName = clipName;
	m_start = start;
	m_end = end;

	char str[20];
	m_texClip = new TextView(m_id, vec2(0, 0), clipName);
	sprintf(str, "Start:%.1f", m_start);
	m_texStart = new TextView(m_id, vec2(0, 0), str);
	sprintf(str, "End:%.1f", m_end);
	m_texEnd = new TextView(m_id, vec2(0, 0), str);
}

void ClipItem::OnDraw(Tiny2D * paint)
{
	ivec2 aP = GetAbsolutePosition();
	paint->DrawRect(aP, m_width, m_height, vec3(0.45, 0.45, 0.45));
	m_texClip->OnDraw(paint);
	m_texStart->OnDraw(paint);
	m_texEnd->OnDraw(paint);
}

void ClipItem::OnMeasure(int fatherWidth, int fatherHeight)
{
	View::OnMeasure(fatherWidth, fatherHeight);

	m_texClip->OnMeasure(m_width, m_height);
	m_texStart->OnMeasure(m_width / 2.0, m_height);
	m_texEnd->OnMeasure(m_width / 2.0, m_height);

	m_height = TopPadding + m_texClip->GetHeight() + Interval + m_texStart->GetHeight() + BottomPadding;
}

void ClipItem::OnPosit(int fatherWidth, int fatherHeight)
{
	View::OnPosit(fatherWidth, fatherHeight);
	ivec2 aP = GetAbsolutePosition();
	m_texClip->OnPosit(aP.x, aP.y);
	m_texClip->SetPosition(ivec2(LeftPadding, TopPadding));
	int sumHeight = m_texClip->GetPositionY() + m_texClip->GetHeight() + Interval;
	m_texStart->OnPosit(aP.x, aP.y);
	m_texStart->SetPosition(ivec2(LeftPadding, sumHeight));
	m_texEnd->OnPosit(aP.x, aP.y);
	m_texEnd->SetPosition(ivec2(m_width - RightPadding - m_texEnd->GetWidth(), sumHeight));
}
