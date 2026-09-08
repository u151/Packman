#include "Esa.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "TestScene.h"
#include "Ground.h"

Esa::Esa(GameObject* parent)
	:GameObject(parent, "Esa"),type_(ESATYPE_NORMAL),hModel_(-1),score_(0)
{
}

Esa::~Esa()
{
}

void Esa::Initialize()
{
	transform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void Esa::Update()
{
	if (type_ == EsaType::ESATYPE_POWER)
	{
		transform_.rotate_.y += 1.0f;
	}

}

void Esa::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Esa::Release()
{
}

void Esa::SetEsaType(EsaType type)
{
	type_ = type;
	if (type_ == EsaType::ESATYPE_NORMAL)
	{
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0.3, 0), 0.3f);
		AddCollider(collision);
		hModel_ = Model::Load("esa1.fbx");
		score_ = 1;
	}
	else if (type_ == EsaType::ESATYPE_POWER)
	{
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0.6, 0), 0.6f);
		AddCollider(collision);
		hModel_ = Model::Load("P_esa2.fbx");
		score_ = 5;
	}
}

void Esa::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		TestScene* testScene =
			dynamic_cast<TestScene*>(GetParent()->GetParent());

		testScene->AddScore(score_);

		Ground* ground =
			dynamic_cast<Ground*>(FindObject("Ground"));

		ground->DecEsaCount(type_);

		KillMe();
	}
}

void Esa::Esatype()
{
}
