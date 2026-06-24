#pragma once
#include "Engine\\GameObject.h"
#include "Engine\\Image.h"

class Char : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Char(GameObject* parent);
	~Char();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;



private:
	int hImage_; //タンクモデルのハンドル
	


};

