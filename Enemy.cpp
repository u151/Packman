#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "TestScene.h"
#include "Engine/Input.h"
#include "Ground.h"
#include "Engine/SphereCollider.h"

namespace
{
	enum ENEMY_STATE
	{
		ENEMY_IDLE,
		ENEMY_WALK,
		ENEMY_TURN, //回転中
		ENEMY_STATE_MAX //状態の数
	};
	ENEMY_STATE estate = ENEMY_STATE::ENEMY_IDLE; //プレイヤーの状態を管理する変数

	enum ENEMY_DIRECTION
	{
		ENEMY_UP,
		ENEMY_DOWN,
		ENEMY_LEFT,
		ENEMY_RIGHT,
		ENEMY_DIRECTION_MAX //方向の数
	};

	ENEMY_DIRECTION pdirection = ENEMY_DOWN; //プレイヤーの向きを管理する変数
	float P_ANGLE[4] = { 180.0f, 0.0f, 90.0f, 270.0f }; //プレイヤーの向きに応じた角度を格納する配列
	XMVECTOR P_MOVE[4] = { XMVectorSet(0, 0, 1, 0),
						   XMVectorSet(0, 0, -1, 0),
						   XMVectorSet(-1, 0, 0, 0),
						   XMVectorSet(1, 0, 0, 0) }; //プレイヤーの向きに応じた移動ベクトルを格納する配列
	float TURN_FRAME = 10.0f; //回転にかかるフレーム数

	float turnStartAngle = 0.0f; //回転開始時の角度を管理する変数
	float turnEndAngle = 0.0f; //回転終了時の角度を管理する変数
	ENEMY_DIRECTION turnEndDirection = ENEMY_DOWN; //回転終了時の向きを管理する変数
	float AdjustAngle(float angle) {
		if (angle >= 180)
		{
			angle -= 360.0f;
		}
		else if (angle < -180.0f)
		{
			angle += 360.0f;
		}
		return angle;
	}
	std::vector<std::vector<int>> gmap;
}


Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), hWalkModel_(-1), hIdleModel_(-1) {
	//swordDirには、初期方向として、ローカルモデルの剣の根っこから
	//先端までのベクトルとして（0,1,0)を代入しておく
	//初期位置は原点
}

void Enemy::Initialize()
{
	hWalkModel_ = Model::Load("Walking.fbx");
	Model::SetAnimFrame(hWalkModel_, 0, 80, 1.0);
	transform_.position_ = { 0.5f, 0.0, 0.5f };
	hIdleModel_ = Model::Load("Idle.fbx");
	Model::SetAnimFrame(hIdleModel_, 0, 117, 1.0);
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1, 0), 1.0f);
	AddCollider(collision);
}

void Enemy::Update()
{
	//transform_.rotate_.y +=1;
	//static float angle = 0.0;
	//angle = angle + 0.3f;
	//XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//XMMATRIX rotateX = XMMatrixRotationX(XMConvertToRadians(angle));
	//XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(angle));
	//XMMATRIX translate = XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	//SetWorldMatrix(scale *  rotate * translate);

	XMVECTOR pos = XMLoadFloat3(&transform_.position_);
	XMVECTOR move = XMVectorSet(0, 0, 0, 0);
	const float SPEED = 0.32f;
	float angle = 0.0f;
	static float turnFrame = 0.0f; //回転中のフレーム数を管理する変数

	if (estate != ENEMY_STATE::ENEMY_TURN) {
		estate = ENEMY_STATE::ENEMY_IDLE;
	}//回転中でなければ、状態を待機にする

	ENEMY_DIRECTION oldDir = pdirection; //pdirection　<=　今の向き 

	if (estate != ENEMY_STATE::ENEMY_TURN)
	{
		if (Input::IsKey(DIK_LEFT))
		{
			pdirection = ENEMY_DIRECTION::ENEMY_LEFT;
			estate = ENEMY_STATE::ENEMY_WALK;
		}
		if (Input::IsKey(DIK_RIGHT))
		{
			pdirection = ENEMY_DIRECTION::ENEMY_RIGHT;
			estate = ENEMY_STATE::ENEMY_WALK;
		}
		if (Input::IsKey(DIK_UP))
		{
			pdirection = ENEMY_DIRECTION::ENEMY_UP;
			estate = ENEMY_STATE::ENEMY_WALK;
		}
		if (Input::IsKey(DIK_DOWN))
		{
			pdirection = ENEMY_DIRECTION::ENEMY_DOWN;
			estate = ENEMY_STATE::ENEMY_WALK;
		}
	}
	if (oldDir != pdirection) {
		//回転しなきゃだよ。
		estate = ENEMY_STATE::ENEMY_TURN;
		turnFrame = 0.0f;//回転中のフレーム数をリセット
		turnStartAngle = P_ANGLE[oldDir];//げんざいのほうこうから
		float diff = AdjustAngle(P_ANGLE[pdirection] - P_ANGLE[oldDir]);
		//diffが正の値なら、右回転、負の値なら左回転
		turnEndDirection = pdirection;//入力方向に３０フレームで回転する
		//回転終了時の角度を計算する
		turnEndAngle = turnStartAngle + diff;
	}
	//  ↑ 状態切り替えの処理
	//　↓ 状態ごとの処理

	if (estate == ENEMY_STATE::ENEMY_TURN)
	{
		////回転中の処理
		//oldDir　→　今の角度 
		//pdirection -> 目標角度
		////３０フレームで回転するようにする
		turnFrame += 1.0f;
		float t = turnFrame / TURN_FRAME; //0.0～1.0
		if (t > 1.0f)
		{
			t = 1.0f;//1.0を超えないようにする(保険）
		}
		angle = turnStartAngle + (turnEndAngle - turnStartAngle) * t;
		transform_.rotate_.y = angle;
		// 30フレーム経過したら、回転終了
		if (turnFrame >= TURN_FRAME)
		{
			pdirection = turnEndDirection;
			transform_.rotate_.y = P_ANGLE[pdirection];
			estate = ENEMY_STATE::ENEMY_WALK;
		}
		return;//早期リターンで、回転中は移動しないようにする
	}
	else if (estate != ENEMY_STATE::ENEMY_IDLE)
	{
		move = P_MOVE[pdirection];
		angle = P_ANGLE[pdirection];
		transform_.rotate_.y = angle;
	}

	pos = pos + SPEED * move;
	XMStoreFloat3(&transform_.position_, pos);
	XMFLOAT3 wpos = transform_.position_;
	//壁オブジェクトに食い込んでたら戻す！
	gmap = ground_->GetMapData();//マップを取得
	//マップの座標に変換する、めり込んでたら戻す。
	int mapX = (int)((wpos.x) + 10) / 2;
	int mapZ = (int)(10 - (wpos.z)) / 2;
	if (gmap[mapZ][mapX] == 1)
	{
		pos = pos - SPEED * move;
		XMStoreFloat3(&transform_.position_, pos);
	}

	//pos = XMVectorAdd(pos, SPEED*move);
}

void Enemy::Draw()
{
	//transform_.scale_ = { 0.01,0.01,0.01 };
	//transform_.position_ = { 0, 0.0, 0 };

	if (estate == ENEMY_STATE::ENEMY_IDLE)
	{
		Model::SetTransform(hIdleModel_, transform_);
		Model::Draw(hIdleModel_);
	}
	else if (estate == ENEMY_STATE::ENEMY_WALK || estate == ENEMY_STATE::ENEMY_TURN)
	{
		Model::SetTransform(hWalkModel_, transform_);
		Model::Draw(hWalkModel_);
	}

}


void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* eTarget)
{

}
