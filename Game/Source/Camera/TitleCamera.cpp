#include "stdafx.h"
#include "TitleCamera.h"
#include "Source/Actor/Character/Player/TitlePlayer.h"
#include "Source/Scene/SceneManager.h"


namespace
{
	const Vector3 TARGET_OFFSET = Vector3(100.0f, 100.0f, 100.0f);
	const Vector3 CAMERA_OFFSET = Vector3(-200.0f, 100.0f, 200.0f);
}

bool TitleCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(-300.0f, 500.0f, 300.0f);
	m_titlePlayer = FindGO<TitlePlayer>("TitlePlayer");

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(20000.0f);

	return true;
}

void TitleCamera::Update()
{
	// シーン切り替え中は更新しない。
	if (SceneManager::GetInstance()->GetIsSceneChangeRequested()) {
		return;
	}

	if (m_titlePlayer == nullptr) {
		return;
	}

	Vector3 playerPos = m_titlePlayer->GetPosition();

	m_cameraPos = playerPos + CAMERA_OFFSET;
	Vector3 target = playerPos + TARGET_OFFSET;

	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(target);
	g_camera3D->SetUp(Vector3::Up);
	g_camera3D->Update();
}
