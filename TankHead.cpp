#include "TankHead.h"
#include "Engine\\Model.h"
#include "Engine\\Input.h"
#include "Bullet.h"


TankHead::TankHead(GameObject* parent)
	:GameObject(parent, "TankHead"), hModel_(-1)
{
	//メンバ変数の初期化をする場所
}

void TankHead::Initialize()
{
	//モデルの読み込み、データの用意

	hModel_ = Model::Load("TankHead.fbx");
	assert(hModel_ >= 0);  //モデルの読み込みに失敗していないか確認
}

void TankHead::Update()
{
	if (Input::IsKey(DIK_LEFT)) 
	{
		transform_.rotate_.y -= 2.5f;
	}

	else if (Input::IsKey(DIK_RIGHT)) 
	{
		transform_.rotate_.y += 2.5f;
	}

	if (Input::IsKeyDown(DIK_SPACE)) 
	{
		XMFLOAT3 cannonTop = Model::GetBonePosition(hModel_, "Top");
		XMFLOAT3 cannonRoot = Model::GetBonePosition(hModel_, "Root");

		XMVECTOR topVec = XMLoadFloat3(&cannonTop);
		XMVECTOR rootVec = XMLoadFloat3(&cannonRoot);

		//XMVECTOR vMove = XMVectorSubtract(topVec, rootVec);

		XMVECTOR vMove = topVec - rootVec;
		vMove = vMove * 0.5f;

		XMFLOAT3 bPos;
		XMStoreFloat3(&bPos, vMove);

		Bullet* pBullet = Instantiate<Bullet>(GetParent()->GetParent());  //親を砲塔にして弾を生成
		pBullet->SetMoveVector(bPos);
		pBullet->SetPosition(cannonTop);
	}
}

void TankHead::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TankHead::Release()
{
}
