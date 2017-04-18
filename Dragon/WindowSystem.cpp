#include "WindowSystem.h"
#include "InputSystem.h"
#include "CommonType.h"
#include "UI.h"
#include "UIDrawer.h"

WindowSystem::WindowSystem(int width, int height, string windowName) : m_windowWidth(width), m_windowHeight(height), m_windowName(windowName)
{
	
}

void WindowSystem::GetFrameSize(int & frameWidth, int & frameHeight)
{
	frameWidth = m_frameWidth;
	frameHeight = m_frameHeight;
}

void WindowSystem::AssignEngine(DragonEngine * engine)
{
	m_engine = engine;
}

void WindowSystem::AssignInput(InputSystem * input)
{
	m_input = input;
}

void WindowSystem::GetWindowSize(int &frameWidth, int &frameHeight)
{
	m_instance->GetFrameSize(frameWidth, frameHeight);
}

void WindowSystem::Render()
{
	
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
	m_uiManager->DispatchEvent(mouseEvent);
}

void WindowSystem::KeyEvent(int key, KeyMotion keyMotion)
{
	if (keyMotion == KeyMotion::KeyDown)
		m_input->KeyDown(key);
	else if (keyMotion == KeyMotion::KeyUp)
		m_input->KeyUp(key);
}

void WindowSystem::InitUI(void)
{
	m_uiManager = new UIManager(m_frameWidth, m_frameHeight);
	//Button *button = new Button("btnTest", vec2(50, 50), 80, 30, "Button");
	Button *button2 = new Button("btnTest2", vec2(10, 500), "Self-Resize-Button");
	ClipBar *clpBar = new ClipBar("clpTest", 100, vec2(50, 10));
	ListView *lstView = new ListView("lstView", vec2(550, 50));
	ClipItem *item = new ClipItem("T", 10, 20);
	lstView->AddItem(item);
	//MouseListener *i = new iMouseListener();
	//button->SetMouseListener(i);
	//TextView *tv = new TextView("TexTest", vec2(50, 150), "Hello DragonEngine", 16);
	//m_uiManager->AddView(*button);
	m_uiManager->AddView(*lstView);
	m_uiManager->AddView(*button2);
	m_uiManager->AddView(*clpBar);
	//m_uiManager->AddView(*tv);
}

WindowSystem * WindowSystem::m_instance = nullptr;

Event::Event(void) : m_mousePosition(0, 0), m_mouseMotion(MouseMotion::NoughtMouse), m_keyMotion(KeyMotion::NoughtKey), m_keyCode(0) 
{

}
