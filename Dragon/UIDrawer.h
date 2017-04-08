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
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ButtonDrawer
*	�衡������Button������
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
*	����汾��Dragon Engine v0.1;
*	�ࡡ������TextViewDrawer
*	�衡������TextView������
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