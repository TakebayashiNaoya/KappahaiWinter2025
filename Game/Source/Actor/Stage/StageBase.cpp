#include "stdafx.h"
#include "StageBase.h"

StageBase::StageBase()
{
}


StageBase::~StageBase()
{
}


bool StageBase::Start()
{
	return true;
}


void StageBase::Update()
{
}


void StageBase::Render(RenderContext& rc)
{
}

void StageBase::InitModel(const std::string& filePath, const Vector3& position)
{
	std::string fullFilePath = "Assets/modelData/stage/" + filePath + ".tkm";
	m_modelRender.Init(fullFilePath.c_str());
	m_modelRender.SetPosition(position);
	physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}