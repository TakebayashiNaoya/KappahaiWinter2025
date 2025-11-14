#include "stdafx.h"
#include "BossEnemy.h"
#include "BossEnemyStateMachine.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption BossEnemy::BOSS_ENEMY_ANIMATION_OPTIONS[] = {
   {"Wolf/idle",	true},
   {"Wolf/walk", true},
   {"Wolf/run",	true},
};

BossEnemy::BossEnemy()
{
	m_stateMachine = std::make_unique<app::bossEnemy::BossEnemyStateMachine>(this);
}

BossEnemy::~BossEnemy()
{
	if (m_bodyCollider)
	{
		DeleteBodyCollider();
	}
}

void BossEnemy::DeleteEnemy()
{
	// コライダーを削除。
	DeleteBodyCollider();
	// 自分自身を削除。
	DeleteGO(this);
}

bool BossEnemy::Start()
{
	return true;
}

void BossEnemy::Update()
{
}

void BossEnemy::Render(RenderContext& rc)
{
}
