#include "EnemySlime.h"
#include <imgui.h>
#include <random>
//#include <algorithm>




//コンストラクタ
EnemySlime::EnemySlime()
{
	model[0] = new Model("Data/Model/Target/enemy_suit_INMotion.mdl");
	//models[1] = new Model("Data/Model/Target/target_2.mdl");

	//モデルが大きいのでスケーリング
	scale = { 0.02f, 0.02f, 0.02f };

	//幅、高さ設定
	/*radius = 0.2f;
	height = 0.0f;*/

	if (titleSwitch == true)
		model[0]->PlayAnimation(4, true, 0.2f);

	CreateModel();
}

//デストラクタ
EnemySlime::~EnemySlime()
{
	delete model[0];
	//delete models[1];
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
	//速力更新処理
	//UpdateVelocity(elapsedTime);

	model[0]->UpdateAnimation(elapsedTime);

	

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model[0]->UpdateTransform();

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);
}

//描画処理
void EnemySlime::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model[0], ShaderId::Lambert);
}


void EnemySlime::CreateModel()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int>dist(0, 3);

	model_index = dist(gen);
}

void EnemySlime::OnDead()
{
	//自信を破棄
	Destroy();
}

//デバッグ用GUI描画
void EnemySlime::DrawDebugGUI()
{
	//ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
	//ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	//if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	//{
	//	//マウスの位置
	//	ImGui::Text("x = %f", ndc.x);
	//	ImGui::Text("y = %f", ndc.y);

	//	//的
	//	//extern int count_1, count_2, count_3, count_4;
	//	ImGui::Text("count_1 = %d", count_1);
	//	ImGui::Text("count_2 = %d", count_2);
	//	ImGui::Text("count_3 = %d", count_3);
	//	ImGui::Text("count_4 = %d", count_4);

	//	//トランスフォーム
	//	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	//	{
	//		//位置
	//		ImGui::InputFloat3("Position", &position.x);
	//		//回転
	//		DirectX::XMFLOAT3 a;
	//		a.x = DirectX::XMConvertToDegrees(angle.x);
	//		a.y = DirectX::XMConvertToDegrees(angle.y);
	//		a.z = DirectX::XMConvertToDegrees(angle.z);
	//		ImGui::InputFloat3("Angle", &a.x);
	//		angle.x = DirectX::XMConvertToRadians(a.x);
	//		angle.y = DirectX::XMConvertToRadians(a.y);
	//		angle.z = DirectX::XMConvertToRadians(a.z);
	//		//スケール
	//		ImGui::InputFloat3("Scale", &scale.x);

	//		//カメラ
	//		//ImGui::InputFloat3("eye",&)
	//	}
	//}
	//ImGui::End();

	ImGui::Begin("Boss");

	// Position
	ImGui::DragFloat3("Position", &position.x, 0.1f);

	// Scale
	ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.01f, 10.0f);

	ImGui::Separator();

	if (ImGui::Button("Reset Position"))
	{
		position = { 0.0f, 0.0f, 0.0f };
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset Scale"))
	{
		scale = { 1.0f, 1.0f, 1.0f };
	}

	// 現在位置表示
	ImGui::Text("Now Position");
	ImGui::Text("X : %.2f", position.x);
	ImGui::Text("Y : %.2f", position.y);
	ImGui::Text("Z : %.2f", position.z);

	ImGui::End();
}
