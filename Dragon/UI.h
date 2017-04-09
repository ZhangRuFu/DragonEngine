#pragma once
#include <list>
#include <string>
#include <GLM\glm.hpp>
#include <iostream>
#include "WindowSystem.h"

using std::list;
using std::string;
using glm::vec2;
using glm::ivec2;

class ButtonDrawer;
class MouseListener;
class KeyBoardListener;

/*
*
*	头文件说明：这里是UI部分的纯逻辑、数据表示，与绘制无关
*
*/


/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：View
*	描　　述：UI控件基类
*
*/

class View
{
protected:
	string m_id;
	ivec2 m_position;					//相对于父对象
	ivec2 m_fatherPosition;				//绝对位置
	int m_width, m_height;

	//监听器
	MouseListener *m_mouseListener;
	KeyBoardListener *m_keyListener;

public:
	View(const string &id, vec2 position, int width, int height);

	virtual bool DispatchEvent(Event &ievent);
	void RePosit(ivec2 fatherPosition) { m_fatherPosition = fatherPosition; }
	//宽高适配
	void ReSize(int width, int height) { /*m_width = width;m_height = height;*/ }

	string GetViewID(void) { return m_id; }
	void SetMouseListener(MouseListener *mouseListener) { m_mouseListener = mouseListener; }
	void SetKeyListener(KeyBoardListener *keyListener) { m_keyListener = keyListener; }

	ivec2 GetAbsolutePosition(void) const { return m_position + m_fatherPosition; }
	ivec2 GetDimension(void) const { return ivec2(m_width, m_height); }
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ViewGroup
*	描　　述：View容器，可包含多个View，ListView中ListItem继承至此。也就是可配置、扩展的控件
*
*/

//==================================改成View*就好了？=============================
class ViewGroup : public View
{
protected:
	list<View*> m_viewList;

public:
	ViewGroup(const string &id, vec2 position, int width, int height) : View(id, position, width, height) {}
	virtual bool DispatchEvent(Event &ievent);

	void AddView(View& view);
	View* FindViewByID(string id);
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：UIManager
*	描　　述：管理UI控件
*
*/

class UIManager : public ViewGroup
{
private:
	static UIManager *s_instance;

public:
	UIManager(int width, int height);

public:
	void AcceptEvent(ivec2 mousePosition, MouseMotion mouseMotion);
	void AcceptEvent(int keyCode, KeyMotion keyMotion);

	static glm::mat4 GenProjection(void);
};

/*
*	MouseListener 鼠标监听器
*	KeyBoardListener 键盘监听器
*/

class MouseListener
{
public:
	virtual bool onMouse(View &view, const Event &e) = 0;
};

class KeyBoardListener
{
public:
	virtual bool onKey(View &view, const Event &e) = 0;
};


//============================================具体UI===========================================

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：TextView
*	描　　述：文字显示控件
*
*/

class TextView : public View
{
protected:
	string m_str;

public:
	TextView(const string &id, vec2 position, int width, int height);
	const string& GetText(void) const {
		return m_str;
	}
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Button
*	描　　述：按钮
*
*/

enum ButtonState { Normal, Focus, Clicked };

class Button : public View
{
private:
	ButtonState m_state;
	ButtonDrawer *m_btnDrawer;

public:
	Button(const string &id, vec2 position, int width, int height);
	ButtonState GetButtonState(void) const { return m_state; }
	virtual bool DispatchEvent(Event &ievent);
};

class iMouseListener : public MouseListener
{
public:
	virtual bool onMouse(View &view, const Event &e)
	{
		using namespace std;
		if (view.GetViewID() == "btnTest")
		{
			if (e.m_mouseMotion == MouseMotion::LeftButtonDown)
				std::cout << "左键按下-btnTest" << std::endl;
			else if (e.m_mouseMotion == MouseMotion::RightButtonDown)
				cout << "右键按下" << endl;
		}
		if (view.GetViewID() == "btnTest2")
		{
			if (e.m_mouseMotion == MouseMotion::MouseMove)
				cout << "移动!-btnTest2" << endl;
		}
		return true;
	}
};