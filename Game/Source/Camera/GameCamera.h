/// <summary>
/// ゲームカメラを管理するクラス。
/// </summary>
#pragma once

class Player;

class GameCamera :public IGameObject
{
public:
public:
	void SetPlayerTarget(Player* player) { m_player = player; } // 新規追加

private:
	bool Start()override final;
	void Update()override final;

	Player* m_player = nullptr;

	Vector3 m_toCameraPos = Vector3::Zero;	// 注視点から視点までのベクトル。

	Vector3 m_cameraPos;    // 実際のカメラ位置（追従用）

};

