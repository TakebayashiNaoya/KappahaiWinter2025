#include "stdafx.h"
#include "TransformEnemy.h"
#include "TransformEnemyStateMachine.h"

TransformEnemy::TransformEnemy()
{
	m_stateMachine = std::make_unique<app::transformEnemy::TransformEnemyStateMachine>(this);
}

bool TransformEnemy::Start()
{
	return false;
}

void TransformEnemy::Update()
{
}

void TransformEnemy::Render(RenderContext& rc)
{
}
