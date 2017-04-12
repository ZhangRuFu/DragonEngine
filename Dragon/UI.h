#pragma once
#include <list>
#include <string>
#include <GLM\glm.hpp>
#include <iostream>
#include "WindowSystem.h"

using std::list;
using std::string;
using glm::vec2;
using glm::vec3;
using glm::ivec2;

class ButtonDrawer;
class TextViewDrawer;
class MouseListener;
class KeyBoardListener;

/*
*
*	ͷ�ļ�˵����������UI���ֵĴ��߼������ݱ�ʾ��������޹�
*
*/


/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������View
*	�衡������UI�ؼ�����
*
*/

class View
{
protected:
	string m_id;
	ivec2 m_position;					//����ڸ�����
	ivec2 m_fatherPosition;				//����λ��
	int m_width, m_height;

	//������
	MouseListener *m_mouseListener;
	KeyBoardListener *m_keyListener;

public:
	virtual void Active(void) = 0;

	View(const string &id, vec2 position, int width, int height);

	virtual bool DispatchEvent(Event &ievent);
	void ReFatherPosition(ivec2 fatherPosition) { m_fatherPosition = fatherPosition; }
	void RePosition(ivec2 position) { m_position = position; }
	void ReSize(int width, int height) { m_width = width; m_height = height; }

	string GetViewID(void) { return m_id; }
	void SetMouseListener(MouseListener *mouseListener) { m_mouseListener = mouseListener; }
	void SetKeyListener(KeyBoardListener *keyListener) { m_keyListener = keyListener; }

	ivec2 GetAbsolutePosition(void) const { return m_position + m_fatherPosition; }
	ivec2 GetDimension(void) const { return ivec2(m_width, m_height); }
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ViewGroup
*	�衡������View�������ɰ������View��ListView��ListItem�̳����ˡ�Ҳ���ǿ����á���չ�Ŀؼ�
*
*/

//==================================�ĳ�View*�ͺ��ˣ�=============================
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
*	����汾��Dragon Engine v0.1;
*	�ࡡ������UIManager
*	�衡����������UI�ؼ�
*
*/

class UIManager : public ViewGroup
{
private:
	static UIManager *s_instance;

public:
	UIManager(int width, int height);
	void AcceptEvent(ivec2 mousePosition, MouseMotion mouseMotion);
	void AcceptEvent(int keyCode, KeyMotion keyMotion);

	static glm::mat4 GenProjection(void);

	virtual void Active(void) {};
};

/*
*	MouseListener ��������
*	KeyBoardListener ���̼�����
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


//============================================����UI===========================================

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������TextView
*	�衡������������ʾ�ؼ�
*
*/

class TextView : public View
{
protected:
	string m_str;
	int m_fontSize;

public:
	TextView(const string &id, vec2 position, string string, int fontSize, vec3 color = vec3(1.0, 1.0, 1.0));
	const string& GetText(void) const { return m_str; }
	int GetFontSize(void) const { return m_fontSize; }
	void SetFontSize(int fontSize) { m_fontSize = fontSize; }
	virtual void Active(void);
};

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������Button
*	�衡��������ť
*
*/

enum ButtonState { Normal, Focus, Clicked };

class Button : public View
{
private:
	ButtonState m_state;
	TextView *m_text;

	static const int m_ubOffset = 5;
	static const int m_lrOffset = 15;

public:
	Button(const string &id, vec2 position, int width, int height, string text="");
	Button(const string &id, vec2 position, string text = "");
	ButtonState GetButtonState(void) const { return m_state; }
	virtual bool DispatchEvent(Event &ievent);
	virtual void Active(void);
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
				std::cout << "�������" << std::endl;
			else if (e.m_mouseMotion == MouseMotion::RightButtonDown)
				cout << "�Ҽ�����" << endl;
		}
		if (view.GetViewID() == "btnTest2")
		{
			if (e.m_mouseMotion == MouseMotion::MouseMove)
				cout << "�ƶ�!-btnTest2" << endl;
		}
		return true;
	}
};