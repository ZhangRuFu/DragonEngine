#pragma once
#include "CommonType.h"
#include "Shader.h"
#include "Tiny2D.h"
#include <GLM\glm.hpp>

class View;
class Button;
class TextView;
class ClipBar;
class Tiny2D;

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
	static TextViewDrawer* Create(const TextView *texView, bool isRegister = true);
	virtual void Draw(void);
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ButtonDrawer
*	�衡������Button������
*
*/
class ButtonDrawer : public UIDrawer
{
protected:
	const Button *m_button;
	TextViewDrawer *m_textDrawer;

	ButtonDrawer(const Button *button);

public:

	virtual void Draw(void);
	static ButtonDrawer* Create(const Button * button, bool isRegister = true);
};


/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ClipBarDrawer
*	�衡������ClipBar������
*
*/

class ClipBarDrawer : public UIDrawer
{
protected:
	ClipBarDrawer(const ClipBar *clipBar);
	
private:
	const ClipBar *m_clipBar;
	TextViewDrawer *m_startText;
	TextViewDrawer *m_endText;
	TextViewDrawer *m_lenText;
	ButtonDrawer *m_startButton;
	ButtonDrawer *m_endButton;
	
	ivec2 m_axisPosition;

public:
	virtual void Draw(void);
	static ClipBarDrawer* Create(const ClipBar *clipBar, bool isRegister = true);
};