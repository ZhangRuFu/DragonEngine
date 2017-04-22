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

class Tiny2D;
class View;

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

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：View
*	描　　述：UI控件基类
*
*/

class View
{
public:
	enum Dimension{MATCH_PARENT = -2, WRAP_CONTENT = -1};

protected:
	string m_id;
	ivec2 m_relativePosition;					//相对于父View的相对位置
	ivec2 m_fatherPosition;						//父级View位置
	int m_layoutWidth, m_layoutHeight;			//布局参数
	int m_width, m_height;						//宽、高
	View *m_parentView;							//父级视图

	//监听器
	MouseListener *m_mouseListener;
	KeyBoardListener *m_keyListener;

public:
	View(const string &id, ivec2 &position, int layoutWidth, int layoutHeight);
	
	//虚函数
	virtual bool DispatchEvent(Event &ievent);						//事件分发
	virtual void OnMeasure(int fatherWidth, int fatherHeight);		//尺寸测定
	virtual void OnPosit(int fatherX, int fatherY);					//位置测定
	virtual void OnDraw(Tiny2D *paint) = 0;							//绘制
	virtual void RequestMeasure(void) { OnMeasure(m_parentView->GetWidth(), m_parentView->GetHeight()); }

	//位置
	void SetFatherPosition(ivec2 fatherPosition) { m_fatherPosition = fatherPosition; }
	void SetPosition(ivec2 position) { m_relativePosition = position; }
	void SetPositionX(int x) { m_relativePosition.x = x; }
	void SetPositionY(int y) { m_relativePosition.y = y; }
	void AddPosition(int x, int y) { m_relativePosition.x += x, m_relativePosition.y += y; }
	void SetSize(int width, int height) { m_width = width; m_height = height; }
	void SetDimension(int width, int height) { m_width = width, m_height = height; }

	ivec2 GetAbsolutePosition(void) const { return m_relativePosition + m_fatherPosition; }
	ivec2 GetPosition(void) const { return m_relativePosition; }
	int GetPositionY(void) const { return m_relativePosition.y; }
	int GetPositionX(void) const { return m_relativePosition.x; }
	ivec2 GetDimension(void) const { return ivec2(m_width, m_height); }
	int GetWidth(void) const { return m_width; }
	int GetHeight(void) const { return m_height; }
	
	string GetViewID(void) { return m_id; }
	void SetParentView(View *parent);
	bool HitView(Event & ievent);

	//事件监听
	void SetMouseListener(MouseListener *mouseListener) { m_mouseListener = mouseListener; }
	void SetKeyListener(KeyBoardListener *keyListener) { m_keyListener = keyListener; }
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ViewGroup
*	描　　述：View容器，可包含多个View，ListView中ListItem继承至此。也就是可配置、扩展的控件
*
*/

class ViewGroup : public View
{
protected:
	list<View*> m_viewList;

public:
	//ViewGroup的宽度和高度不能为WRAP_CONTENT
	ViewGroup(const string id, ivec2 position, int width, int height) : View(id, position, width, height) {}
	virtual bool DispatchEvent(Event &ievent);						//事件分发
	virtual void OnMeasure(int fatherWidth, int fatherHeight);		//尺寸测定
	virtual void OnPosit(int fatherX, int fatherY);					//位置测定
	virtual void OnDraw(Tiny2D *paint);								//绘制
	void AddView(View *view);
	View* FindViewByID(string id);
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
public:
	enum TextAligin{Top = 1, Bottom = 2, Left = 4, Right = 8, Center = 15};

protected:
	string m_text;
	int m_fontSize;
	vec3 m_fontColor;
	ivec2 m_fontPosition;
	int m_texWidth, m_texHeight;
	TextAligin m_texAlign;

public:
	TextView(const string id, ivec2 position, string text, int width = Dimension::WRAP_CONTENT, int height = Dimension::WRAP_CONTENT, TextAligin texAlign = TextAligin::Center, vec3 color = vec3(1.0, 1.0, 1.0), int fontSize = 16);

	//Override
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherX, int fatherY);
	virtual void OnDraw(Tiny2D *paint);

	string GetText(void) const { return m_text; }
	void SetText(string text) { m_text = text; }
	vec3 GetFontColor(void) const { return m_fontColor; }
	void SetFontColor(vec3 color) { m_fontColor = color; }
	int GetFontSize(void) const { return m_fontSize; }
	void SetFontSize(int fontSize) { m_fontSize = fontSize; }
	TextAligin GetTextAlign(void) { return m_texAlign; }
	void SetTextAlign(TextAligin texAlign) { m_texAlign = texAlign; }
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Button
*	描　　述：按钮
*
*/

enum ButtonState { Normal, Focus, Clicked };

class Button : public TextView
{
private:
	ButtonState m_state;
	const int LRPadding = 15;
	const int TBPadding = 10;

public:
	Button(const string &id, ivec2 position, string text, int width = View::Dimension::WRAP_CONTENT, int height = View::Dimension::WRAP_CONTENT);
	
	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnPosit(int fatherX, int fatherY);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual bool DispatchEvent(Event &ievent);

	ButtonState GetButtonState(void) const { return m_state; }
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ClipBar
*	描　　述：片段选择
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
	ClipBar(string id, float len, ivec2 position, int width = 250, int height = 120);		//构造函数
	float GetStartValue(void) { return m_start; }
	float GetEndValue(void) { return m_end; }
	void SetStartValue(float value);
	void AddStartValue(float delta) { SetStartValue(delta + m_start); }
	void AddEndValue(float delta) { SetEndValue(m_end + delta); }
	void SetEndValue(float value);
	float GetLength(void) { return m_length; }
	int GetSlideMinX(void) { return m_minPositionX; }
	int GetSlideMaxX(void) { return m_maxPositionX; }

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherWidth, int fatherHeight);
	virtual bool DispatchEvent(Event &ievent);

private:
	float m_length;
	float m_start;
	float m_end;
	ivec2 m_axisPosition;

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
				if ("ClipBar.StartButton" == view.GetViewID())
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



/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ListItem
*	描　　述：ListView中Item的基类
*
*/
class ListItem : public View
{
public:
	ListItem(string id = "", ivec2 position = vec2(0, 0), int width = View::Dimension::MATCH_PARENT, int height = View::Dimension::WRAP_CONTENT) : View(id, position, width, height) {};
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ClipItem
*	描　　述：存放片段数据的ListItem
*
*/
class ClipItem : public ListItem
{
private:
	string m_clipName;
	float m_start;
	float m_end;

	static const int Interval = 10;
	static const int LeftPadding = 10;
	static const int RightPadding = 10;
	static const int TopPadding = 10;
	static const int BottomPadding = 10;

	TextView *m_texClip;
	TextView *m_texStart;
	TextView *m_texEnd;

public:
	ClipItem(string clipName, float start, float end);

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherWidth, int fatherHeight);
};

/*
*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：ListView
*	描　　述：列表视图
*
*/
class ListView : public View
{
	static const int LRPadding = 5;
	static const int TBPadding = 5;
private:
	list<ListItem*> m_items;

public:
	ListView(string id, ivec2 position, int width = 200, int height = 400);
	void AddItem(ListItem* item);
	list<ListItem*> GetItems(void) const { return m_items; }

	//Override
	virtual void OnDraw(Tiny2D *paint);
	virtual void OnMeasure(int fatherWidth, int fatherHeight);
	virtual void OnPosit(int fatherWidth, int fatherHeight);
	virtual bool DispatchEvent(Event &ievent);
};