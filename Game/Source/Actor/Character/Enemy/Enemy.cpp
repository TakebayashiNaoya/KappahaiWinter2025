#include "stdafx.h"
#include "Enemy.h"
#include "Source/Actor/Character/Player/Player.h"

namespace
{
	constexpr float GRAVITY_POWER = 9.8f;			// 重力。
	constexpr float RUN_SPEED = 3.0f;				// 走る速度
}

Enemy::Enemy()
{
}

/// <summary>
/// プレイヤーが一定範囲内にいる場合、プレイヤーに向かって走る。
/// </summary>
void Enemy::RunToPlayer()
{
	// プレイヤーへの方向ベクトルを計算。
	Vector3 directionToPlayer = m_playerFoundPos - m_position;
	directionToPlayer.Normalize();

	// プレイヤーへの方向ベクトルから、接線方向を取得。
	Vector3 moveDirection = ProjectOnPlane(directionToPlayer, m_upDirection);
	moveDirection.Normalize();

	m_moveSpeed += moveDirection * RUN_SPEED;


	///--- ジャンプ・重力処理 ---///
	// 空中の移動速度の計算。
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//速度 = 初速度 - 重力 * 時間。
	float jumpPower = m_initialJumpSpeed - (GRAVITY_POWER * m_fallTimer);

	m_moveSpeed += m_upDirection * jumpPower;	// 垂直方向に加算。


	///--- 移動処理 ---///
	Vector3 rayStartPos = m_position + m_upDirection * 10.0f;	// 少し上からレイを飛ばす。
	Vector3 rayEndPos = m_position + m_moveSpeed;							// 移動先までレイを飛ばす。

	// レイが地面に当たったら、その位置に移動させる。
	Vector3 hitPos = Vector3::Zero;
	if (PhysicsWorld::GetInstance()->RayTest(rayStartPos, rayEndPos, hitPos)) {
		// 地面にぶつかった
		m_position = hitPos;
		// ジャンプ終了
		m_initialJumpSpeed = 0.0f;
		m_fallTimer = 0.0f;
		return;
	}
	// 地面にぶつからなかったら、そのまま移動させる。
	m_position = rayEndPos;
}

/// <summary>
/// プレイヤーが一定範囲内にいる場合、プレイヤーから逃げる。
/// </summary>
void Enemy::RunAwayFromPlayer()
{
	// プレイヤーからの方向ベクトルを計算。
	Vector3 directionFromPlayer = m_position - m_playerFoundPos;
	directionFromPlayer.Normalize();

	// プレイヤーからの方向ベクトルから、接線方向を取得。
	Vector3 moveDirection = ProjectOnPlane(directionFromPlayer, m_upDirection);
	moveDirection.Normalize();

	m_moveSpeed = moveDirection * RUN_SPEED;

	// 次の移動地点を先に計算
	Vector3 rayStartPos = m_position + m_moveSpeed;
	// レイのスタート地点を少しだけ上にずらす。
	rayStartPos += m_upDirection * 5.0f;

	// 次の移動地点をレイのスタート地点にして、惑星の中心に向かってレイを飛ばす。
	// そして、レイが当たった位置を次の移動地点とする。
	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}
}

bool Enemy::IsStomped()
{
	return false;
}
