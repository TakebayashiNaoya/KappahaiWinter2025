#include "stdafx.h"
#include "Spawner.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Battle/BattleManager.h"

namespace
{
	constexpr float RESPAWN_TIME = 5.0f;
}

Spawner::Spawner()
{
}

Spawner::~Spawner()
{
	if (m_deformEnemy) {
		DeleteGO(m_deformEnemy);
	}
}

bool Spawner::Start()
{
	m_deformEnemy = NewGO<DeformEnemy>(0, "DeformEnemy");
	m_deformEnemy->SetTRS(m_position, m_rotation, m_scale);
	return true;
}

void Spawner::Update()
{
	if (BattleManager::IsBattleFinish()) {
		return;
	}

	if (m_deformEnemy->IsDying())
	{
		m_respawnTimer += g_gameTime->GetFrameDeltaTime();

		if (m_respawnTimer > RESPAWN_TIME) {
			m_deformEnemy = NewGO<DeformEnemy>(0, "DeformEnemy");
			m_deformEnemy->SetTRS(m_position, m_rotation, m_scale);
			m_respawnTimer = 0.0f;
		}
	}
}
