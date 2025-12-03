#include "stdafx.h"
#include "Rocket.h"


Rocket::Rocket()
{
}


Rocket::~Rocket()
{
	// BattleManager‚©‚çƒƒPƒbƒg‚ð“o˜^‰ðœB
	if (auto bm = BattleManager::GetInstance()) {
		bm->UnregisterRocket();
	}
}


bool Rocket::Start()
{
	// BattleManager‚ÉƒƒPƒbƒg‚ð“o˜^B
	if (auto bm = BattleManager::GetInstance()) {
		bm->RegisterRocket(this);
	}

	m_modelRender.Init("Assets/modelData/Object/Rocket/rocket.tkm");
	m_scale = Vector3(200.0f, 200.0f, 200.0f);
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
	return true;
}


void Rocket::Update()
{
}


void Rocket::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
