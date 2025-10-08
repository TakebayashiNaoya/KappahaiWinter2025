#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}


bool BackGround::Start()
{
	return true;
}


void BackGround::Update()
{
}


void BackGround::Render(RenderContext& rc)
{
}

void BackGround::InitModel(const std::string& filePath, const Vector3& position)
{
	std::string fullFilePath = "Assets/modelData/stage/" + filePath + ".tkm";
	m_modelRender.Init(fullFilePath.c_str());
	m_modelRender.SetPosition(position);
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}




/**********************************************/


FirstPlanet::FirstPlanet()
{
	m_filePath = "firstPlanet";
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