#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������WindowSystem
*	�衡�������Բ�ͬ����ϵͳ�ĳ���
*
*/
#include <string>
#include <map>
#include <GLM\glm.hpp>

using std::string;
using std::map;

class DragonEngine;
class InputSystem;
class UIManager;

/*
*	Event �¼�
*
*/

enum MouseMotion { NoughtMouse, RightButtonDown, RightButtonUp, LeftButtonDown, LeftButtonUp, MouseMove };
enum KeyMotion { NoughtKey, KeyDown, KeyUp };

struct Event
{
	//Mouse
	MouseMotion m_mouseMotion;
	glm::ivec2 m_mousePosition;

	//KeyBoard
	KeyMotion m_keyMotion;
	int m_keyCode;

	Event(void);
	bool isMouseKeyDown(void)
	{
		static const bool isDown[]{ false, true, false, true, false, false };
		return isDown[m_mouseMotion];
	}

	bool isMouseKeyUp(void)
	{
		static const bool isUp[]{ false, false, true, false, true, false };
		return isUp[m_mouseMotion];
	}
};

class WindowSystem
{
protected:
	static WindowSystem *m_instance;
	DragonEngine *m_engine;
	InputSystem *m_input;
	UIManager *m_uiManager;
	int m_windowWidth;
	int m_windowHeight;
	int m_frameWidth;
	int m_frameHeight;
	string m_windowName;
	map<int, int> m_keyMap;
	

public:
	WindowSystem(int width, int height, string windowName);

public:
	void GetFrameSize(int &frameWidth, int &frameHeight);
	void AssignEngine(DragonEngine *engine);
	void AssignInput(InputSystem *input);

public:
	virtual void Render() = 0;
	virtual void Start() = 0;
	void MouseEvent(int x, int y, MouseMotion mouseMotion);
	void KeyEvent(int key, KeyMotion keyMotion);
	void InitUI(void);
};