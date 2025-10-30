#include "stdafx.h"
#include "Enemy.h"
#include "Source/Actor/Character/Player/Player.h"

Enemy::~Enemy()
{
	if (m_bodyCollider != nullptr)
	{
		DeleteBodyCollider();
	}
}
