#include "stdafx.h"
#include "GameCamera.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Scene/SceneManager.h"

#if _DEBUG
#define AddjustConst
#else
#define AddjustConst const
#endif

namespace
{
	AddjustConst float CAMERA_ROTATION_SPEED = 3.0f;
}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 500.0f, -700.0f);
	m_player = FindGO<Player>("Player");

	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		2000.0f,			//カメラの移動速度の最大値。
		true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(20000.0f);

	return true;
}

void GameCamera::Update()
{
	// シーン切り替え中は更新しない。
	if (SceneManager::GetInstance()->GetIsSceneChangeRequested()) {
		return;
	}

	if (BattleManager::GetIsBattleFinish()) {
		return;
	}

	if (m_player == nullptr) {
		return;
	}

	Vector3 playerPos = m_player->GetPosition();
	Vector3 up = m_player->GetUpDirection();

	if (up.LengthSq() < 0.0001f) {
		up.Set(Vector3::Up);
	}

	// スティックによるカメラ回転
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	if (fabsf(x) > 0.01f || fabsf(y) > 0.01f)
	{
		Quaternion rotY;
		rotY.SetRotationDeg(up, CAMERA_ROTATION_SPEED * x);
		rotY.Apply(m_toCameraPos);

		Vector3 axisX;
		axisX.Cross(up, m_toCameraPos);
		axisX.Normalize();

		Quaternion rotX;
		rotX.SetRotationDeg(axisX, CAMERA_ROTATION_SPEED * y);
		rotX.Apply(m_toCameraPos);
	}

	Quaternion playerAdditionalRotation = m_player->GetAdditionalRot();
	//if (playerAdditionalRotation.a)
	{
		// キャラクターの1フレーム間の回転をカメラに適用
		playerAdditionalRotation.Apply(m_toCameraPos);
	}

	// 1. プレイヤーの足元の位置（アンカー）を計算する
	Vector3 anchorPos = playerPos; // デフォルトはプレイヤー位置
	Vector3 rayStart = playerPos + up * 50.0f;
	Vector3 rayEnd = Vector3::Zero;      // 下方向（重力方向）へレイを飛ばす
	Vector3 hitPos = Vector3::Zero;

	// 地面が見つかったら、その場所を基準にする
	if (PhysicsWorld::GetInstance()->RayTest(rayStart, rayEnd, hitPos)) {
		anchorPos = hitPos;
	}


	// 2. カメラの位置を計算する
	// 修正前： Vector3 desiredCameraPos = playerPos + m_toCameraPos;
	// 修正後： アンカー（地面）を基準にオフセットを足す
	Vector3 desiredCameraPos = anchorPos + m_toCameraPos;


	// 3. 注視点（ターゲット）を計算する
	// 修正前： Vector3 target = playerPos + up * 100.0f;
	// 修正後： アンカー（地面）を基準に高さを足す
	Vector3 target = anchorPos + up * 100.0f;


	// 4. 計算結果を反映
	m_cameraPos = desiredCameraPos;

	g_camera3D->SetUp(up);
	m_springCamera.SetPosition(m_cameraPos);
	m_springCamera.SetTarget(target);
	m_springCamera.Update();
}
