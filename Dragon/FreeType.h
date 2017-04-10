#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>
#include <string>
#include <iostream>
#include <vector>
#include <GLM\glm.hpp>
#include "Drawer.h"


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
	unsigned int m_texID;		//����ID
	int m_bearingX;
	int m_bearingY;
	int m_width;
	int m_height;
	int m_advance;
};

//������Ⱦ������RenderSystem��������Ⱦϵͳ���
class FontRender : public Drawer
{
private:
	vector<Character> m_charSet;
	static const int m_rawFontSize = 16;

public:
	FontRender(string shaderName = "font");
	void DrawText(const string &str, vec2 position, int fontSize, vec3 color);
	void GetDimension(const string &str, int &width, int &height);
	virtual void Draw(void) = 0;
	virtual void PublicSet(void);
	virtual RenderLevel GetRenderLevel(void) { return RenderLevel::NonRender; }

private:
	void Init(void);
	
	static GraphicsBuffer *m_graphicBuffer;
};


//FreeType�����������ڼ���TrueType����
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