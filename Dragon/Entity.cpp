#include "Entity.h"
#include "ResourceSystem.h"

GameEntity::GameEntity()
{

}

GameEntity::GameEntity(Transform &transform) : m_transform(transform)
{

}

Transform * GameEntity::GetTransform(void)
{
	return &m_transform;
}


//=============================================GameSpirit=================================================

GameSpirit::GameSpirit()
{
	ResourceSystem::Register(this);
}

GameSpirit::GameSpirit(Transform &transform) : GameEntity(transform)
{
	ResourceSystem::Register(this);
}