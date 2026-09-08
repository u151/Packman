#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"

class Text;
class Player;
class Ground;

class TestScene : public GameObject
{
public:
	TestScene(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void AddScore(int score);

private:
	Player* pPlayer_;
	Ground* pGround_;
	Text* pText_;
};