#include "stdafx.h"
#include "Enemy.h"

Enemy::~Enemy()
{
	if (m_bodyCollider)
	{
		DeleteBodyCollider();
	}
}

/// <summary>
/// プレイヤーを追いかける方向を計算して返します。
/// </summary>
/// <returns> 追跡方向。</returns>
const Vector3 Enemy::ComputeMoveDirection() const
{
	// プレイヤーへの方向ベクトルを計算。
	Vector3 directionToPlayer = m_playerFoundPos - m_position;
	directionToPlayer.Normalize();

	// プレイヤーへの方向ベクトルから、接線方向を取得。
	Vector3 moveDirection = ProjectOnPlane(directionToPlayer, m_upDirection);
	moveDirection.Normalize();

	return moveDirection;
}