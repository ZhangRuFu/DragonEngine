#include "DragonEngine.h"
#include "Soldier.h"
#include "WoodPlane.h"
#include "Monster.h"
#include "GLFWWindowSystem.h"
#include <Windows.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

using std::cout;
using std::endl;

DragonEngine::DragonEngine(void)
{
	string windowName = "Dragon Engine 0.1";
	int frameWidth, frameHeight;
	
	m_gameState = GameState::PreStart;
	m_windowSystem = new GLFWWindowSystem(800, 800, windowName);
	m_windowSystem->GetFrameSize(frameWidth, frameHeight);
	m_renderSystem = RenderSystem::GetInstance(frameWidth, frameHeight);
	m_resourceSystem = ResourceSystem::GetInstance();
	m_inputSystem = InputSystem::GetInstance();
	m_windowSystem->AssignEngine(this);
	m_windowSystem->AssignInput(m_inputSystem);
	m_windowSystem->InitUI();
	m_time = new Time();
	Init();
	cout << "DragonEngine->SUCCESS:DragonEngine初始化完成！" << endl;
}

void DragonEngine::Move()
{
	static unsigned int lastTime = timeGetTime();
	unsigned int currentTime = timeGetTime();
	unsigned int timeSpan = currentTime - lastTime;
	m_time->Elapse(timeSpan);
	lastTime = currentTime;
	if(m_gameState == GameState::Gaming)
		m_resourceSystem->Move();
}

void DragonEngine::Draw()
{
	m_renderSystem->Draw();
}

void DragonEngine::Start()
{
	//游戏循环开始
	m_gameState = GameState::Gaming;
	cout << "DragonEngine->SUCCESS:DragonEngine开始运行!" << endl;
	m_windowSystem->Start();

}

void DragonEngine::Init()
{
	Camera *m_camera = new Camera();
	Soldier *soldier = new Soldier();
	//Monster *monster = new Monster();
	WoodPlane *woodPlane = new WoodPlane();
	Light *light = new Light(Transform(vec3(20, 20, 20)));
	
	woodPlane->GetTransform()->Scale(vec3(50, 50, 50));
}
