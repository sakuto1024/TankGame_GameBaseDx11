#include "Bullet.h"
#include "Engine\\Model.h"

namespace
{
	const float G = 1.02f;
}

Bullet::Bullet(GameObject* parent)
	:GameObject(parent, "Bullet"), hModel_(-1), move_(XMFLOAT3( 0, 0, 0 ))
{
	velocity = XMFLOAT3(1.0f, 0.1f, 1.0f);
}

void Bullet::Initialize()
{
	hModel_ = Model::Load("Bullet.fbx");
	assert(hModel_ >= 0);  //モデルの読み込みに失敗していないか確認

	SphereCollider* collider = new SphereCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);
	AddCollider(collider);
}

void Bullet::Update()
{
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);  //ロード : 読み込み
	XMVECTOR vMove = XMLoadFloat3(&move_);  //ロード : 読み込み
	vPos = vPos + vMove;  //弾の進行方向に移動する
	XMStoreFloat3(&transform_.position_, vPos);  //ストア : 書き込み

	//transform_.position_.x += move_.x;
	//transform_.position_.y += move_.y;
	//transform_.position_.z += move_.z;

	velocity.y = velocity.y * G;

	transform_.position_.y = transform_.position_.y - velocity.y;

	if (transform_.position_.z > 50.0f || transform_.position_.z < -50.0f || transform_.position_.x > 50.0f || transform_.position_.x < -50.0f || transform_.position_.y > 50.0f || transform_.position_.y < -50.0f)
	{
		KillMe();  //弾がある程度遠くに行ったら消す
	}
}

void Bullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bullet::Release()
{
}
