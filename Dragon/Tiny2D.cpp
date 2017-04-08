#include <GL\glew.h>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "Tiny2D.h"
#include "RenderSystem.h"

Tiny2D::Tiny2D(int scrWidth, int scrHeight, Shader *shader)
{
	//着色器
	m_shader = shader;

	//建立缓冲对象
	unsigned int *bufferObject = new unsigned int[3 * Basic2D::Basic2DCount];
	glGenBuffers(2 * Basic2D::Basic2DCount, bufferObject);
	glGenVertexArrays(Basic2D::Basic2DCount, bufferObject + 2 * Basic2D::Basic2DCount);
	for (int i = 0; i < Basic2D::Basic2DCount; ++i)
	{
		m_graphicBuffer.m_vbo.push_back(bufferObject[i]);
		m_graphicBuffer.m_ebo.push_back(bufferObject[i + Basic2D::Basic2DCount]);
		m_graphicBuffer.m_vao.push_back(bufferObject[i + 2 * Basic2D::Basic2DCount]);
	}
	delete[] bufferObject;

	m_screenWidth = scrWidth;
	m_screenHeight = scrHeight;
	m_projection = glm::ortho<float>(0.0f, (float)m_screenWidth, -(float)m_screenHeight, 0.0f);
	m_shader->SetUniformValue("projection", m_projection);

	//三角形
	m_triangle[0] = vec3(0.0f, 0.5f, 0.0f);
	m_triangle[1] = vec3(-0.5f, -0.5f, 0.0f);
	m_triangle[2] = vec3(0.5f, -0.5f, 0.0f);
	Tranglarize(m_TriangleIndex, 3);
	BufferData(Basic2D::Triangle, m_TriangleIndex.data(), sizeof(unsigned int) * m_TriangleIndex.size(), m_triangle, sizeof(vec3) * 3, BufferUsage::STATIC);

	//矩形
	m_rect[0] = vec3(0.0f, 0.0f, 0.0f);
	m_rect[1] = vec3(0.0f, -1.0f, 0.0f);
	m_rect[2] = vec3(1.0f, -1.0f, 0.0f);
	m_rect[3] = vec3(1.0f, 0.0f, 0.0f);
	Tranglarize(m_rectIndex, 4);
	BufferData(Basic2D::Rectangle, m_rectIndex.data(), sizeof(unsigned int) * m_rectIndex.size(), m_rect, sizeof(vec3) * 4, BufferUsage::STATIC);

	//圆形
	m_circle.push_back(vec3(0.0f + 1.0f, 0.0f - 1.0f, 0.0f));
	for (float angle = 0; angle < 360; angle++)
		m_circle.push_back(vec3(cos(radians(angle)) + 1.0f, sin(radians(angle)) - 1.0f, 0.0f));
	Tranglarize(m_circleIndex, m_circle.size(), true);
	BufferData(Basic2D::Circle, m_circleIndex.data(), sizeof(unsigned int) * m_circleIndex.size(), m_circle.data(), sizeof(vec3) * m_circle.size(), BufferUsage::STATIC);

	//圆角矩形
	m_roundedRect.reserve(1 + (m_divideAngle + 1) * 4);
	m_roundedRect.push_back(vec3(0.0, 0.0, 0.0));
	float curAngle = 0;
	float divAngle = 90.0 / m_divideAngle;
	for (int i = 0; i < 4; i++)
	{
		float angle = 0;
		for (int j = 0; j <= m_divideAngle; j++, angle += divAngle)
			m_roundedRect.push_back(vec3(cos(radians(curAngle + angle)), sin(radians(curAngle + angle)), 0.0f));
		curAngle += 90;
	}
	Tranglarize(m_roundedRectIndex, m_roundedRect.size(), true);
	BufferData(Basic2D::RoundedRect, m_roundedRectIndex.data(), sizeof(unsigned int) * m_roundedRectIndex.size(), nullptr, sizeof(vec3) * m_roundedRect.size(), BufferUsage::DYNAMIC);

	//字体
	m_fontRender = FontRender::GetInstance();
}

void Tiny2D::Init(int screenWidth, int screenHeight, Shader *shader)
{
	if(m_instance == nullptr)
		m_instance = new Tiny2D(screenWidth, screenHeight, shader);
}

Tiny2D * Tiny2D::GetInstance(void)
{
	if (m_instance == nullptr)
		throw exception("Error:Tiny2D-GetInstance-实例为空，Tiny2D未初始化");
	return m_instance;
}

void Tiny2D::DrawTriangle(vec2 lt, int width, int height, vec3 color)
{
	glm::mat4 model = translate(vec3(lt.x, -lt.y, 0.0f));
	model = scale(model, vec3(width, height, 1.0f));
	m_shader->SetUniformValue("model", model);
	glUniform3fv(m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(m_graphicBuffer.m_vao[Basic2D::Triangle]);
	glDrawElements(GL_TRIANGLES, m_TriangleIndex.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::DrawRect(vec2 rt, int width, int height, vec3 color)
{
	glm::mat4 model = translate(vec3(rt.x, -rt.y, 0.0f));
	model = scale(model, vec3(width, height, 1.0f));
	m_shader->SetUniformValue("model", model);
	m_shader->SetUniformValue("projection", m_projection);
	glUniform3fv(m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(m_graphicBuffer.m_vao[Basic2D::Rectangle]);
	glDrawElements(GL_TRIANGLES, m_rectIndex.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::DrawFont(const string &str, vec2 positioin, vec3 color)
{
	m_fontRender->DrawText(str, positioin, color);
}

void Tiny2D::DrawCircle(vec2 lt, int radius, vec3 color)
{
	glm::mat4 model = translate(vec3(lt.x, -lt.y, 0.0f));
	model = scale(model, vec3(radius, radius, 1.0f));
	m_shader->SetUniformValue("model", model);
	glUniform3fv(m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(m_graphicBuffer.m_vao[Basic2D::Circle]);
	glDrawElements(GL_TRIANGLES, m_circleIndex.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::DrawRoundRect(vec2 lt, int width, int height, int radius, vec3 color)
{
	static vector<vec3> position;
	position.clear();
	int diameter = radius * 2;
	if (diameter > height || diameter > width)
		throw exception("圆角矩形radius > width || height");

	float halfW = width / 2;
	float halfH = height / 2;
	static vec2 offset[4]{
		vec2(1, 1),
		vec2(-1, 1),
		vec2(-1, -1),
		vec2(1, -1)
	};
	position.push_back(vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < 4; i++)
	{
		int startIndex = i * (m_divideAngle + 1) + 1;
		for (int j = 0; j <= m_divideAngle; j++)
			position.push_back(vec3(radius * m_roundedRect[startIndex + j].x + offset[i].x * (halfW - radius), radius * m_roundedRect[startIndex + j].y + offset[i].y * (halfH - radius), 0.0f));
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_graphicBuffer.m_vbo[Basic2D::RoundedRect]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * position.size(), position.data());
	
	m_shader->Use();
	glm::mat4 model = translate(vec3(halfW + lt.x, -(halfH + lt.y), 0.0f));
	m_shader->SetUniformValue("model", model);
	glUniform3fv(m_shader->GetUniformLocation("color"), 1, value_ptr(color));
	glBindVertexArray(m_graphicBuffer.m_vao[Basic2D::RoundedRect]);
	glDrawElements(GL_TRIANGLES, m_roundedRectIndex.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Tiny2D::Tranglarize(vector<unsigned int> &index, int count, bool isClose)
{
	for (int i = 1; i < count - 1; i++)
	{
		index.push_back(0);
		index.push_back(i);
		index.push_back(i + 1);
	}
	if (isClose)
	{
		index.push_back(0);
		index.push_back(count-1);
		index.push_back(1);
	}
}

void Tiny2D::BufferData(Basic2D basic2D, void *index, int indexSize, void *buffer, int bufferSize, BufferUsage bufferUsage)
{
	static int gl_BufferUsage[3]{ GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW };
	glBindVertexArray(m_graphicBuffer.m_vao[basic2D]);
	glBindBuffer(GL_ARRAY_BUFFER, m_graphicBuffer.m_vbo[basic2D]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_graphicBuffer.m_ebo[basic2D]);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, gl_BufferUsage[bufferUsage]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, index, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Tiny2D *Tiny2D::m_instance = nullptr;