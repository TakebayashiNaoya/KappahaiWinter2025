#include "stdafx.h"
#include "LoadingScreen.h"

LoadingScreen* LoadingScreen::m_instance = nullptr;

namespace
{
	constexpr float FULL_HD_WIDTH = 1920.0f;						// フルHDの幅
	constexpr float FULL_HD_HEIGHT = 1080.0f;						// フルHDの高さ

	constexpr float HALF_FULL_HD_WIDTH = FULL_HD_WIDTH / 2.0f;		// フルHDの半分の幅
	constexpr float HALF_FULL_HD_HEIGHT = FULL_HD_HEIGHT / 2.0f;	// フルHDの半分の高さ

	constexpr float IMAGE_RATIO = 5.0f;								// 周囲画像の幅・高さの倍率

	const float IMAGE_CENTER_WIDTH = FULL_HD_WIDTH * IMAGE_RATIO;	// 中央画像の幅
	const float IMAGE_CENTER_HEIGHT = FULL_HD_HEIGHT * IMAGE_RATIO;	// 中央画像の高さ

	const Vector2 PIVOT_TOP_CENTER = { 0.5f, 1.0f };				// 上辺の中央
	const Vector2 PIVOT_BOTTOM_CENTER = { 0.5f, 0.0f };				// 下辺の中央
	const Vector2 PIVOT_CENTER_LEFT = { 0.0f, 0.5f };				// 左辺の中央
	const Vector2 PIVOT_CENTER_RIGHT = { 1.0f, 0.5f };				// 右辺の中央
	const Vector2 PIVOT_CENTER = { 0.5f, 0.5f };					// 完全な中央

	const float CROSE_DURATION = 1.5f;								// クローズの時間
	const float OPEN_DURATION = 1.5f;								// オープンの時間
}


LoadingScreen::LoadingScreen()
{
	m_displayImages = new SpriteRender[enImageParts_Num];

	// レイアウト初期化。
	InitLayout();

	// 画像を更新。
	for (int i = 0; i < enImageParts_Num; ++i) {
		m_displayImages[i].Update();
	}
	m_timer = 0.0f;
}


LoadingScreen::~LoadingScreen()
{
	if (m_displayImages != nullptr) {
		delete[] m_displayImages;
		m_displayImages = nullptr;
	}
}


bool LoadingScreen::Start()
{
	return true;
}


void LoadingScreen::Update()
{
	switch (m_state)
	{
	case enState_Close:
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
		// CROSE_DURATIONの時間をかけて0.0f→1.0fへ変化させる。
		float ratio = m_timer / CROSE_DURATION;
		// 0.0f→1.0fへの変化を1.0f→0.0fへ変化させる。
		float invRatio = 1.0f - ratio;

		// 中央画像
		m_displayImages[enImageParts_Center].SetScale({ invRatio, invRatio, 1.0f });

		// 周囲画像
		// Top (↓方向へ移動)
		m_displayImages[enImageParts_Top].SetPosition({ 0.0f, HALF_FULL_HD_HEIGHT * IMAGE_RATIO * invRatio, 0.0f });
		// Bottom (↑方向へ移動)
		m_displayImages[enImageParts_Bottom].SetPosition({ 0.0f, HALF_FULL_HD_HEIGHT * IMAGE_RATIO * invRatio * -1.0f, 0.0f });
		// Left (→方向へ移動)
		m_displayImages[enImageParts_Left].SetPosition({ HALF_FULL_HD_WIDTH * IMAGE_RATIO * invRatio * -1.0f, 0.0f, 0.0f });
		// Right (←方向へ移動)
		m_displayImages[enImageParts_Right].SetPosition({ HALF_FULL_HD_WIDTH * IMAGE_RATIO * invRatio, 0.0f, 0.0f });

		// 画像を更新。
		for (int i = 0; i < enImageParts_Num; ++i) {
			m_displayImages[i].Update();
		}

		// 画像の移動・縮小が完了したら、ロード状態へ移行。
		if (m_timer >= CROSE_DURATION) {
			m_timer = 0.0f;
			m_state = enState_Loading;
		}
		break;
	}

	case enState_Loading:
	{
		break;
	}

	case enState_Open:
	{
		// NOTE:最初の実行時、m_displayImagesがnullptrのため。
		if (m_displayImages == nullptr) {
			break;
		}

		m_timer += g_gameTime->GetFrameDeltaTime();
		// OPEN_DURATIONの時間をかけて0.0f→1.0fへ変化させる。
		float ratio = m_timer / OPEN_DURATION;

		// 中央画像
		m_displayImages[enImageParts_Center].SetScale({ ratio, ratio, 1.0f });
		// 周囲画像
		// Top (↑方向へ移動)
		m_displayImages[enImageParts_Top].SetPosition({ 0.0f, HALF_FULL_HD_HEIGHT * IMAGE_RATIO * ratio, 0.0f });
		// Bottom (↓方向へ移動)
		m_displayImages[enImageParts_Bottom].SetPosition({ 0.0f, HALF_FULL_HD_HEIGHT * IMAGE_RATIO * ratio * -1.0f, 0.0f });
		// Left (←方向へ移動)
		m_displayImages[enImageParts_Left].SetPosition({ HALF_FULL_HD_WIDTH * IMAGE_RATIO * ratio * -1.0f, 0.0f, 0.0f });
		// Right (→方向へ移動)
		m_displayImages[enImageParts_Right].SetPosition({ HALF_FULL_HD_WIDTH * IMAGE_RATIO * ratio, 0.0f, 0.0f });

		// 画像を更新。
		for (int i = 0; i < enImageParts_Num; ++i) {
			m_displayImages[i].Update();
		}

		// 画像の移動・拡大が完了したら、ロード状態へ移行。
		if (m_timer >= OPEN_DURATION) {
			m_timer = 0.0f;
			m_state = enState_Opened;
			break;
		}

		break;
	}

	case enState_Opened:
	{
		break;
	}

	default:
	{
		break;
	}
	}
}

void LoadingScreen::Render(RenderContext& rc)
{
	if (m_displayImages == nullptr) {
		return;
	}
	for (int i = 0; i < enImageParts_Num; ++i) {
		m_displayImages[i].Draw(rc);
	}

	if (m_state == enState_Loading) {
		m_loadingIcon.Draw(rc);
	}
}

void LoadingScreen::InitLayout()
{
	m_displayImages[enImageParts_Center].Init("Assets/Sprite/LoadingImage_Center.dds", IMAGE_CENTER_WIDTH, IMAGE_CENTER_HEIGHT);

	m_displayImages[enImageParts_Top].Init("Assets/Sprite/LoadingImage_Around.dds", FULL_HD_WIDTH, HALF_FULL_HD_HEIGHT);
	m_displayImages[enImageParts_Top].SetPivot(PIVOT_BOTTOM_CENTER);
	m_displayImages[enImageParts_Top].SetPosition({ 0.0f, HALF_FULL_HD_HEIGHT * IMAGE_RATIO, 0.0f });

	m_displayImages[enImageParts_Bottom].Init("Assets/Sprite/LoadingImage_Around.dds", FULL_HD_WIDTH, HALF_FULL_HD_HEIGHT);
	m_displayImages[enImageParts_Bottom].SetPivot(PIVOT_TOP_CENTER);
	m_displayImages[enImageParts_Bottom].SetPosition({ 0.0f, HALF_FULL_HD_HEIGHT * IMAGE_RATIO * -1.0f, 0.0f });

	m_displayImages[enImageParts_Left].Init("Assets/Sprite/LoadingImage_Around.dds", HALF_FULL_HD_WIDTH, FULL_HD_HEIGHT);
	m_displayImages[enImageParts_Left].SetPivot(PIVOT_CENTER_RIGHT);
	m_displayImages[enImageParts_Left].SetPosition({ HALF_FULL_HD_WIDTH * IMAGE_RATIO * -1.0f, 0.0f, 0.0f });

	m_displayImages[enImageParts_Right].Init("Assets/Sprite/LoadingImage_Around.dds", HALF_FULL_HD_WIDTH, FULL_HD_HEIGHT);
	m_displayImages[enImageParts_Right].SetPivot(PIVOT_CENTER_LEFT);
	m_displayImages[enImageParts_Right].SetPosition({ HALF_FULL_HD_WIDTH * IMAGE_RATIO, 0.0f, 0.0f });

	m_loadingIcon.Init("Assets/Sprite/Loading.dds", FULL_HD_WIDTH, FULL_HD_HEIGHT);
}




/********************************/


LoadingScreenObject::LoadingScreenObject()
{
	LoadingScreen::CreateInstance();
}


LoadingScreenObject::~LoadingScreenObject()
{
	LoadingScreen::Delete();
}


bool LoadingScreenObject::Start()
{
	return true;
}


void LoadingScreenObject::Update()
{
	LoadingScreen::GetInstance()->Update();
}


void LoadingScreenObject::Render(RenderContext& rc)
{
	LoadingScreen::GetInstance()->Render(rc);
}