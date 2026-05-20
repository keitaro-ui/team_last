#include "PlayerManager.h"

void PlayerManager::Update(float elapsedTime)
{
	player->Update(elapsedTime);
	player2048->Update(elapsedTime);
}

void PlayerManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	player->Render(rc, renderer);
	player2048->Render(rc, renderer);
}

void PlayerManager::RenderDebugPrimitive(const RenderContext& rc, ShapeRenderer* renderer)
{
}