#pragma once
#include <GLM\common.hpp>
#include <string>
#include <vector>
#include "Drawer.h"
#include "Shader.h"
#include "FreeType.h"
#include "CommonType.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using std::string;
using std::vector;

/*
*	引擎版本：Dragon Engine v0.1;
*	类　　名：Tiny2D
*	描　　述：2D图元、图像的绘制工具类, 相当Drawer
*
*/
//单例模式
class Tiny2D : public Drawer
{	
	enum BufferUsage { STATIC, DYNAMIC, STREAM };
	enum Basic2D{Triangle, Rectangle, RoundedRect, Circle, Text, Basic2DCount};
public:
	static void Init(int screenWidth, int screenHeight, Shader*);
	static Tiny2D* GetInstance(void);

	//绘制函数
	void DrawTriangle(vec2 lt, int width, int height, vec3 color);
	void DrawRect(vec2 rt, int width, int height, vec3 color);
	void DrawFont(const string &str, vec2 positioin, vec3 color);
	void DrawCircle(vec2 lt, int radius, vec3 color);
	void DrawRoundRect(vec2 lt, int width, int height, int radius, vec3 color);

private:
	Tiny2D(int scrWidth, int scrHeight, Shader*);
	//工具函数
	void Tranglarize(vector<unsigned int> &index, int count, bool isClose=false);
	void BufferData(Basic2D basic2D, void *index, int indexSize, void *buffer, int bufferSize, BufferUsage bufferUsage);

private:
	Shader *m_shader;						//Tiny2D包含Shader，也是对Shader的扩展，概念上和Drawer一样
	GraphicsBuffer m_graphicBuffer;
	int m_screenWidth, m_screenHeight;
	mat4 m_projection;
	static Tiny2D *m_instance;
	

	//三角形
	vec3 m_triangle[3];
	vector<unsigned int> m_TriangleIndex;

	//矩形
	vec3 m_rect[4];
	vector<unsigned int> m_rectIndex;

	//圆
	vector<vec3> m_circle;
	vector<unsigned int> m_circleIndex;

	//圆角矩形
	vector<vec3> m_roundedRect;
	vector<unsigned int> m_roundedRectIndex;
	const int m_divideAngle = 5;

	//字体渲染器
	FontRender *m_fontRender;
};