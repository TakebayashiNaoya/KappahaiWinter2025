#include "stdafx.h"
#include "Spawner.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"

namespace
{
	constexpr float RESPAWN_TIME = 5.0f;
}

Spawner::Spawner()
{
}

Spawner::~Spawner()
{
}

bool Spawner::Start()
{
	m_deformEnemy = NewGO<DeformEnemy>(0, "DeformEnemy");
	BattleManager::GetInstance()->Register(m_deformEnemy);
	m_deformEnemy->SetTRS(m_position, m_rotation, m_scale);
	return true;
}

void Spawner::Update()
{
	if (BattleManager::GetIsBattleFinish()) {
		return;
	}

	// エネミーが生きているかチェック
	if (m_deformEnemy != nullptr)
	{
		// 死んでいたら、ポインタを手放す（削除はBattleManagerがやってくれる）
		if (m_deformEnemy->IsDying()) {
			m_deformEnemy = nullptr;
		}
	}
	// エネミーがいない（死んだ後）なら、リスポーンタイマーを動かす
	else
	{
		m_respawnTimer += g_gameTime->GetFrameDeltaTime();

		if (m_respawnTimer > RESPAWN_TIME) {
			m_deformEnemy = NewGO<DeformEnemy>(0, "DeformEnemy");
			BattleManager::GetInstance()->Register(m_deformEnemy);
			m_deformEnemy->SetTRS(m_position, m_rotation, m_scale);
			m_respawnTimer = 0.0f;
		}
	}
}
