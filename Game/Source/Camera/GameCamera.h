/// <summary>
/// ゲームカメラを管理するクラス。
/// </summary>
#pragma once
#include "camera/SpringCamera.h"

class Player;

class GameCamera :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;

	Player* m_player = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;	// 注視点から視点までのベクトル。
	Vector3 m_cameraPos = Vector3::Zero;	// 実際のカメラ位置（追従用）
	SpringCamera m_springCamera;			// スプリングカメラ。
};

