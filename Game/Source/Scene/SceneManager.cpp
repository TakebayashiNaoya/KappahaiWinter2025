#include "stdafx.h"
#include "SceneManager.h"
#include "Title.h"
#include "InGame.h"
#include "GameClear.h"
#include "GameOver.h"

namespace
{
	IScene* CreateScene(SceneID id)
	{
		switch (id)
		{
		case SceneID::Title:
			return new Title();
		case SceneID::InGame:
			return new InGame();
		case SceneID::GameClear:
			return new GameClear();
		case SceneID::GameOver:
			return new GameOver();
		default:
			return nullptr;
		}
	}
}

void SceneManager::Update()
{
	if (m_requestID != SceneID::None)
	{
		IScene* nextScene = CreateScene(m_requestID);
		nextScene->Start();
		// 古いシーンを削除して新しいシーンに切り替え。
		delete m_currentScene;
		m_currentScene = nextScene;
		// シーンリクエストをクリア。
		m_requestID = SceneID::None;
	}
	if (m_currentScene)
	{
		m_currentScene->Update();
	}
}
