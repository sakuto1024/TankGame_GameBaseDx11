#include "Enemy.h"
#include "Engine\\Model.h"
#include "Engine\\Input.h"
#include "Ground.h"
#include "Engine//Debug.h"
#include "Engine\\SphereCollider.h"
#include "Engine\\BoxCollider.h"


Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), hModel_(-1), score_(0)
{
	//transform_.position_.x = rand() % 40 - 20;
	//transform_.position_.z = rand() % 40 - 20;

	randX = rand() % 25;
	randY = rand() % 25;
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Enemy.fbx");

	//　　　　　　　　　モデルハンドル、開始フレーム、終了フレーム、アニメーション速度
	Model::SetAnimFrame(hModel_, 1, 100, ((float)(rand() % 300) / 100.0f));

	assert(hModel_ >= 0);  //モデルの読み込みに失敗していないか確認

	

	//BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 3.0f, 1.0f));
	SphereCollider* collider = new SphereCollider(XMFLOAT3(0.0f, 1.0f, 0.0f), 1.0f);
	AddCollider(collider);
	
	
}

void Enemy::Update()
{

	Debug::Log("RANDPOS = ");
	Debug::Log(transform_.position_.x, true);  //後ろのtrueは改行するかどうか

	static float time = 0.0f;


	//ot_.rotate_.y = time;
	time += 1.0f;
	//transform_.position_.x = randX * sin(0.2f * time);
	//transform_.position_.z = randY * sin(0.2f * time);


	if ((int)time % 30 == (rand() % 30)) {
		transform_.position_.x += (rand() % 200 - 100) / 50;
		transform_.position_.z += (rand() % 200 - 100) / 50;
	}

	/*transform_.position_.x += (rand() % 200 - 100) / 50;
	transform_.position_.z += (rand() % 200 - 100) / 50;*/

	RayCastData data;
	data.start = transform_.position_;
	data.start.y = 0.0f; //地面は0より下に掘られて作られている。そうじゃないときはもっと上から!
	data.dir = { 0, -1, 0 };  //真下にレイを飛ばす

	Ground* pGround = (Ground*)FindObject("Ground");
	int hGroundModel = pGround->GetModelHandle();
	Model::RayCast(hGroundModel, &data);  //レイキャストして、地面に当たったかどうか,当たった時の距離や法線が返ってくる

	if (data.hit) {
		transform_.position_.y = -data.dist;
		//レイの発射位置から、地面までの距離を引いて、地面にぴったりつける
	}
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Bullet")
	{
		pTarget->KillMe();  //バレットを消す
		KillMe();  //自分も消す

		score_++;
	}


}
