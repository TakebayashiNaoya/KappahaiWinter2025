#include "stdafx.h"
#include "GameCamera.h"
#include "Source/Actor/Character/Player/Player.h"

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

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(20000.0f);

	return true;
}

void GameCamera::Update()
{
	Vector3 playerPos = m_player->GetPosition();
	Vector3 up = m_player->GetDirectionFromPlanetCenter();

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

	// プレイヤーの位置と回転後の相対ベクトルからカメラの位置を計算
	Vector3 desiredCameraPos = playerPos + m_toCameraPos;

	// カメラ位置の更新
	m_cameraPos = desiredCameraPos;

	// 注視点の計算
	Vector3 target = playerPos + up * 80.0f;

	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(target);
	g_camera3D->SetUp(up);
	g_camera3D->Update();
}
