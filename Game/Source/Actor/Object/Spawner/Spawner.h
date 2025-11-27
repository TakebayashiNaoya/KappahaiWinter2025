#pragma once
#include "Source/Actor/Actor.h"

class DeformEnemy;

class Spawner : public Actor
{
public:
	Spawner();
	~Spawner();

private:
	virtual bool Start() override final;
	virtual void Update() override final;


private:
	DeformEnemy* m_deformEnemy = nullptr;
	float m_respawnTimer = 0.0f;
};

