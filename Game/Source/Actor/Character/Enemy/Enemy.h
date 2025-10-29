/// <summary>
/// エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Character.h"

class Enemy : public Character
{
public:
	Enemy();

	/// <summary>
	/// プレイヤーが見つかったかどうかを取得します。
	/// </summary>
	/// <returns> プレイヤーが見つかったらtrueを返す。</returns>
	const bool& GetIsFoundPlayer() const
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

	/// <summary>
	/// プレイヤーに踏まれているかを判定します。
	/// </summary>
	/// <returns> 踏まれていればtrue、踏まれていなければfalseを返します。</returns>
	bool IsStomped();

	bool m_isFoundPlayer = false;
	Vector3 m_playerFoundPos = Vector3::Zero;
};