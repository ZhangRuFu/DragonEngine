#include <GLM\gtx\transform.hpp>
#include "UIDrawer.h"
#include "UI.h"
#include "RenderSystem.h"
#include "Tiny2D.h"

UIDrawer::UIDrawer(void) {}

ButtonDrawer::ButtonDrawer(const Button *button)
{
	m_button = button;
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
}

ButtonDrawer * ButtonDrawer::Create(const Button * button)
{
	ButtonDrawer *drawer = new ButtonDrawer(button);
	drawer->Register();
	return drawer;
}

TextViewDrawer::TextViewDrawer(const TextView * texView)
{
	m_texView = texView;
}

TextViewDrawer * TextViewDrawer::Create(const TextView * texView)
{
	TextViewDrawer *drawer = new TextViewDrawer(texView);
	drawer->Register();
	return drawer;
}

void TextViewDrawer::Draw(void)
{
	DrawText(m_texView->GetText(), m_texView->GetAbsolutePosition(), vec3(1.0f, 1.0f, 1.0f));
}