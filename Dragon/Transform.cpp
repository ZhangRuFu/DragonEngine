#include <GLM\gtc\matrix_transform.hpp>
#include "Transform.h"

Transform::Transform(vec3 position, vec3 rotation, vec3 scale) : m_position(position), m_rotation(rotation), m_scale(scale), m_orientation(0, 0, -1)
{

}

void Transform::Move(float speed)
{
	m_position += speed * m_orientation;
}

void Transform::Move(vec3 step)
{
	m_position += step;
}

void Transform::ChangeOrientation(float angle)
{
	mat4 rotate;
	rotate = glm::rotate(rotate, angle, vec3(0, 1, 0));
	m_orientation = vec3(rotate * vec4(m_orientation, 0.0));
	m_rotation.y += angle;
}

void Transform::Scale(vec3 & scale)
{
	m_scale = scale;
}

vec3 Transform::getPosition(void)
{
	return m_position;
}

vec3 Transform::getRotation(void)
{
	return m_rotation;
}

vec3 Transform::getScale(void)
{
	return m_scale;
}

vec3 Transform::getOrientation(void)
{
	return m_orientation;
}
