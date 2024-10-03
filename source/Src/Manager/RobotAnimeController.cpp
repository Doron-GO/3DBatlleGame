#include<DxLib.h>
#include "RobotAnimeController.h"

static constexpr float BLEND_TIME =0.1f;


RobotAnimeController::RobotAnimeController(int modelId)
{
	//モデル本体
	modelId_ = modelId;
	deltaTime_ = 0.0f;
}

RobotAnimeController::~RobotAnimeController()
{
}

void RobotAnimeController::Update(void)
{
	deltaTime_ =  DeltaTime::GetInstsnce().GetDeltaTime();
	UpperBodyUpdate();
	LowerBodyUpdate();
	DetachAnim();
}

void RobotAnimeController::Draw(void)
{
	DrawFormatStringF(0.0f, 270.0f, 0xffffff, "上半身ブレンドされるRATE : %f", blend_[Body::UP].rate_);
	DrawFormatStringF(0.0f, 290.0f, 0xffffff, "下半身ブレンドされるRATE: %f", blend_[Body::LOW].rate_);
	DrawFormatStringF(0.0f, 310.0f, 0xffffff, "上半身ブレンドされるRATE : %f", 1.0f - blend_[Body::UP].rate_);
	DrawFormatStringF(0.0f, 330.0f, 0xffffff, "下半身ブレンドされるRATE: %f", 1.0f - blend_[Body::LOW].rate_);
	DrawFormatStringF(0.0f, 350.0f, 0xffffff, "上半身アニメーションアタッチ番号 : %d", playAnim_[Body::UP].attachNo_);
	DrawFormatStringF(0.0f, 370.0f, 0xffffff, "下半身アニメーションアタッチ番号: %d", playAnim_[Body::LOW].attachNo_);
	DrawFormatStringF(0.0f, 390.0f, 0xffffff, "上半身ブレンドされるアタッチ番号: %d", blend_[Body::UP].attachNo_);
	DrawFormatStringF(0.0f, 410.0f, 0xffffff, "下半身ブレンドされるアタッチ番号 : %d", blend_[Body::LOW].attachNo_);
	DrawFormatStringF(0.0f, 430.0f, 0xffffff, "ブレンド前再生時間: %f", MV1GetAttachAnimTime(modelId_, 0));
	DrawFormatStringF(0.0f, 450.0f, 0xffffff, "ブレンド後再生時間: %f", MV1GetAttachAnimTime(modelId_, 1));	
	DrawFormatStringF(0.0f, 470.0f, 0xffffff, "上半身アニメーション再生時間 : %f", playAnim_[Body::UP].step_);
	DrawFormatStringF(0.0f, 490.0f, 0xffffff, "下半身アニメーション再生時間: %f", playAnim_[Body::LOW].step_);
	DrawFormatStringF(0.0f, 530.0f, 0xffffff, "上半身優先フラグ: %d", playAnim_[Body::UP].priority_ );
}

void RobotAnimeController::Add(int type, const std::string& path, float speed, float totalTime)
{
	Animation anim;
	//ロード
	anim.modelId_ = MV1LoadModel(path.c_str());
	//アニメーションタイプをインデックス（新たな要素を末尾に）に追加
	anim.animIndex_ = type;
	//スピード
	anim.speed_ = speed;
	//総再生時間
	anim.totalTime_ = totalTime;
	isAttach_[static_cast<STATE>(type)] = false;
	//アニメーションタイプが入ってない場合？ここ確認
	if (animations_.count(type) == 0)
	{
		// 入れ替え
		animations_.emplace(type, anim);
	}
	else
	{
		// 追加
		animations_[type].modelId_ = anim.modelId_;
		animations_[type].animIndex_ = anim.animIndex_;
		animations_[type].attachNo_ = anim.attachNo_;
		animations_[type].totalTime_ = anim.totalTime_;
	}
}

void RobotAnimeController::UpperBodyUpdate(void)
{
	//ブレンド中フラグが立っていれば
	if (blend_[Body::UP].blendFlag_)
	{
		//rateが1.0fでブレンド終了
		if (blend_[Body::UP].rate_ < 1.0f)
		{
			//ROOTと背骨0の、次アニメレートを増やす
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 60, blend_[Body::UP].rate_, false);
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 61, blend_[Body::UP].rate_, false);
			//背骨0の前アニメレートを徐々に減らす
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::UP].attachNo_, 61, 1.0f - blend_[Body::UP].rate_, false);
			//背骨1以下はすべて連動、前を減らし、次を増やす
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 62, blend_[Body::UP].rate_, true);
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::UP].attachNo_, 62, 1.0f - blend_[Body::UP].rate_, true);
			//ROOTは不動のため、現アニメの影響を受けない
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 60, 0.0f, false);

			blend_[Body::UP].rate_ = blend_[Body::UP].stepBlend_ / BLEND_TIME;
			blend_[Body::UP].stepBlend_ += deltaTime_;
		}
		//ブレンドが終了したら
		else
		{
			//に関するパラメータを初期化
			blend_[Body::UP].blendFlag_ = false;
			blend_[Body::UP].stepBlend_ = 0.0f;
			blend_[Body::UP].rate_ = 1.0f;
		}
	}
	//ブレンド中じゃなければ通常再生
	else
	{
		//ストップフラグがfalse
		if (!playAnim_[Body::UP].isStop_)
		{
			// 再生時間を進める
			playAnim_[Body::UP].step_ += (deltaTime_ * playAnim_[Body::UP].speed_);
			// アニメーション終了判定
			bool isEnd = false;
			//アニメーション進行時間がトータル時間を超えたら
			if (playAnim_[Body::UP].step_ > playAnim_[Body::UP].totalTime_)
			{
				//アニメーション終了フラグを立てる
				isEnd = true;
				playAnim_[Body::UP].isEnd = true;
				//アニメーション優先再生フラグをfalseにする
				playAnim_[Body::UP].priority_ = false;
			}
			// アニメーションが終了したら
			if (isEnd)
			{
				//ループ再生フラグが立っていれば
				if (playAnim_[Body::UP].isLoop_)
				{
					playAnim_[Body::UP].step_ = 0.0f;
				}
				else
				{
					// ステップをトータルタイムに固定
					playAnim_[Body::UP].step_ = playAnim_[Body::UP].totalTime_;
				}
			}
		}
		else
		{
			playAnim_[Body::UP].priority_ = false;
		}
		// アニメーション設定
		MV1SetAttachAnimTime(modelId_, playAnim_[Body::UP].attachNo_, playAnim_[Body::UP].step_);
	}
}

void RobotAnimeController::LowerBodyUpdate(void)
{
	if (blend_[Body::LOW].blendFlag_)
	{
		if (blend_[Body::LOW].rate_ < 1.0f)
		{
			//ROOTの現アニメのrateを減らし、新しいアニメのrateを増やす
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 60, blend_[Body::LOW].rate_, false);
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::LOW].attachNo_, 60, 1.0f - blend_[Body::LOW].rate_, false);
			//下半身 右足以下と左足以下連動
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 115, blend_[Body::LOW].rate_, true);
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 120, blend_[Body::LOW].rate_, true);
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::LOW].attachNo_, 115, 1.0f - blend_[Body::LOW].rate_, true);
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::LOW].attachNo_, 120, 1.0f - blend_[Body::LOW].rate_, true);

			blend_[Body::LOW].rate_ = blend_[Body::LOW].stepBlend_ / BLEND_TIME;
			blend_[Body::LOW].stepBlend_ += deltaTime_;
		}
		else
		{
			blend_[Body::LOW].blendFlag_ = false;
			blend_[Body::LOW].stepBlend_ = 0.0f;
			blend_[Body::LOW].rate_ = 1.0f;
		}
	}
	else
	{
		if (playType_[Body::UP] == playType_[Body::LOW])
		{
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 60, 1.0f, false);
		}
		//ストップフラグがfalse
		if (!playAnim_[Body::LOW].isStop_)
		{
			// 再生
			playAnim_[Body::LOW].step_ += (deltaTime_ * playAnim_[Body::LOW].speed_);

			// アニメーション終了判定
			bool isEnd = false;
			//アニメーション進行時間がトータル時間を超えたら
			if (playAnim_[Body::LOW].step_ > playAnim_[Body::LOW].totalTime_)
			{
				isEnd = true;
				playAnim_[Body::LOW].isEnd = true;
				playAnim_[Body::LOW].priority_ = false;
			}
			// アニメーションの1ループが再生終了しているときに
			if (isEnd)
			{
				// ループ再生がtrueなら
				if (playAnim_[Body::LOW].isLoop_)
				{
					// 再生ステップをリセット
					playAnim_[Body::LOW].step_ = 0.0f;
				}
				else
				{
					// ループしない
					playAnim_[Body::LOW].step_ = playAnim_[Body::LOW].totalTime_;
				}
			}
		}
		else
		{
			playAnim_[Body::LOW].priority_ = false;
		}
		// アニメーション設定
		MV1SetAttachAnimTime(modelId_, playAnim_[Body::LOW].attachNo_, playAnim_[Body::LOW].step_);
	}
}

void RobotAnimeController::DetachAnim(void)
{
	//追加されているアニメーションの数分回す
	for (const auto& anim:isAttach_)
	{
		//アタッチされていなければ戻る
		if (!anim.second)
		{
			continue;
		}
		//1 アタッチされているアニメが現在再生中の上半身アニメでない
		//2 アタッチされているアニメが現在再生中の下半身アニメでない
		//3 アタッチされているアニメがブレンド中の下半身アニメでない
		//1,2,3全てが当てはまっていたら、そのアニメをデタッチして、false状態にする
		if (anim.first!= playType_[Body::UP] &&
			anim.first!= playType_[Body::LOW]&&
			anim.first!= blend_[Body::UP].type_&&
			anim.first != blend_[Body::LOW].type_
			)
		{
				//アニメーションを外す
				MV1DetachAnim(modelId_, attachedTypeNum_[anim.first]);
				isAttach_[anim.first] = false;
		}
	}
	
}

void RobotAnimeController::UpperBodyPlay(int type, bool priority, bool isLoop ,
	bool isStop, float endStep , float startStep , bool isForce )
{
	//現在再生中のアニメーションとタイプが違い、優先再生でもなく、かつアニメーションタイプが-1でなければアニメーションを入れ替える
	if ( !(playAnim_[Body::UP].priority_) && playType_[Body::UP] != static_cast<STATE>(type) &&  type != -1)
	{
		auto newType = static_cast<STATE>(type);
		if (!blend_[Body::UP].blendFlag_)
		{
			//現在のアニメーションナンバーをブレンドアニメーションナンバーに格納
			blend_[Body::UP].attachNo_ = playAnim_[Body::UP].attachNo_;

			//現在のプレイタイプをブレンドアニメーションタイプに格納
			blend_[Body::UP].type_ = playType_[Body::UP];

			//新しいアニメーション情報を再生アニメーション情報に格納
			playAnim_[Body::UP] = animations_[type];

			//現在再生中タイプを新しいアニメーションに更新
			playType_[Body::UP] = newType;

			//新しいアニメーションがまだアタッチされていなければ
			if (!isAttach_[newType])
			{
				//アニメーションをアタッチして現在再生中アニメーションナンバーに格納
				playAnim_[Body::UP].attachNo_ = MV1AttachAnim(modelId_, 0, playAnim_[Body::UP].modelId_);
				//アニメーション影響率を変える
				MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 60, 0.0f, true);
				//アタッチ中フラグをtrueにする
				isAttach_[newType] = true;
				//アタッチ中アニメーションのアタッチナンバーを保存
				attachedTypeNum_[newType] = playAnim_[Body::UP].attachNo_;
				//再生開始時間を設定
				playAnim_[Body::UP].step_ = startStep;
			}
			else
			{
				//アタッチされていない場合、アタッチする
				playAnim_[Body::UP].attachNo_ = attachedTypeNum_[newType];
				if (!startStep == 0.0f)
				{
					playAnim_[Body::UP].step_ = startStep;
				}
				else
				{
					playAnim_[Body::UP].step_ = MV1GetAttachAnimTime(modelId_, playAnim_[Body::UP].attachNo_);
				}
			}
			playAnim_[Body::UP].priority_ = priority;
			playAnim_[Body::UP].isStop_ =isStop;
			playAnim_[Body::UP].isLoop_ =isLoop;
			blend_[Body::UP].blendFlag_ = true;
			blend_[Body::UP].rate_ = 0.0f;
		}
		//アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			playAnim_[Body::UP].totalTime_ = endStep;
		}
		else
		{
			playAnim_[Body::UP].totalTime_ = MV1GetAttachAnimTotalTime(modelId_, playAnim_[Body::UP].attachNo_);
		}
		//アニメーションループ
		playAnim_[Body::UP].isLoop_ = isLoop;
		// アニメーションしない
		playAnim_[Body::UP].isStop_ = isStop;
	}
}

void RobotAnimeController::LowerBodyPlay(int type, bool priority , bool isLoop ,
	bool isStop , float endStep, float startStep, bool isForce )
{
	//アニメーションタイプが違い、かつアニメーションタイプが入っていたら
	if (!(playAnim_[Body::LOW].priority_) && playType_[Body::LOW] != static_cast<STATE>(type) && type != -1)
	{
		auto newType = static_cast<STATE>(type);
		if (!blend_[Body::LOW].blendFlag_)
		{
			blend_[Body::LOW].attachNo_ = playAnim_[Body::LOW].attachNo_;
			blend_[Body::LOW].type_ = playType_[Body::LOW];
			playAnim_[Body::LOW] = animations_[type];
			playType_[Body::LOW] = newType;

			//切り替わり先のアニメーションがアタッチ済みかどうかを判定
			if (!isAttach_[newType])
			{
				//アタッチされていない場合、アタッチする
				playAnim_[Body::LOW].attachNo_ = MV1AttachAnim(modelId_, 0, playAnim_[Body::LOW].modelId_);
				MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 60, 0.0f, true);
				isAttach_[newType] = true;
				attachedTypeNum_[newType] = playAnim_[Body::LOW].attachNo_;

				playAnim_[Body::LOW].step_ = startStep;
			}
			else
			{
				//アタッチ済みの場合すでにあるアタッチ番号をそのまま使用
				playAnim_[Body::LOW].attachNo_ = attachedTypeNum_[newType] ;
				if (!startStep == 0.0f)
				{
					playAnim_[Body::LOW].step_ = startStep;
				}
				else
				{
					playAnim_[Body::LOW].step_ = MV1GetAttachAnimTime(modelId_, playAnim_[Body::LOW].attachNo_);
				}
			}
			//初期化
			//playAnim_[DW].step_ = startStep;
			playAnim_[Body::LOW].isStop_ = isStop;
			playAnim_[Body::LOW].isLoop_ = isLoop;
			playAnim_[Body::LOW].priority_ = priority;
			blend_[Body::LOW].blendFlag_ = true;
			blend_[Body::LOW].rate_ = 0.0f;
		}
		//アニメーション総時間の取得
		if (endStep > 0.0f)
		{
			playAnim_[Body::LOW].totalTime_ = endStep;
		}
		else
		{
			playAnim_[Body::LOW].totalTime_ = MV1GetAttachAnimTotalTime(modelId_, playAnim_[Body::LOW].attachNo_);
		}
		//アニメーションループ
		playAnim_[Body::LOW].isLoop_ = isLoop;
		// アニメーションしない
		playAnim_[Body::LOW].isStop_ = isStop;
	}
}

void RobotAnimeController::DebugDraw()
{
	for (auto num =0; num <2; num++)
	{
		STATE state = playType_[static_cast<Body>(num)];
		std::string  st;
		switch (state)
		{
		case RobotAnimeController::STATE::NONE:
			break;
		case RobotAnimeController::STATE::RUN:
			st = "RUN";
			break;
		case RobotAnimeController::STATE::JUMP:
			st = "JUMP";
			break;
		case RobotAnimeController::STATE::DAMAGE:
			st = "DAMAGE";
			break;
		case RobotAnimeController::STATE::SHOT:
			st = "SHOT";
			break;
		case RobotAnimeController::STATE::IDLE:
			st = "IDLE";
			break;
		case RobotAnimeController::STATE::BOOST:
			st = "DASH";
			break;
		case RobotAnimeController::STATE::COMBAT1:
			st = "COMBAT1";

			break;
		case RobotAnimeController::STATE::COMBAT_RUN:
			st = "COMBAT_RUN";
			break;
		}
		DrawFormatString(0, 430+(20* num), 0xffffff, "アニメーション:%s", st.c_str());
	}
}

bool RobotAnimeController::IsUpperEnd(void)
{
	if (playAnim_[Body::UP].isEnd )
	{
		return true;
	}
	return false;
}

bool RobotAnimeController::IsLowerEnd(void)
{
	if (playAnim_[Body::LOW].isEnd)
	{
		return true;
	}
	return false;
}

bool RobotAnimeController::IsEnd(void)
{
	if (playAnim_[Body::UP].isEnd&& playAnim_[Body::LOW].isEnd)
	{
		return true;
	}
	return false;
}

