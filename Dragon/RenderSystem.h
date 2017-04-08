#pragma once
/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：RenderSystem
*	描　　述：对OpenGL、DirectX等底层渲染引擎的抽象，目前只支持OpenGL，对需要绘制的对象进行管理
*
*/
#include <GL\glew.h>
#include <list>
#include <vector>
#include <map>
#include <string>

#include "Drawer.h"
#include "CommonType.h"
#include "UIDrawer.h"

using std::list;
using std::vector;
using std::map;
using std::string;

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：DrawList
*	描　　述：绘制列表，多个待绘制资源的Drawer对应同一个shader，为了避免切换shader带来的开销，以这样的列表形式绘制
*
*/
//=========================================改成结构体可能更好====================================
template<typename T>
class DrawerList
{
	Shader *m_shader;				//shader
	list<T*> m_drawerList;			//drawer列表

public:
	DrawerList(Shader *shader);

	friend class RenderSystem;
};

class RenderSystem
{
	enum ShaderType { VertexShader = GL_VERTEX_SHADER, FragmentShader = GL_FRAGMENT_SHADER };
	enum RenderType { Entity, UI};

private:
	int m_frameWidth;
	int m_frameHeight;

	vector<DrawerList<Drawer>> m_renderList;					//绘制列表
	DrawerList<UIDrawer> m_uiRenderList;						//UI绘制列表

	map<string, unsigned int> m_shaderMap;	

	static RenderSystem *m_instance;
private:
	RenderSystem(int frameWidth, int frameHeight);

public:
	static RenderSystem* GetInstance(int frameWidth = 500, int frameHeight = 500);

public:
	void Draw();
	void ReSize(int frameWidth, int frameHeight);
	static unsigned int CreateTexture(TextureInfo &info);
	static unsigned int CreateCubeTexture(vector<TextureInfo> *info);
	static Shader* LoadShader(string shaderName);
	static void RegisterDrawer(Drawer *drawer);
	static void RegisterDrawer(UIDrawer *uiDrawer);

private:
	bool Init(void);

	//Drawer注册
	void Register(Drawer *drawer);
	void Register(UIDrawer *drawer);

	//Shader加载
	Shader* CreateShader(string shaderPath, string shaderName);
	void AddRenderList(Shader* shader, RenderType type);
	int LoadShaderSource(string fileName, char ** source);
	void AddShader(string sourcePath, ShaderType shaderType, GLenum shaderID);
	void RenderSystem::Validate(GLenum shaderID);
};