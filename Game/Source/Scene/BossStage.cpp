#include "stdafx.h"
#include "BossStage.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/Actor/Planet/BossPlanet.h"
#include "Source/Actor/Object/Spawner/Spawner.h"
#include "Source/UI/UIBossLife.h"
#include "Source/Actor/Actor.h"

BossStage::BossStage()
{
}


BossStage::~BossStage()
{
	DeleteGO(m_bossPlanet);
	DeleteGO(m_uiBossLife);
	DeleteGO(m_bossEnemy);

	for (auto spawner : m_spawners) {
		if (spawner) {
			DeleteGO(spawner);
		}
	}
}


bool BossStage::Start()
{
	m_uiBossLife = NewGO<UIBossLife>(0, "UIBossLife");
	return true;
}

void BossStage::OnUpdate()
{
}


void BossStage::InitLevel()
{
	m_levelRender.Init("Assets/modelData/stage/BossPlanet/BossPlanetLevel.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"planet")) {
			m_bossPlanet = NewGO<BossPlanet>(0, "BossPlanet");
			m_bossPlanet->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"player")) {
			m_player = NewGO<Player>(0, "Player");
			m_player->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"spiderSpawner")) {
			Spawner* spider = NewGO <Spawner>(0, "Spawner");
			spider->SetTRS(objData.position, objData.rotation, objData.scale);
			m_spawners.push_back(spider);
			return true;
		}
		if (objData.EqualObjectName(L"bear")) {
			m_bossEnemy = NewGO <BossEnemy>(0, "BossEnemy");
			m_bossEnemy->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		return false;

		});
}

