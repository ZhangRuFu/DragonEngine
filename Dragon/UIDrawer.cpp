#include <GLM\gtx\transform.hpp>
#include "UIDrawer.h"
#include "UI.h"
#include "RenderSystem.h"
#include "Tiny2D.h"

UIDrawer::UIDrawer(void) {}

ButtonDrawer::ButtonDrawer(const Button *button)
{
	m_button = button;
	m_textDrawer = TextViewDrawer::Create(button->GetTextView(), false);
}

void ButtonDrawer::Draw()
{
	
	glm::vec3 position = glm::vec3(m_button->GetAbsolutePosition(), 0.0f);
	glm::ivec2 dimension = m_button->GetDimension();

	int buttonState = m_button->GetButtonState();

	static glm::vec3 buttonColor = vec3(0.0f, 0.0f, 0.9f);
	static glm::vec3 offset = vec3(0.1f, 0.1f, 0.1f);
	vec3 color = buttonColor;
	if (buttonState == ButtonState::Focus)
		color += offset;
	else if (buttonState == ButtonState::Clicked)
		color -= offset;
	
	DrawRect(position, dimension.x, dimension.y, color);
	m_textDrawer->Draw();
	
}

ButtonDrawer * ButtonDrawer::Create(const Button * button, bool isRegister)
{
	ButtonDrawer *drawer = new ButtonDrawer(button);
	if(isRegister)
		drawer->Register();
	return drawer;
}

TextViewDrawer::TextViewDrawer(const TextView * texView)
{
	m_texView = texView;
}

TextViewDrawer * TextViewDrawer::Create(const TextView * texView, bool isRegister)
{
	TextViewDrawer *drawer = new TextViewDrawer(texView);
	if(isRegister)
		drawer->Register();
	return drawer;
}

void TextViewDrawer::Draw(void)
{
	DrawText(m_texView->GetText(), m_texView->GetAbsolutePosition(), m_texView->GetFontSize(), vec3(1.0f, 1.0f, 1.0f));
}

ClipBarDrawer::ClipBarDrawer(const ClipBar * clipBar)
{
	m_clipBar = clipBar;
	m_startText = TextViewDrawer::Create(clipBar->GetStartTextView(), false);
	m_endText = TextViewDrawer::Create(clipBar->GetEndTextView(), false);
	m_lenText = TextViewDrawer::Create(clipBar->GetLengthTextView(), false);
	m_startButton = ButtonDrawer::Create(clipBar->GetStartButton(), false);
	m_endButton = ButtonDrawer::Create(clipBar->GetEndButton(), false);
	m_axisPosition.x = ClipBarMeasure::m_leftOffset;
	m_axisPosition.y = clipBar->GetStartTextView()->GetPositionY() + clipBar->GetStartTextView()->GetHeight() + ClipBarMeasure::m_axisToStart;
}

void ClipBarDrawer::Draw(void)
{
	ivec2 rt;
	ivec2 dimension;
	rt = m_clipBar->GetAbsolutePosition();
	dimension = m_clipBar->GetDimension();
	DrawRoundRect(rt, dimension.x, dimension.y, 15 ,vec3(0.7f, 0.1f, 0.2f));
	rt.x += m_axisPosition.x;
	rt.y += m_axisPosition.y;
	DrawLine(vec2(rt.x, rt.y), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y));
	DrawLine(vec2(rt.x, rt.y - 5), vec2(rt.x, rt.y + 5));
	DrawLine(vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y - 5), vec2(rt.x + ClipBarMeasure::m_axisLen, rt.y + 5));
	m_startText->Draw();
	m_endText->Draw();
	m_lenText->Draw();
	m_startButton->Draw();
	m_endButton->Draw();
}

ClipBarDrawer* ClipBarDrawer::Create(const ClipBar * clipBar, bool isRegister)
{
	ClipBarDrawer *drawer = new ClipBarDrawer(clipBar);
	if (isRegister)
		drawer->Register();
	return drawer;
}
