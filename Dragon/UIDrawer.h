#pragma once
#include "CommonType.h"
#include "Shader.h"
#include "Tiny2D.h"
#include <GLM\glm.hpp>

class View;
class Button;
class TextView;
class Tiny2D;

/*
*
*	ͷ�ļ�˵����������UI�ؼ��Ļ��Ʋ���
*
*/


/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������UIDrawer
*	�衡������UI���������࣬ʹ��Tiny2D�ṩ�Ľӿڽ��л���
*
*/

class UIDrawer : public Tiny2D
{
public:
	UIDrawer(void);

	virtual void Draw() = 0;
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::UI; }
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������TextViewDrawer
*	�衡������TextView������
*
*/
class TextViewDrawer : public UIDrawer
{
protected:
	const TextView *m_texView;
	TextViewDrawer(const TextView *texView);

public:
	static TextViewDrawer* Create(const TextView *texView);
	virtual void Draw(void);
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ButtonDrawer
*	�衡������Button������
*
*/
class ButtonDrawer : public TextViewDrawer
{
protected:
	const Button *m_button;
	ButtonDrawer(const Button *button, const TextView *textView);

public:

	virtual void Draw(void);
	static ButtonDrawer* Create(const Button * button, const TextView *textView);
};

