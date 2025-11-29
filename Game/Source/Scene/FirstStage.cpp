#include "stdafx.h"
#include "FirstStage.h"
#include "Source/Actor/Planet/FirstPlanet.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/UI/UIInGame.h"
#include "Source/Battle/BattleManager.h"
#include "Source/Collision/CollisionManager.h"
#include "Source/Actor/Object/Rocket/Rocket.h"
#include "LoadingScreen.h"


FirstStage::FirstStage()
{

}


FirstStage::~FirstStage()
{
	DeleteGO(m_firstStage);
	DeleteGO(m_rocket);

	for (auto basicEnemy : m_basicEnemies) {
		if (basicEnemy) {
			DeleteGO(basicEnemy);
		}
	}

	for (auto deformEnemy : m_deformEnemies) {
		if (deformEnemy) {
			DeleteGO(deformEnemy);
		}
	}
}


bool FirstStage::Start()
{
	return true;
}

void FirstStage::OnUpdate()
{
	if (m_rocket)
	{
		if (m_rocket->IsGooled() && LoadingScreen::GetState() == LoadingScreen::enState_Opened) {
			LoadingScreen::StartLoading();
		}
		if (m_rocket->IsGooled() && LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
			SceneManager::GetInstance()->ChangeScene(SceneID::BossStage);
		}
	}
}


void FirstStage::InitLevel()
{
	m_levelRender.Init("Assets/modelData/stage/FirstPlanet/FirstPlanetLevel.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"FirstPlanet")) {
			m_firstStage = NewGO<FirstPlanet>(0, "FirstPlanet");
			m_firstStage->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"player")) {
			m_player = NewGO<Player>(0, "Player");
			m_player->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"wolf")) {
			BasicEnemy* wolf = NewGO<BasicEnemy>(0, "BasicEnemy");
			wolf->SetTRS(objData.position, objData.rotation, objData.scale);
			m_basicEnemies.push_back(wolf);
			return true;
		}
		if (objData.EqualObjectName(L"spider")) {
			DeformEnemy* spider = NewGO <DeformEnemy>(0, "DeformEnemy");
			spider->SetTRS(objData.position, objData.rotation, objData.scale);
			m_deformEnemies.push_back(spider);
			return true;
		}
		if (objData.EqualObjectName(L"rocket")) {
			m_rocket = NewGO <Rocket>(0, "Rocket");
			m_rocket->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		return false;

		});
}