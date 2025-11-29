#include "stdafx.h"
#include "SceneManager.h"
#include "Title.h"
#include "FirstStage.h"
#include "BossStage.h"


SceneManager* SceneManager::m_instance = nullptr;


namespace
{
	IScene* CreateScene(SceneID id)
	{
		switch (id)
		{
		case SceneID::Title:
			return new Title();
		case SceneID::FirstStage:
			return new FirstStage();
		case SceneID::BossStage:
			return new BossStage();
		default:
			return nullptr;
		}
	}
}


void SceneManager::Update()
{
	if (m_requestID != SceneID::None)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		// 古いシーンを削除して新しいシーンに切り替え。
		m_currentScene = CreateScene(m_requestID);
		m_currentScene->Start();
		// シーンリクエストをクリア。
		m_requestID = SceneID::None;
	}
	if (m_currentScene)
	{
		m_currentScene->Update();
	}
}




/********************************/


SceneManagerObject::SceneManagerObject()
{
	SceneManager::CreateInstance();
}


SceneManagerObject::~SceneManagerObject()
{
	SceneManager::DeleteInstance();
}


bool SceneManagerObject::Start()
{
	return true;
}


void SceneManagerObject::Update()
{
	SceneManager::GetInstance()->Update();
}