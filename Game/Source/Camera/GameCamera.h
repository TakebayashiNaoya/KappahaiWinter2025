/// <summary>
/// ゲームカメラを管理するクラス。
/// </summary>
#pragma once

class Player;

class GameCamera :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;

	Player* m_player = nullptr;

	Vector3 m_toCameraPos = Vector3::Zero;	// 注視点から視点までのベクトル。
};

