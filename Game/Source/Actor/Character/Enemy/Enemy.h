/// <summary>
/// エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Character.h"

class Enemy : public Character
{
public:
	virtual ~Enemy();

	/// <summary>
	/// プレイヤーが見つかったかどうかを取得します。
	/// </summary>
	/// <returns> プレイヤーが見つかったらtrueを返す。</returns>
	const bool& IsFoundPlayer() const
	{
		return m_isFoundPlayer;
	}


	/// <summary>
	/// プレイヤーが見つかったかどうかと、その座標を設定します。
	/// </summary>
	/// <param name="isFound"> プレイヤーが見つかったらtrueを入れる。</param>
	/// <param name="playerPos"> プレイヤー最終座標を更新する。</param>
	void SetIsFoundPlayer(const bool isFound, const Vector3& playerPos)
	{
		m_isFoundPlayer = isFound;
		m_playerFoundPos = playerPos;
	}


protected:
	bool m_isFoundPlayer = false;
	Vector3 m_playerFoundPos = Vector3::Zero;
	bool m_isDead = false;						// 死亡状態かどうか。


protected:
	/// <summary>
	/// プレイヤーを追いかける方向を計算して返します。
	/// </summary>
	/// <returns> 追跡方向。</returns>
	const Vector3 ComputeMoveDirection()const override;
};