#include "stdafx.h"
#include "FirstStage.h"
#include "Source/Actor/Planet/FirstPlanet.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Actor/Object/Rocket/Rocket.h"
#include "LoadingScreen.h"
#include "Source/Actor/Object/Treasure/Treasure.h"
#include "Source/UI/UIGear.h"


FirstStage::FirstStage()
{

}


FirstStage::~FirstStage()
{
	DeleteGO(m_firstStage);

	BattleManager::GetInstance()->Unregister(m_uiGear);
	DeleteGO(m_uiGear);

	BattleManager::GetInstance()->Unregister(m_rocket);
	DeleteGO(m_rocket);

	for (auto treasure : m_treasures) {
		if (treasure) {
			BattleManager::GetInstance()->Unregister(treasure);
			DeleteGO(treasure);
		}
	}
}


bool FirstStage::Start()
{
	SoundManager::Play(enSoundList_FirstStageBGM, true);

	// ロード明けで音量が上がるため、一旦音量を0にしておく。
	SoundManager::SetVolume(enSoundList_FirstStageBGM, 0.0f);

	m_uiGear = NewGO<UIGear>(0, "UIGear");
	BattleManager::GetInstance()->Register(m_uiGear);

	return true;
}

void FirstStage::OnUpdate()
{
	if (BattleManager::GetIsBattleFinish()) {
		SoundManager::StopBGM(enSoundList_FirstStageBGM, 0.0f);
		return;
	}

	if (m_rocket)
	{
		if (m_rocket->GetIsGooled() && LoadingScreen::GetState() == LoadingScreen::enState_Opened) {
			LoadingScreen::StartLoading();
		}
		if (m_rocket->GetIsGooled() && LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
			SceneManager::GetInstance()->ChangeScene(SceneID::BossStage);
		}
	}
}


void FirstStage::InitLevel()
{
	m_levelRender.Init("Assets/modelData/stage/FirstPlanet/FirstPlanetLevel.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Planet")) {
			m_firstStage = NewGO<FirstPlanet>(0, "FirstPlanet");
			m_firstStage->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"player")) {
			m_player = NewGO<Player>(0, "Player");
			BattleManager::GetInstance()->Register(m_player);
			m_player->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"wolf")) {
			BasicEnemy* wolf = NewGO<BasicEnemy>(0, "BasicEnemy");
			BattleManager::GetInstance()->Register(wolf);
			wolf->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"spider")) {
			DeformEnemy* spider = NewGO <DeformEnemy>(0, "DeformEnemy");
			BattleManager::GetInstance()->Register(spider);
			spider->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"treasure")) {
			Treasure* treasure = NewGO <Treasure>(0, "Treasure");
			BattleManager::GetInstance()->Register(treasure);
			treasure->SetTRS(objData.position, objData.rotation, objData.scale);
			m_treasures.push_back(treasure);
			return true;
		}
		if (objData.EqualObjectName(L"rocket")) {
			m_rocket = NewGO <Rocket>(0, "Rocket");
			BattleManager::GetInstance()->Register(m_rocket);
			m_rocket->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		return false;

		});
}