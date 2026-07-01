#pragma once
#include "Engine\\GameObject.h"

class Kusa : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Kusa(GameObject* parent);

	//デストラクタ
	~Kusa() {}  //インライン定義

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void OnCollision(GameObject* pTarget) override;

	void SetScore(int score) { score_ = score; }

private:
	int hModel_; //タンクモデルのハンドル
	int hImage_;

	float randX;
	float randY;

	XMFLOAT3 velocity;

	int score_;  //得点
};

