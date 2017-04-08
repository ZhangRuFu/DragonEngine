#include <iostream>

#include "RenderSystem.h"
#include "Shader.h"
#include "Tiny2D.h"
#include "FreeType.h"


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using std::cout;
using std::endl;

RenderSystem::RenderSystem(int frameWidth, int frameHeight) : m_uiRenderList(nullptr)
{
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	bool result = Init();
	if (result)
		cout << "DragonEngine->SUCCESS:渲染引擎初始化完成!" << endl;
	else
		cout << "DragonEngine->ERROR:RenderSystem::Init():渲染引擎初始化失败!" << endl;
}

RenderSystem* RenderSystem::GetInstance(int frameWidth, int frameHeight)
{
	if (m_instance == nullptr)
		m_instance = new RenderSystem(frameWidth, frameHeight);
	return m_instance;

}

bool RenderSystem::Init()
{
	//OpenGL版本号
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "OpenGL版本:" << version << endl;

	//GLEW初始化
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		cout << "DragonEngine->ERROR:GLEW初始化错误!错误信息:" << glewGetErrorString(result) << endl;
		return false;
	}
	else
		cout << "DragonEngine->SUCCESS:GLEW初始化成功!" << endl;

	//OpenGL初始化
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, m_frameWidth, m_frameHeight);
	glEnable(GL_DEPTH_TEST);

	//加载必要Shader=========================改成XML加载====================================
	AddRenderList(CreateShader("E:\\OpenGL\\Shader\\Assimp\\", "AssimpModel"), RenderType::Entity);
	AddRenderList(CreateShader("E:\\OpenGL\\Shader\\ModelRender\\PureColor\\", "SimpleModel"), RenderType::Entity);
	AddRenderList(CreateShader("E:\\OpenGL\\Shader\\ModelRender\\Animation\\", "AnimationModel"), RenderType::Entity);
	
	//Tiny2D Shader
	AddRenderList(CreateShader("E:\\OpenGL\\Shader\\2D\\", "2D"), RenderType::UI);

	//Tiny2D初始化
	FontRender::Init(m_frameWidth, m_frameHeight, CreateShader("E:\\OpenGL\\Shader\\Font\\", "font"));
	Tiny2D::Init(m_frameWidth, m_frameHeight, m_uiRenderList.m_shader);
	return true;
}

void RenderSystem::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < m_renderList.size(); i++)
	{
		Shader *shader = m_renderList[i].m_shader;
		shader->Use();
		list<Drawer*> &drawerList = m_renderList[i].m_drawerList;
		for (list<Drawer*>::iterator iterator = drawerList.begin(); iterator != drawerList.end(); iterator++)
		{
			//=====================================PublicSet多次！=============================================
			if (iterator == drawerList.begin())
				(*iterator)->PublicSet();
			(*iterator)->Draw();
		}
	}

	//UI绘制
	m_uiRenderList.m_shader->Use();
	for (list<UIDrawer*>::const_iterator i = m_uiRenderList.m_drawerList.begin(); i != m_uiRenderList.m_drawerList.end(); i++)
		(*i)->Draw();
}

void RenderSystem::ReSize(int frameWidth, int frameHeight)
{
	m_frameWidth = frameWidth; m_frameHeight = frameHeight;
	glViewport(0, 0, m_frameWidth, m_frameHeight);
}

void RenderSystem::Register(Drawer *drawer)
{
	if (drawer == nullptr)
	{
		cout << "DragonEngine->ERROR:RenderSystem::Register, 待注册的Drawer为null" << endl;
		return;
	}
	string shaderName = drawer->GetShader()->GetShaderName();

	if (m_shaderMap.find(shaderName) == m_shaderMap.end())
	{
		cout << "DragonEngine->ERROR:RenderSystem::Register, 未知Shader" << shaderName << endl;
		return;
	}
	unsigned int index = m_shaderMap[shaderName];
	m_renderList[index].m_drawerList.push_back(drawer);
}

void RenderSystem::Register(UIDrawer * drawer)
{
	if (drawer == nullptr)
	{
		cout << "DragonEngine->ERROR:RenderSystem::Register, 待注册的Drawer为null" << endl;
		return;
	}
	m_uiRenderList.m_drawerList.push_back(drawer);
}

unsigned int RenderSystem::CreateTexture(TextureInfo &info)
{
	GLenum texID;
	GLint format = info.m_isAlpha ? GL_RGBA : GL_RGB;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, info.m_isAlpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, info.m_isAlpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, info.m_width, info.m_height, 0, format, GL_UNSIGNED_BYTE, info.m_imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

unsigned int RenderSystem::CreateCubeTexture(vector<TextureInfo> *info)
{
	GLenum cubeTex;
	if (info->size() != 6)
		return -1;

	glGenTextures(1, &cubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	for (unsigned int i = 0; i < info->size(); i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, (*info)[i].m_width, (*info)[i].m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, (*info)[i].m_imageData);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return cubeTex;
}

Shader* RenderSystem::LoadShader(string shaderName)
{
	if (m_instance->m_shaderMap.find(shaderName) != m_instance->m_shaderMap.end())
		return m_instance->m_renderList[m_instance->m_shaderMap[shaderName]].m_shader;
	if (m_instance->m_uiRenderList.m_shader->GetShaderName() == shaderName)
		return m_instance->m_uiRenderList.m_shader;
	cout << "DragonEngine->ERROR:RenderSystem::LoadShader:未知的着色器\n";
	return nullptr;
}

void RenderSystem::RegisterDrawer(Drawer * drawer)
{
	m_instance->Register(drawer);
}

void RenderSystem::RegisterDrawer(UIDrawer * uiDrawer)
{
	m_instance->Register(uiDrawer);
}

Shader* RenderSystem::CreateShader(string shaderPath, string shaderName)
{
	GLenum shaderID = glCreateProgram();
	if (!shaderID)
	{
		cout << "DragonEngine->ERROR:RenderSystem::CreateShader:着色器程序创建失败\n";
		return false;
	}
	string shaderFullPath = shaderPath + shaderName;
	AddShader(shaderFullPath + ".vs", ShaderType::VertexShader, shaderID);
	AddShader(shaderFullPath + ".fs", ShaderType::FragmentShader, shaderID);
	Validate(shaderID);
	Shader *shader = new Shader(shaderID, shaderName);

	return shader;
}

void RenderSystem::AddRenderList(Shader * shader, RenderType type)
{
	//注册Shader
	string shaderName = shader->GetShaderName();
	switch (type)
	{
	case RenderSystem::Entity:
		m_shaderMap[shaderName] = m_renderList.size();
		m_renderList.push_back(DrawerList<Drawer>(shader));
		break;
	case RenderSystem::UI:
		m_uiRenderList.m_shader = shader;
		break;
	default:
		return;
	}
	cout << "DragonEngine->SUCCESS:已加载[" << shaderName << "]着色器\n";
}

int RenderSystem::LoadShaderSource(string fileName, char ** source)
{
	FILE *fp = nullptr;
	errno_t result = fopen_s(&fp, fileName.c_str(), "r");
	if (result)
		cout << "Shader文件打开错误!\n";
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	rewind(fp);
	*source = new char[len + 1]();
	fread(*source, len, 1, fp);
	len = strlen(*source);
	return len;
}

void RenderSystem::AddShader(string sourcePath, ShaderType shaderType, GLenum shaderID)
{
	static char* shaderName[2]{ "片段", "顶点" };
	GLenum subShaderID = glCreateShader(shaderType);
	if (!subShaderID)
	{
		cout << "DragonEngine->ERROR:RenderSystem::AddShader, 子着色器创建失败";
		return;
	}

	char *source = nullptr;
	int len = LoadShaderSource(sourcePath, &source);
	glShaderSource(subShaderID, 1, &source, &len);
	glCompileShader(subShaderID);
	GLint isSuccess = 0;
	char info[128];
	glGetShaderiv(subShaderID, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetShaderInfoLog(subShaderID, sizeof(info), nullptr, info);
		cout << "DragonEngine->ERROR:RenderSystem::AddShader, "<< shaderName[shaderType - ShaderType::FragmentShader] << "着色器编译错误，错误信息:\n" << info << endl;
		cout << "着色器路径:" << sourcePath << endl;
		return;
	}
	glAttachShader(shaderID, subShaderID);
	delete[] source;
}

void RenderSystem::Validate(GLenum shaderID)
{
	GLint isSuccess = 0;
	char info[128];
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, info);
		cout << "着色器链接错误，错误信息:\n" << info << endl;
	}
	glValidateProgram(shaderID);
}

template<typename T> DrawerList<T>::DrawerList(Shader * shader) : m_shader(shader){ }

RenderSystem * RenderSystem::m_instance = nullptr;