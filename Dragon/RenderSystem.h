#pragma once
/*
*	����汾��Dragon Engine v0.1;
*	�ࡡ������RenderSystem
*	�衡��������OpenGL��DirectX�ȵײ���Ⱦ����ĳ���Ŀǰֻ֧��OpenGL������Ҫ���ƵĶ�����й���
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
*	����汾��Dragon Engine v0.1;
*	�ࡡ������DrawList
*	�衡�����������б������������Դ��Drawer��Ӧͬһ��shader��Ϊ�˱����л�shader�����Ŀ��������������б���ʽ����
*
*/
//=========================================�ĳɽṹ����ܸ���====================================
template<typename T>
class DrawerList
{
	Shader *m_shader;				//shader
	list<T*> m_drawerList;			//drawer�б�

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

	vector<DrawerList<Drawer>> m_renderList;					//�����б�
	DrawerList<UIDrawer> m_uiRenderList;						//UI�����б�

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

	//Drawerע��
	void Register(Drawer *drawer);
	void Register(UIDrawer *drawer);

	//Shader����
	Shader* CreateShader(string shaderPath, string shaderName);
	void AddRenderList(Shader* shader, RenderType type);
	int LoadShaderSource(string fileName, char ** source);
	void AddShader(string sourcePath, ShaderType shaderType, GLenum shaderID);
	void RenderSystem::Validate(GLenum shaderID);
};