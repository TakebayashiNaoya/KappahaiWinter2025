/// <summary>
/// エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Character.h"

class Player;

class Enemy : public Character
{
protected:
	/// <summary>
	/// プレイヤーが一定距離まで近づいてきたら発見。
	/// </summary>
	/// <returns>プレイヤーが見つかった場合は true、見つからない場合は false を返します。</returns>
	bool IsPlayerFound();

	/// <summary>
	/// プレイヤーが一定範囲内にいる場合、プレイヤーに向かって走る。
	/// </summary>
	void RunToPlayer();

	/// <summary>
	/// プレイヤーが一定範囲内にいる場合、プレイヤーから逃げる。
	/// </summary>
	void RunAwayFromPlayer();

	Player* m_player = nullptr;
};