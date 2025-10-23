#include "stdafx.h"
#include "Enemy.h"
#include "Source/Actor/Character/Player/Player.h"

namespace
{
	constexpr float PLAYER_SEARCH_RADIUS = 500.0f;	// プレイヤー検出半径
	constexpr float RUN_SPEED = 5.0f;				// 走る速度
}

/// <summary>
/// プレイヤーが一定距離まで近づいてきたら発見とみなす。
/// </summary>
/// <returns>プレイヤーが見つかった場合は true、見つからない場合は false を返します。</returns>
bool Enemy::IsPlayerFound()
{
	Vector3 distanceToPlayer = m_player->GetPosition() - m_position;
	if (distanceToPlayer.Length() < PLAYER_SEARCH_RADIUS) {
		return true;
	}
	return false;
}

/// <summary>
/// プレイヤーが一定範囲内にいる場合、プレイヤーに向かって走る。
/// </summary>
void Enemy::RunToPlayer()
{
	// プレイヤーへの方向ベクトルを計算。
	Vector3 directionToPlayer = m_player->GetPosition() - m_position;
	directionToPlayer.Normalize();

	// プレイヤーへの方向ベクトルから、接線方向を取得。
	Vector3 moveDirection = ProjectOnPlane(directionToPlayer, m_directionFromPlanetCenter);
	moveDirection.Normalize();

	m_moveSpeed = moveDirection * RUN_SPEED;

	// 次の移動地点を先に計算し、これをレイのスタート地点とする。
	Vector3 rayStartPos = m_position + m_moveSpeed;
	// レイのスタート地点を少しだけ上にずらす。
	rayStartPos += m_directionFromPlanetCenter * 5.0f;

	// 次の移動地点をレイのスタート地点にして、惑星の中心に向かってレイを飛ばす。
	// そして、レイが当たった位置を次の移動地点とする。
	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}
}

/// <summary>
/// プレイヤーが一定範囲内にいる場合、プレイヤーから逃げる。
/// </summary>
void Enemy::RunAwayFromPlayer()
{
	// プレイヤーからの方向ベクトルを計算。
	Vector3 directionFromPlayer = m_position - m_player->GetPosition();
	directionFromPlayer.Normalize();

	// プレイヤーからの方向ベクトルから、接線方向を取得。
	Vector3 moveDirection = ProjectOnPlane(directionFromPlayer, m_directionFromPlanetCenter);
	moveDirection.Normalize();

	m_moveSpeed = moveDirection * RUN_SPEED;

	// 次の移動地点を先に計算
	Vector3 rayStartPos = m_position + m_moveSpeed;
	// レイのスタート地点を少しだけ上にずらす。
	rayStartPos += m_directionFromPlanetCenter * 5.0f;

	// 次の移動地点をレイのスタート地点にして、惑星の中心に向かってレイを飛ばす。
	// そして、レイが当たった位置を次の移動地点とする。
	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}
}