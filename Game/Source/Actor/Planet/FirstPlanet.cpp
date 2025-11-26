#include "stdafx.h"
#include "FirstPlanet.h"


FirstPlanet::FirstPlanet()
{
	m_filePath = "FirstPlanet/firstPlanet";
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	InitModel(m_filePath, m_position);
}


FirstPlanet::~FirstPlanet()
{
}


bool FirstPlanet::Start()
{
	return true;
}


void FirstPlanet::Update()
{

}


void FirstPlanet::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}