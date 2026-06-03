#include "Tank.h"
#include "Engine\\Model.h"
#include "Engine\\Input.h"
//#include <assert.h>

Tank::Tank(GameObject* parent)
	:GameObject(parent, "Tank"), hModel_(-1)
{
	XMMATRIX m;
	m = XMMatrixTranslation(2, 3, 4);

	XMVECTOR mZ = { 0, 0, 1 };

	
}

Tank::~Tank()
{
}

void Tank::Initialize()
{
	hModel_ = Model::Load("TankBody.fbx");
	assert(hModel_ >= 0);  //モデルの読み込みに失敗していないか確認
}

void Tank::Update()
{
	if (Input::IsKey(DIK_LEFT) || Input::IsKey(DIK_A)) {
		transform_.rotate_.y--;
	}

	else if (Input::IsKey(DIK_RIGHT) || Input::IsKey(DIK_D)) {
		transform_.rotate_.y++;
	}

	if (Input::IsKey(DIK_UP) || Input::IsKey(DIK_W)) {
		XMVECTOR vPos = XMLoadFloat3(&transform_.rotate_);
	}

	XMMATRIX m;
	m = XMMatrixTranslation(2, 3, 4);
	XMMATRIX mY = XMMatrixRotationY(XMConvertToRadians(90));
}

void Tank::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Tank::Release()
{
}
