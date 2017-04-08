#pragma once
#include "CommonType.h"
#include "Shader.h"
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

class UIDrawer
{
protected:
	static Tiny2D *m_drawer;

public:
	UIDrawer(void);

	virtual void Draw() = 0;
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

public:
	ButtonDrawer(const Button *button);

	virtual void Draw(void);
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

public:
	TextViewDrawer(const TextView *texView);

	virtual void Draw(void);
};