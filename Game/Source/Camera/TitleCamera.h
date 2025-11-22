/// <summary>
/// ゲームカメラを管理するクラス。
/// </summary>
#pragma once

class TitlePlayer;

class TitleCamera :public IGameObject
{
private:
	bool Start()override final;
	void Update()override final;

	TitlePlayer* m_titlePlayer = nullptr;
	Vector3 m_toCameraPos = Vector3::Zero;	// 注視点から視点までのベクトル。
	Vector3 m_cameraPos = Vector3::Zero;	// 実際のカメラ位置（追従用）

};

