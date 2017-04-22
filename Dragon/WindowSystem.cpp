#include "WindowSystem.h"
#include "InputSystem.h"
#include "CommonType.h"
#include "Activity.h"

WindowSystem::WindowSystem(int width, int height, string windowName) : m_windowWidth(width), m_windowHeight(height), m_windowName(windowName)
{
	m_engine = nullptr;
}

void WindowSystem::GetFrameSize(int & frameWidth, int & frameHeight)
{
	frameWidth = m_frameWidth;
	frameHeight = m_frameHeight;
}

void WindowSystem::AssignInput(InputSystem * input)
{
	m_input = input;
}

void WindowSystem::AssginEngine(DragonEngine * engine)
{
	m_engine = engine;
}

void WindowSystem::AddActivity(Activity * activity)
{
	UIModel::AddActivity(activity);
	activity->OnMeasure(m_frameWidth, m_frameHeight);
	activity->OnPosit(0, 0);
}

void WindowSystem::MouseEvent(int x, int y, MouseMotion mouseMotion)
{
	static int buttonMap[]{ MOUSE_UNKNOWN, MOUSE_KEY_RIGHT, MOUSE_KEY_RIGHT, MOUSE_KEY_LEFT, MOUSE_KEY_LEFT, MOUSE_KEY_MIDDLE };
	static bool isDown[]{ false, true, false, true, false, false};
	Event mouseEvent;
	mouseEvent.m_mousePosition.x = x;
	mouseEvent.m_mousePosition.y = y;
	mouseEvent.m_mouseMotion = mouseMotion;

	m_input->SetMousePosition(x, y);
	if (isDown[mouseMotion])
	{
		m_input->MouseKeyDown(buttonMap[mouseMotion]);
	}
	else
	{
		m_input->MouseKeyUp(buttonMap[mouseMotion]);
	}
	GetActive()->AcceptEvent(ivec2(x, y), mouseMotion);
}

void WindowSystem::KeyEvent(int key, KeyMotion keyMotion)
{
	if (keyMotion == KeyMotion::KeyDown)
		m_input->KeyDown(key);
	else if (keyMotion == KeyMotion::KeyUp)
		m_input->KeyUp(key);
	GetActive()->AcceptEvent(key, keyMotion);
}


Event::Event(void) : m_mousePosition(0, 0), m_mouseMotion(MouseMotion::NoughtMouse), m_keyMotion(KeyMotion::NoughtKey), m_keyCode(0) 
{

}
