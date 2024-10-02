#include "BeamRifle.h"
#include<DxLib.h>
#include"../../Utility/AsoUtility.h"
#include"../../Object/Actor/Player/Player.h"
#include"../Time/DeltaTime.h"

#pragma region Parameter
//ビームが発射可能になるまでの時間
 constexpr float RELOAD_COUNT_TIME = 100.0f;

//ビームの再発射可能時間を計測するデルタタイムにかける定数
 constexpr float RELOAD_COUNT_RATE = 60.0f;

//装弾数
 constexpr int MAX_BULLETS = 5;

//ビームライフルの大きさ
 constexpr VECTOR SCALE = { 0.07f,0.07f,0.07f };

#pragma endregion


BeamRifle::BeamRifle(int playerType, int playMode, Player& player):player_(player),WeaponBase(playerType, player.GetTransform())
{
	//モデルの読み込み
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BEAMRIFLE));
	//座標
	transform_.pos = MV1GetFramePosition(player_.GetTransform().modelId, ATTACH_RIGHT_HAND_FRAME);
	//大きさ
	transform_.scl = SCALE;
	//回転
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(-90.0f), AsoUtility::Deg2RadF(90.0f), AsoUtility::Deg2RadF(180.0f));
	transform_.Update();
	
	//有効か
	activeFlag_ = true;

	//残弾数を最大に
	numberofBullets_ = MAX_BULLETS;

	for (int beam = 0; beam < numberofBullets_; beam++)
	{
		beams_.emplace_back(std::make_unique<BeamShot>(playerType, player.IsHorming(),beam, playMode));
	}

	//デルタタイムの初期化
	deltaTime_ = 0.0f;

	//クールタイムの初期化
	coolTime_ = RELOAD_COUNT_TIME;
}

void BeamRifle::Update(void)
{
	//デルタタイムの更新
	deltaTime_ = DeltaTime::GetInstsnce().GetDeltaTime();
	
	//座標、回転の同期
	SyncPosition();
	
	//弾の更新
	for (auto& beam : beams_)
	{
		beam->Update(player_.GetEnemyPos());
	}

	//クールタイムの更新
	CoolTimeCount();
}

void BeamRifle::Draw(void)
{
	//有効状態ならライフルを描画
	if (activeFlag_)
	{
		MV1DrawModel(transform_.modelId);
	}
}

void BeamRifle::Trigger(void)
{
	for (auto& beam : beams_)
	{
		//弾が有効状態でなく、残弾が残っていたら発射する
		if (beam->IsActive()||!(numberofBullets_>0))
		{
			continue;
		}		
		//クールタイムを設定
		coolTime_ = 0.0f;
		
		//残弾を1減らす
		numberofBullets_--;

		//座標を設定
		beam->SetPos(transform_.pos);
		
		//弾を有効化
		beam->Activate();
		break;
	}
}

void BeamRifle::InActivateHorming(void)
{
	for (auto& beam : beams_)
	{
		//有効状態でなければ
		if (!beam->IsActive())
		{
			continue;
		}
		//ホーミング無しで有効化
		beam->InActivateHorming();
	}
}

const std::vector<std::unique_ptr<BeamShot>>& BeamRifle::GetBeams() const
{
	//ビームの配列を返す
	return beams_;
}

const int& BeamRifle::GetNumnberOfBullets(void)
{
	//残弾数を返す
	return numberofBullets_;
}

void BeamRifle::NumnberOfBullets(void)
{
	//残弾数を数える
	int bullets = MAX_BULLETS;
	for (auto& beam : beams_)
	{
		//有効状態なら1減らす
		if (beam->IsActive() )
		{
			bullets--;
			continue;
		}
	}
	numberofBullets_ = bullets ;
}

void BeamRifle::CoolTimeCount(void)
{
	//クールタイムが規定を超えてなければ加算する
	if (!(coolTime_ > RELOAD_COUNT_TIME))
	{
		coolTime_ += deltaTime_ * RELOAD_COUNT_RATE;
		//一定を超えたら最大の値に固定する
		if (coolTime_ > RELOAD_COUNT_TIME)
		{
			coolTime_ = RELOAD_COUNT_TIME;
		}
	}
	//クールタイムが規定を超えていれば最大の値に固定する
	else
	{
		coolTime_ = RELOAD_COUNT_TIME;
	}
	Reload();
}

void BeamRifle::Reload(void)
{
	//リロード開始時間まで到達したら一発装填してカウントを0に戻す
	if (coolTime_ == RELOAD_COUNT_TIME)
	{
		numberofBullets_++;
		coolTime_ = 0.0f;
		//弾が5以上になったら5に固定する
		if (numberofBullets_>= MAX_BULLETS)
		{
			numberofBullets_ = MAX_BULLETS;
		}
	}
}

