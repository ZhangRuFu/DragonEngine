#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>
#include <string>
#include <iostream>
#include <vector>
#include <GLM\glm.hpp>


using std::exception;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using glm::mat4;
using glm::vec2;
using glm::vec3;

class Shader;

struct Character
{
	unsigned int m_texID;		//纹理ID
	int m_bearingX;
	int m_bearingY;
	int m_width;
	int m_height;
	int m_advance;
};

//字体渲染器，由RenderSystem管理，与渲染系统相关
class FontRender
{
private:
	Shader *m_fontShader;
	mat4 m_windowProjection;
	vector<Character> m_charSet;

	GLenum m_fontVBO;
	GLenum m_fontVAO;
	static FontRender *m_instance;

public:
	static FontRender* GetInstance(void)
	{
		if (m_instance == nullptr)
			throw exception("ERROR:FontRender未建立实例!");
		return m_instance;
	}
	static void Init(int windowW, int windowH, Shader *shader)
	{
		if (m_instance == nullptr)
			m_instance = new FontRender(windowW, windowH, shader);
	}
	void ModifyWindow(int windowWidth, int windowHeight);
	void DrawText(const string &str, vec2 position, vec3 color);

private:
	FontRender(int windowWidth, int windowHeight, Shader *shader);
	void Init(void);
};


//FreeType加载器，用于加载TrueType数据
class FreeType
{
public:
	FreeType(string fontPath, int height, int width=0);
	~FreeType(void);
	void GetCharacters(vector<Character> &charSet, vector<const unsigned char*> &buffer);

private:
	FT_Library m_ftLibrary;
	FT_Face m_ftFace;
	string m_fontPath;
	int m_fontWidth, m_fontHeight;
	static const int s_charCount = 128;
};