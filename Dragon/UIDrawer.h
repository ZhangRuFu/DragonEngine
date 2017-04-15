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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：UIDrawer
*	描　　述：UI绘制器基类，使用Tiny2D提供的接口进行绘制
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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：TextViewDrawer
*	描　　述：TextView绘制器
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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ButtonDrawer
*	描　　述：Button绘制器
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
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ClipBarDrawer
*	描　　述：ClipBar绘制器
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