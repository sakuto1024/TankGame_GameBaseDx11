#include "Tank.h"
#include "Engine\\Model.h"
#include "Engine\\Input.h"
#include "Engine//Debug.h"]
#include "Ground.h"
#include "Engine\\Camera.h"
//#include <assert.h>


namespace
{
	XMVECTOR vFront = { 0, 0, 1, 0 }; //タンクの前方向ベクトル
	const float moveSpeed = 0.1f;

	enum CAM_TYPE
	{
		FIXED_CAM,			//固定カメラ
		TPS_CAM,		//三人称視点カメラ
		TPS_CAMROT,		//三人称視点カメラ (回転)
		FPS_CAM,		//一人称視点カメラ
		CAM_TYPE_MAX

	};
}

//タンクのボディを表すクラス
Tank::Tank(GameObject* parent)
	:GameObject(parent, "Tank"), hModel_(-1), camType_(FIXED_CAM)
{	
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
	if (Input::IsKeyDown(DIK_C)) {
		camType_ = (camType_ + 1) % CAM_TYPE_MAX;  //0,1,2...CAM_TYPE_MAX-1の順でcamType_を切り替える
	}

	switch (camType_)
	{
	case FIXED_CAM:
		Camera::SetTarget(XMFLOAT3(0, 0, 0));
		Camera::SetPosition(XMFLOAT3(0, 20, -30));
		break;

	case TPS_CAM:
		Camera::SetTarget(XMFLOAT3(transform_.position_));
		Camera::SetPosition(XMFLOAT3(0, 10, transform_.position_.z - 30));
		break;

	case TPS_CAMROT:
		break;

	case FPS_CAM:
		break;
	}

	if (Input::IsKey(DIK_LEFT) || Input::IsKey(DIK_A)) {
		transform_.rotate_.y -= 2.0f;
	}

	else if (Input::IsKey(DIK_RIGHT) || Input::IsKey(DIK_D)) {
		transform_.rotate_.y += 2.0f;
	}

	Debug::Log("CAMTYPE = ");
	Debug::Log(camType_, true);  //後ろのtrueは改行するかどうか

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);  //ロード：読み込み
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));  //Y軸回転行列を作る
	//XMVector3TransformCoordは、ベクトルを行列で変換する関数。回転行列をベクトルにかけると、回転したベクトルを得られる
	XMVECTOR vMove = XMVector3TransformCoord(vFront, mRotY);

	vPos = vPos + moveSpeed * vMove;

	if (Input::IsKey(DIK_UP) || Input::IsKey(DIK_W)) {
		XMStoreFloat3(&transform_.position_, vPos);  //ストア：書き込み (格納)
	}

	//レイキャストして、浮いてたら地面まで落とす
	RayCastData data;
	data.start = transform_.position_;
	data.start.y = 0.0f; //地面は0より下に掘られて作られている。そうじゃないときはもっと上から!
	data.dir = { 0, -1, 0 };  //真下にレイを飛ばす

	Ground* pGround = (Ground*)FindObject("Ground");
	int hGroundModel = pGround->GetModelHandle();
	Model::RayCast(hGroundModel, &data);  //レイキャストして、地面に当たったかどうか,当たった時の距離や法線が返ってくる

	if (data.hit) {
		transform_.position_.y = - data.dist;
		//レイの発射位置から、地面までの距離を引いて、地面にぴったりつける
	}
}

void Tank::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Tank::Release()
{
}
