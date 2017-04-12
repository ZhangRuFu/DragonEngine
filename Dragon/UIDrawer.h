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
*	头文件说明：这里是UI控件的绘制部分
*
*/


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
	static TextViewDrawer* Create(const TextView *texView);
	virtual void Draw(void);
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ButtonDrawer
*	描　　述：Button绘制器
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

