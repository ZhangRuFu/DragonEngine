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
class UIDrawer;
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

	UIDrawer *m_drawer;

	//������
	MouseListener *m_mouseListener;
	KeyBoardListener *m_keyListener;

public:
	View(const string &id, vec2 position, int width, int height);
	virtual bool DispatchEvent(Event &ievent);

	//λ��
	void ReFatherPosition(ivec2 fatherPosition) { m_fatherPosition = fatherPosition; }
	void RePosition(ivec2 position) { m_position = position; }
	void RePositionX(int x) { m_position.x = x; }
	void RePositionY(int y) { m_position.y = y; }
	void ReSize(int width, int height) { m_width = width; m_height = height; }
	
	ivec2 GetAbsolutePosition(void) const { return m_position + m_fatherPosition; }
	ivec2 GetPosition(void) const { return m_position; }
	int GetPositionY(void) const { return m_position.y; }
	int GetPositionX(void) const { return m_position.x; }
	ivec2 GetDimension(void) const { return ivec2(m_width, m_height); }
	int GetWidth(void) const { return m_width; }
	int GetHeight(void) const { return m_height; }
	void AddPosition(int x, int y) { m_position.x += x, m_position.y += y; }
	string GetViewID(void) { return m_id; }

	//�¼�����
	void SetMouseListener(MouseListener *mouseListener) { m_mouseListener = mouseListener; }
	void SetKeyListener(KeyBoardListener *keyListener) { m_keyListener = keyListener; }

	void RegisterDrawer(void);
	UIDrawer* GetDrawer(void) const { return m_drawer; }
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
	void SetText(string str) { m_str = str; }
	int GetFontSize(void) const { return m_fontSize; }
	void SetFontSize(int fontSize) { m_fontSize = fontSize; }
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
	const TextView *GetTextView(void) const 
	{ 
		int i = 5;
		return m_text; 
	}
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

/*
*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������ClipBar
*	�衡������Ƭ��ѡ��
*
*/

class ClipBarMeasure
{
public:
	static const int m_lenToTop = 20;
	static const int m_leftOffset = 20;
	static const int m_rightOffset = 20;
	static const int m_startToLenDistence = 10;
	static const int m_axisToStart = 20;
	static const int m_axisLen = 210;
	static const int m_slideLen = 10;
	static const int m_slideToBottom = 10;
};

class ClipBar : public View
{
public:
	ClipBar(string id, float len, vec2 position, int width = 250, int height = 120);		//���캯��
	float GetStartValue(void) { return m_start; }
	float GetEndValue(void) { return m_end; }
	void SetStartValue(float value);
	void AddStartValue(float delta) { SetStartValue(delta + m_start); }
	void AddEndValue(float delta) { SetEndValue(m_end + delta); }
	void SetEndValue(float value);
	float GetLength(void) { return m_length; }
	int GetSlideMinX(void) { return m_minPositionX; }
	int GetSlideMaxX(void) { return m_maxPositionX; }

	const TextView* GetStartTextView(void) const { return m_startText; };
	const TextView* GetEndTextView(void) const { return m_endText; }
	const TextView* GetLengthTextView(void) const { return m_lenText; };
	const Button* GetStartButton(void) const { return m_startButton; };
	const Button* GetEndButton(void) const { return m_endButton; }

	virtual bool DispatchEvent(Event &ievent);
private:
	float m_length;
	float m_start;
	float m_end;

	TextView *m_startText;
	TextView *m_endText;
	TextView *m_lenText;
	Button *m_startButton;
	Button *m_endButton;

	int m_minPositionX, m_maxPositionX;

	class ClipButtonListener : public MouseListener
	{
	private:
		ClipBar *m_clipBar;

	public:
		ClipButtonListener(ClipBar *clipBar) { m_clipBar = clipBar; }

		virtual bool onMouse(View &view, const Event &e)
		{
			using namespace std;
			static bool down = false;
			static ivec2 position;
			if (e.m_mouseMotion == MouseMotion::LeftButtonDown)
			{
				down = true;
				position = e.m_mousePosition;
			}
			else if (e.m_mouseMotion == MouseMotion::LeftButtonUp)
				down = false;
			if (down && e.m_mouseMotion == MouseMotion::MouseMove)
			{
				int viewX = view.GetPositionX();
				float deltaX = e.m_mousePosition.x - position.x;
				if (viewX + deltaX > m_clipBar->GetSlideMaxX() || viewX + deltaX < m_clipBar->GetSlideMinX())
					return true;
				float len = deltaX / ClipBarMeasure::m_axisLen * m_clipBar->GetLength();
				if (m_clipBar->GetStartButton() == &view)
					m_clipBar->AddStartValue(len);
				else
					m_clipBar->AddEndValue(len);
				view.AddPosition(deltaX, 0);
				position = e.m_mousePosition;
			}
			return true;
		}
	};
};

class ListItem : public View
{
public:
	ListItem(string id = "item", vec2 position = vec2(0, 0), int width = 0, int height = 0);
};

class ClipItem : public ListItem
{
private:
	string m_clipName;
	float m_start;
	float m_end;

	TextView *m_texClip;
	TextView *m_texStart;
	TextView *m_texEnd;

public:
	ClipItem(string clipName, float start, float end);
	const TextView* GetClipNameTextView(void) { return m_texClip; }
	const TextView* GetStartTextView(void) { return m_texStart; }
	const TextView* GetEndTextView(void) { return m_texEnd; }
};

class ListView : public View
{
private:
	list<ListItem*> m_items;

public:
	ListView(string id, vec2 position, int width = 200, int height = 400);
	void AddItem(ListItem* item);
	list<ListItem*> GetItems(void) const { return m_items; }
};