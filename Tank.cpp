#include "Tank.h"
#include "Engine\\Model.h"
#include "Engine\\Input.h"
#include "Engine//Debug.h"
#include "Ground.h"
#include "Engine\\Camera.h"
#include "TankHead.h"
//#include <assert.h>


namespace
{
	XMVECTOR vFront = { 0, 0, 1, 0 }; //タンクの前方向ベクトル
	const float moveSpeed = 0.1f;
	const float CAM_HEIGHT_BIAS = 0.2f;  //カメラの高さのバイアス
	const float TPSCAM_BIAS_Y = 3.0f;
	const float TPSCAM_BIAS_Z = 10.0f;

	enum CAM_TYPE
	{
		FIXED_CAM,			//固定カメラ
		TPS_CAM,		//三人称視点カメラ
		TPS_CAMROT,		//三人称視点カメラ (回転)
		FPS_CAM,		//一人称視点カメラ
		CAM_TYPE_MAX

	};

	//TankHead* t;
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

	//t = Instantiate<TankHead>(this->GetParent());
	
	Instantiate<TankHead>(this);  //親をタンクにして砲塔を生成 thisはTank自身
}

void Tank::Update()
{
	
	//t->SetPosition(XMFLOAT3(transform_.position_.x, transform_.position_.y, transform_.position_.z));
	//t->SetRotate(transform_.rotate_);

	if (Input::IsKeyDown(DIK_C)) {
		camType_ = (camType_ + 1) % CAM_TYPE_MAX;  //0,1,2...CAM_TYPE_MAX-1の順でcamType_を切り替える
	}

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);  //ロード：読み込み
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));  //Y軸回転行列を作る
	//XMVector3TransformCoordは、ベクトルを行列で変換する関数。回転行列をベクトルにかけると、回転したベクトルを得られる
	XMVECTOR vMove = XMVector3TransformCoord(vFront, mRotY);

	switch (camType_)
	{
	case FIXED_CAM:
		SetFixedCam();
		break;

	case TPS_CAM:
	{
		XMFLOAT3 camPos = transform_.position_;
		camPos.y = camPos.y + TPSCAM_BIAS_Y;
		camPos.z = camPos.z - TPSCAM_BIAS_Z;
		Camera::SetPosition(camPos);
		Camera::SetTarget(XMFLOAT3(transform_.position_));
	}
		break;

	case TPS_CAMROT:
	{
		XMFLOAT3 camPos;  //タンクの位置をカメラの位置にする
		XMVECTOR vCAM = { 0.0f, TPSCAM_BIAS_Y, -TPSCAM_BIAS_Z, 0.0f }; //カメラの位置をタンクの位置より少し後ろにする
		vCAM = XMVector3TransformCoord(vCAM, mRotY);  //タンクの回転をカメラの位置に反映させる
		XMStoreFloat3(&camPos, vPos + vCAM);  //カメラの位置をタンクの位置に反映させる


		//XMFLOAT3 camPos = transform_.position_;
		//camPos.y = camPos.y - TPSCAM_BIAS_Y;
		//camPos.z = camPos.z + TPSCAM_BIAS_Z;

		//XMVECTOR cVec = XMLoadFloat3(&transform_.position_) - XMLoadFloat3(&camPos);
		//XMVECTOR cMove = XMVector3TransformCoord(cVec, mRotY);

		//XMStoreFloat3(&camPos, vPos + cMove);  //カメラの注意点をタンクの前方にする

		Camera::SetPosition(camPos);
		Camera::SetTarget(XMFLOAT3(transform_.position_));

	}

		break;

	case FPS_CAM:
		XMFLOAT3 camPos = transform_.position_;
		camPos.y = camPos.y + CAM_HEIGHT_BIAS;
		Camera::SetPosition(camPos);  //カメラの位置をタンクの位置にする
		XMFLOAT3 camTarget;
		XMStoreFloat3(&camTarget, vPos + vMove);  //カメラの注意点をタンクの前方にする
		camTarget.y = camTarget.y + CAM_HEIGHT_BIAS;
		Camera::SetTarget(camTarget);
		break;
	}

	if ( Input::IsKey(DIK_A)) {
		transform_.rotate_.y -= 2.0f;
	}

	else if (Input::IsKey(DIK_D)) {
		transform_.rotate_.y += 2.0f;
	}

	//Debug::Log("CAMTYPE = ");
//	Debug::Log(camType_, true);  //後ろのtrueは改行するかどうか


	if (Input::IsKey(DIK_W)) {
		vPos = vPos + moveSpeed * vMove;

		XMStoreFloat3(&transform_.position_, vPos);  //ストア：書き込み (格納)
	}

	else if (Input::IsKey(DIK_S)) {
		vPos = vPos - ((moveSpeed * vMove) * 0.7);

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

void Tank::SetFixedCam()
{
	Camera::SetTarget(XMFLOAT3(0, 0, 0));
	Camera::SetPosition(XMFLOAT3(0, 25, -50));
}
