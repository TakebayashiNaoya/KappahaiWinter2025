#pragma once

class Game : public IGameObject
{
public:
	Game();
	~Game();


private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;
};