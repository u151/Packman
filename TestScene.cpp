#include "TestScene.h"
#include "Player.h"
#include "Ground.h"
#include "Engine/Camera.h"
#include "Engine/Text.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

namespace {
	int myScore = 0;
}

//コンストラクタ
TestScene::TestScene(GameObject* parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
//餌を数えて残りの数を表示
//スコアを表示
//sprintf
void TestScene::Initialize()
{
	//pWp = Instantiate<Weapon>(this);
	pPlayer_ = Instantiate<Player>(this);
	pGround_ = Instantiate<Ground>(this);
	
	pPlayer_->SetGround(pGround_);
	Camera::SetPosition({ 0,20,-20 });
	Camera::SetTarget({ 0,0,0 });

	pText_ = new Text;
	pText_->Initialize();
}

//更新
void TestScene::Update()
{
	if (Input::IsKeyDown(DIK_R))
	{
		SceneManager* sceneManager =
			dynamic_cast<SceneManager*>(GetParent());

		sceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

//描画
void TestScene::Draw()
{
	std::string scrText;
	scrText = "SCORE:" + std::to_string(myScore);
	pText_->Draw(20, 20, scrText.c_str());
}

//開放
void TestScene::Release()
{
}

void TestScene::AddScore(int score)
{
	myScore += score;
}
