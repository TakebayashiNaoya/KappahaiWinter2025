#pragma once
#include "Level3DRender/LevelRender.h"

class BackGround;
class Player;
class GameCamera;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

private:
	BackGround* m_backGround = nullptr;
	Player* m_player = nullptr;
	GameCamera* m_gameCamera = nullptr;
};