/// <summary>
/// エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Character.h"

class Enemy : public Character
{
public:
	~Enemy();

	/// <summary>
	/// プレイヤーが見つかったかどうかを取得します。
	/// </summary>
	/// <returns> プレイヤーが見つかったらtrueを返す。</returns>
	const bool& GetIsFoundPlayer() const
	{
		return m_isFoundPlayer;
	}

	/// <summary>
	/// 死亡状態を取得します。
	/// </summary>
	/// <returns> 死亡している場合はtrue、そうでない場合はfalseを返します。</returns>
	const bool GetIsDead() const
	{
		return m_isDead;
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

	/// <summary>
	/// 死亡状態を設定します。
	/// </summary>
	/// <param name="isDead"> true の場合は死亡、false の場合は生存を表します。</param>
	void SetIsDead(const bool isDead)
	{
		m_isDead = isDead;
	}

	bool m_isFoundPlayer = false;
	Vector3 m_playerFoundPos = Vector3::Zero;
	bool m_isDead = false;						// 死亡状態かどうか。

protected:
	/// <summary>
	/// エネミー削除の処理を行います。
	/// </summary>
	virtual void DeleteEnemy() = 0;
};