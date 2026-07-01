#include "PlayScene.h"
#include "Ground.h"
#include "Tank.h"
#include "Enemy.h"
#include "Char.h"
#include "Kusa.h"
//#include "TankHead.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene")
{
}

void PlayScene::Initialize()
{
	Instantiate<Ground>(this);  //親をPlaySceneにして地面を生成
	Instantiate<Tank>(this);
	Instantiate<Kusa>(this);
	

	for (int i = 0; i < 10; i++)
	{
		Enemy * e = Instantiate<Enemy>(this);
		e->SetPosition(XMFLOAT3((float)(rand() % 50 - 25), 0, (float)(rand() % 50 - 25)));

	}

	for (int i = 0; i < 5; i++)
	{
		Kusa* k = Instantiate<Kusa>(this);
		k->SetPosition(XMFLOAT3((float)(rand() % 50 - 25), 0, (float)(rand() % 50 - 25)));

	}

	Instantiate<Char>(this);

	//Instantiate<TankHead>(this);
}

void PlayScene::Update()
{
	
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}

