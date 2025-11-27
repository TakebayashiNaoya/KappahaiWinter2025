#include "stdafx.h"
#include "BossPlanet.h"


BossPlanet::BossPlanet()
{
	m_filePath = "BossPlanet/BossPlanet";
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	InitModel(m_filePath, m_position);
}


BossPlanet::~BossPlanet()
{
}


bool BossPlanet::Start()
{
	return true;
}


void BossPlanet::Update()
{

}


void BossPlanet::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}