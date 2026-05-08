#pragma once
#include <DirectXMath.h>
#include "System/RenderContext.h"
#include "System/ModelRenderer.h"
#include "Character.h"
#include "System/Sprite.h"

class Typing:public Character
{
public:
	Typing();
	~Typing();

	//XVˆ—
	void Update(float elapsedTime);

	void DrawDebugGUI();

	//•`‰æˆ—
	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	Model* model = nullptr;
};

class Shooting :public Character
{
public:
	Shooting();
	~Shooting();

	//XVˆ—
	void Update(float elapsedTime);

	void DrawDebugGUI();

	//•`‰æˆ—
	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	std::unique_ptr<Sprite> sprite = nullptr;

	float vert = 0.0f;		//c
	float horiz = 0.0f;		//‰¡
};