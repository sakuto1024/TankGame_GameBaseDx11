#include "Char.h"

Char::Char(GameObject* parent)
	:GameObject(parent, "Char"), hImage_(-1)
{
}

Char::~Char()
{
}

void Char::Initialize()
{
	hImage_ = Image::Load("char.png");
}

void Char::Update()
{
	transform_.position_ = { 1.0f, 1.0f, 1.0f };
}

void Char::Draw()
{
	Image::SetRect(hImage_, 0, 32, 256, 32);
	//Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void Char::Release()
{
}
