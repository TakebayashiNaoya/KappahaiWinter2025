#pragma once
#include "Level3DRender/LevelRender.h"


class SceneManagerObject;
class BattleManagerObject;
class CollisionManagerObject;


class Game : public IGameObject
{
public:
	Game();
	~Game();


private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;


private:
	SceneManagerObject* m_sceneManager = nullptr;
	BattleManagerObject* m_battleManager = nullptr;
	CollisionManagerObject* m_collisionManager = nullptr;
};