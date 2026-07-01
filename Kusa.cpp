#include "Kusa.h"
#include "Engine\\Model.h"
#include "Engine\\Input.h"
#include "Ground.h"
#include "Engine//Debug.h"
#include "Engine\\SphereCollider.h"
#include "Engine\\BoxCollider.h"


Kusa::Kusa(GameObject* parent)
	:GameObject(parent, "Kusa"), hModel_(-1), score_(0)
{
	//transform_.position_.x = rand() % 40 - 20;
	//transform_.position_.z = rand() % 40 - 20;

	randX = (float)(rand() % 25);
	randY = (float)(rand() % 25);

	velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Kusa::Initialize()
{
	hModel_ = Model::Load("Kusa.fbx");

	//　　　　　　　　　モデルハンドル、開始フレーム、終了フレーム、アニメーション速度
	Model::SetAnimFrame(hModel_, 1, 180, ((float)(rand() % 300) / 100.0f));

	assert(hModel_ >= 0);  //モデルの読み込みに失敗していないか確認

	transform_.scale_.y = transform_.scale_.y * 4.0f;


	//BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 3.0f, 1.0f));
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0.0f, 1.0f, 0.0f), 1.0f);
	AddCollider(collider);

	transform_.position_.y = 1.0f;
}

void Kusa::Update()
{

	Debug::Log("RANDPOS = ");
	Debug::Log(transform_.position_.x, true);  //後ろのtrueは改行するかどうか

	static float time = 0.0f;


	//ot_.rotate_.y = time;
	time += 1.0f;
	//transform_.position_.x = randX * sin(0.2f * time);
	//transform_.position_.z = randY * sin(0.2f * time);


	if ((int)time % 30 == (rand() % 30)) {
		velocity.x = (float)((rand() % 201) - 100.0f) / 1000.0f;
		velocity.z = (float)((rand() % 201) - 100.0f) / 1000.0f;
	}

	transform_.position_.x += velocity.x;
	transform_.position_.z += velocity.z;

	RayCastData data;
	data.start = transform_.position_;
	data.start.y = 0.0f; //地面は0より下に掘られて作られている。そうじゃないときはもっと上から!
	data.dir = { 0, -1, 0 };  //真下にレイを飛ばす

	Ground* pGround = (Ground*)FindObject("Ground");
	int hGroundModel = pGround->GetModelHandle();
	Model::RayCast(hGroundModel, &data);  //レイキャストして、地面に当たったかどうか,当たった時の距離や法線が返ってくる

	if (data.hit) {
		transform_.position_.y = -data.dist + 2.0f;
		//レイの発射位置から、地面までの距離を引いて、地面にぴったりつける
	}
}

void Kusa::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);


}

void Kusa::Release()
{
}

void Kusa::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Bullet")
	{
		pTarget->KillMe();  //バレットを消す
		KillMe();  //自分も消す

		score_++;
	}


}
