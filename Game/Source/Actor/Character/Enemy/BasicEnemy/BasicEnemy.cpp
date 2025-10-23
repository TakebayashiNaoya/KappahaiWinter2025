#include "stdafx.h"
#include "BasicEnemy.h"
#include "BasicEnemyStateMachine.h"

BasicEnemy::BasicEnemy()
{
	m_stateMachine = std::make_unique<app::basicEnemy::BasicEnemyStateMachine>(this);
}

bool BasicEnemy::Start()
{
	return false;
}

void BasicEnemy::Update()
{
}

void BasicEnemy::Render(RenderContext& rc)
{
}
