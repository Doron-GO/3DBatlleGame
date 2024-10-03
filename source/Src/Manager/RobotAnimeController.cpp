#include<DxLib.h>
#include "RobotAnimeController.h"

static constexpr float BLEND_TIME =0.1f;


RobotAnimeController::RobotAnimeController(int modelId)
{
	//���f���{��
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
	DrawFormatStringF(0.0f, 270.0f, 0xffffff, "�㔼�g�u�����h�����RATE : %f", blend_[Body::UP].rate_);
	DrawFormatStringF(0.0f, 290.0f, 0xffffff, "�����g�u�����h�����RATE: %f", blend_[Body::LOW].rate_);
	DrawFormatStringF(0.0f, 310.0f, 0xffffff, "�㔼�g�u�����h�����RATE : %f", 1.0f - blend_[Body::UP].rate_);
	DrawFormatStringF(0.0f, 330.0f, 0xffffff, "�����g�u�����h�����RATE: %f", 1.0f - blend_[Body::LOW].rate_);
	DrawFormatStringF(0.0f, 350.0f, 0xffffff, "�㔼�g�A�j���[�V�����A�^�b�`�ԍ� : %d", playAnim_[Body::UP].attachNo_);
	DrawFormatStringF(0.0f, 370.0f, 0xffffff, "�����g�A�j���[�V�����A�^�b�`�ԍ�: %d", playAnim_[Body::LOW].attachNo_);
	DrawFormatStringF(0.0f, 390.0f, 0xffffff, "�㔼�g�u�����h�����A�^�b�`�ԍ�: %d", blend_[Body::UP].attachNo_);
	DrawFormatStringF(0.0f, 410.0f, 0xffffff, "�����g�u�����h�����A�^�b�`�ԍ� : %d", blend_[Body::LOW].attachNo_);
	DrawFormatStringF(0.0f, 430.0f, 0xffffff, "�u�����h�O�Đ�����: %f", MV1GetAttachAnimTime(modelId_, 0));
	DrawFormatStringF(0.0f, 450.0f, 0xffffff, "�u�����h��Đ�����: %f", MV1GetAttachAnimTime(modelId_, 1));	
	DrawFormatStringF(0.0f, 470.0f, 0xffffff, "�㔼�g�A�j���[�V�����Đ����� : %f", playAnim_[Body::UP].step_);
	DrawFormatStringF(0.0f, 490.0f, 0xffffff, "�����g�A�j���[�V�����Đ�����: %f", playAnim_[Body::LOW].step_);
	DrawFormatStringF(0.0f, 530.0f, 0xffffff, "�㔼�g�D��t���O: %d", playAnim_[Body::UP].priority_ );
}

void RobotAnimeController::Add(int type, const std::string& path, float speed, float totalTime)
{
	Animation anim;
	//���[�h
	anim.modelId_ = MV1LoadModel(path.c_str());
	//�A�j���[�V�����^�C�v���C���f�b�N�X�i�V���ȗv�f�𖖔��Ɂj�ɒǉ�
	anim.animIndex_ = type;
	//�X�s�[�h
	anim.speed_ = speed;
	//���Đ�����
	anim.totalTime_ = totalTime;
	isAttach_[static_cast<STATE>(type)] = false;
	//�A�j���[�V�����^�C�v�������ĂȂ��ꍇ�H�����m�F
	if (animations_.count(type) == 0)
	{
		// ����ւ�
		animations_.emplace(type, anim);
	}
	else
	{
		// �ǉ�
		animations_[type].modelId_ = anim.modelId_;
		animations_[type].animIndex_ = anim.animIndex_;
		animations_[type].attachNo_ = anim.attachNo_;
		animations_[type].totalTime_ = anim.totalTime_;
	}
}

void RobotAnimeController::UpperBodyUpdate(void)
{
	//�u�����h���t���O�������Ă����
	if (blend_[Body::UP].blendFlag_)
	{
		//rate��1.0f�Ńu�����h�I��
		if (blend_[Body::UP].rate_ < 1.0f)
		{
			//ROOT�Ɣw��0�́A���A�j�����[�g�𑝂₷
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 60, blend_[Body::UP].rate_, false);
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 61, blend_[Body::UP].rate_, false);
			//�w��0�̑O�A�j�����[�g�����X�Ɍ��炷
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::UP].attachNo_, 61, 1.0f - blend_[Body::UP].rate_, false);
			//�w��1�ȉ��͂��ׂĘA���A�O�����炵�A���𑝂₷
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 62, blend_[Body::UP].rate_, true);
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::UP].attachNo_, 62, 1.0f - blend_[Body::UP].rate_, true);
			//ROOT�͕s���̂��߁A���A�j���̉e�����󂯂Ȃ�
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 60, 0.0f, false);

			blend_[Body::UP].rate_ = blend_[Body::UP].stepBlend_ / BLEND_TIME;
			blend_[Body::UP].stepBlend_ += deltaTime_;
		}
		//�u�����h���I��������
		else
		{
			//�Ɋւ���p�����[�^��������
			blend_[Body::UP].blendFlag_ = false;
			blend_[Body::UP].stepBlend_ = 0.0f;
			blend_[Body::UP].rate_ = 1.0f;
		}
	}
	//�u�����h������Ȃ���Βʏ�Đ�
	else
	{
		//�X�g�b�v�t���O��false
		if (!playAnim_[Body::UP].isStop_)
		{
			// �Đ����Ԃ�i�߂�
			playAnim_[Body::UP].step_ += (deltaTime_ * playAnim_[Body::UP].speed_);
			// �A�j���[�V�����I������
			bool isEnd = false;
			//�A�j���[�V�����i�s���Ԃ��g�[�^�����Ԃ𒴂�����
			if (playAnim_[Body::UP].step_ > playAnim_[Body::UP].totalTime_)
			{
				//�A�j���[�V�����I���t���O�𗧂Ă�
				isEnd = true;
				playAnim_[Body::UP].isEnd = true;
				//�A�j���[�V�����D��Đ��t���O��false�ɂ���
				playAnim_[Body::UP].priority_ = false;
			}
			// �A�j���[�V�������I��������
			if (isEnd)
			{
				//���[�v�Đ��t���O�������Ă����
				if (playAnim_[Body::UP].isLoop_)
				{
					playAnim_[Body::UP].step_ = 0.0f;
				}
				else
				{
					// �X�e�b�v���g�[�^���^�C���ɌŒ�
					playAnim_[Body::UP].step_ = playAnim_[Body::UP].totalTime_;
				}
			}
		}
		else
		{
			playAnim_[Body::UP].priority_ = false;
		}
		// �A�j���[�V�����ݒ�
		MV1SetAttachAnimTime(modelId_, playAnim_[Body::UP].attachNo_, playAnim_[Body::UP].step_);
	}
}

void RobotAnimeController::LowerBodyUpdate(void)
{
	if (blend_[Body::LOW].blendFlag_)
	{
		if (blend_[Body::LOW].rate_ < 1.0f)
		{
			//ROOT�̌��A�j����rate�����炵�A�V�����A�j����rate�𑝂₷
			MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 60, blend_[Body::LOW].rate_, false);
			MV1SetAttachAnimBlendRateToFrame(modelId_, blend_[Body::LOW].attachNo_, 60, 1.0f - blend_[Body::LOW].rate_, false);
			//�����g �E���ȉ��ƍ����ȉ��A��
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
		//�X�g�b�v�t���O��false
		if (!playAnim_[Body::LOW].isStop_)
		{
			// �Đ�
			playAnim_[Body::LOW].step_ += (deltaTime_ * playAnim_[Body::LOW].speed_);

			// �A�j���[�V�����I������
			bool isEnd = false;
			//�A�j���[�V�����i�s���Ԃ��g�[�^�����Ԃ𒴂�����
			if (playAnim_[Body::LOW].step_ > playAnim_[Body::LOW].totalTime_)
			{
				isEnd = true;
				playAnim_[Body::LOW].isEnd = true;
				playAnim_[Body::LOW].priority_ = false;
			}
			// �A�j���[�V������1���[�v���Đ��I�����Ă���Ƃ���
			if (isEnd)
			{
				// ���[�v�Đ���true�Ȃ�
				if (playAnim_[Body::LOW].isLoop_)
				{
					// �Đ��X�e�b�v�����Z�b�g
					playAnim_[Body::LOW].step_ = 0.0f;
				}
				else
				{
					// ���[�v���Ȃ�
					playAnim_[Body::LOW].step_ = playAnim_[Body::LOW].totalTime_;
				}
			}
		}
		else
		{
			playAnim_[Body::LOW].priority_ = false;
		}
		// �A�j���[�V�����ݒ�
		MV1SetAttachAnimTime(modelId_, playAnim_[Body::LOW].attachNo_, playAnim_[Body::LOW].step_);
	}
}

void RobotAnimeController::DetachAnim(void)
{
	//�ǉ�����Ă���A�j���[�V�����̐�����
	for (const auto& anim:isAttach_)
	{
		//�A�^�b�`����Ă��Ȃ���Ζ߂�
		if (!anim.second)
		{
			continue;
		}
		//1 �A�^�b�`����Ă���A�j�������ݍĐ����̏㔼�g�A�j���łȂ�
		//2 �A�^�b�`����Ă���A�j�������ݍĐ����̉����g�A�j���łȂ�
		//3 �A�^�b�`����Ă���A�j�����u�����h���̉����g�A�j���łȂ�
		//1,2,3�S�Ă����Ă͂܂��Ă�����A���̃A�j�����f�^�b�`���āAfalse��Ԃɂ���
		if (anim.first!= playType_[Body::UP] &&
			anim.first!= playType_[Body::LOW]&&
			anim.first!= blend_[Body::UP].type_&&
			anim.first != blend_[Body::LOW].type_
			)
		{
				//�A�j���[�V�������O��
				MV1DetachAnim(modelId_, attachedTypeNum_[anim.first]);
				isAttach_[anim.first] = false;
		}
	}
	
}

void RobotAnimeController::UpperBodyPlay(int type, bool priority, bool isLoop ,
	bool isStop, float endStep , float startStep , bool isForce )
{
	//���ݍĐ����̃A�j���[�V�����ƃ^�C�v���Ⴂ�A�D��Đ��ł��Ȃ��A���A�j���[�V�����^�C�v��-1�łȂ���΃A�j���[�V���������ւ���
	if ( !(playAnim_[Body::UP].priority_) && playType_[Body::UP] != static_cast<STATE>(type) &&  type != -1)
	{
		auto newType = static_cast<STATE>(type);
		if (!blend_[Body::UP].blendFlag_)
		{
			//���݂̃A�j���[�V�����i���o�[���u�����h�A�j���[�V�����i���o�[�Ɋi�[
			blend_[Body::UP].attachNo_ = playAnim_[Body::UP].attachNo_;

			//���݂̃v���C�^�C�v���u�����h�A�j���[�V�����^�C�v�Ɋi�[
			blend_[Body::UP].type_ = playType_[Body::UP];

			//�V�����A�j���[�V���������Đ��A�j���[�V�������Ɋi�[
			playAnim_[Body::UP] = animations_[type];

			//���ݍĐ����^�C�v��V�����A�j���[�V�����ɍX�V
			playType_[Body::UP] = newType;

			//�V�����A�j���[�V�������܂��A�^�b�`����Ă��Ȃ����
			if (!isAttach_[newType])
			{
				//�A�j���[�V�������A�^�b�`���Č��ݍĐ����A�j���[�V�����i���o�[�Ɋi�[
				playAnim_[Body::UP].attachNo_ = MV1AttachAnim(modelId_, 0, playAnim_[Body::UP].modelId_);
				//�A�j���[�V�����e������ς���
				MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::UP].attachNo_, 60, 0.0f, true);
				//�A�^�b�`���t���O��true�ɂ���
				isAttach_[newType] = true;
				//�A�^�b�`���A�j���[�V�����̃A�^�b�`�i���o�[��ۑ�
				attachedTypeNum_[newType] = playAnim_[Body::UP].attachNo_;
				//�Đ��J�n���Ԃ�ݒ�
				playAnim_[Body::UP].step_ = startStep;
			}
			else
			{
				//�A�^�b�`����Ă��Ȃ��ꍇ�A�A�^�b�`����
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
		//�A�j���[�V���������Ԃ̎擾
		if (endStep > 0.0f)
		{
			playAnim_[Body::UP].totalTime_ = endStep;
		}
		else
		{
			playAnim_[Body::UP].totalTime_ = MV1GetAttachAnimTotalTime(modelId_, playAnim_[Body::UP].attachNo_);
		}
		//�A�j���[�V�������[�v
		playAnim_[Body::UP].isLoop_ = isLoop;
		// �A�j���[�V�������Ȃ�
		playAnim_[Body::UP].isStop_ = isStop;
	}
}

void RobotAnimeController::LowerBodyPlay(int type, bool priority , bool isLoop ,
	bool isStop , float endStep, float startStep, bool isForce )
{
	//�A�j���[�V�����^�C�v���Ⴂ�A���A�j���[�V�����^�C�v�������Ă�����
	if (!(playAnim_[Body::LOW].priority_) && playType_[Body::LOW] != static_cast<STATE>(type) && type != -1)
	{
		auto newType = static_cast<STATE>(type);
		if (!blend_[Body::LOW].blendFlag_)
		{
			blend_[Body::LOW].attachNo_ = playAnim_[Body::LOW].attachNo_;
			blend_[Body::LOW].type_ = playType_[Body::LOW];
			playAnim_[Body::LOW] = animations_[type];
			playType_[Body::LOW] = newType;

			//�؂�ւ���̃A�j���[�V�������A�^�b�`�ς݂��ǂ����𔻒�
			if (!isAttach_[newType])
			{
				//�A�^�b�`����Ă��Ȃ��ꍇ�A�A�^�b�`����
				playAnim_[Body::LOW].attachNo_ = MV1AttachAnim(modelId_, 0, playAnim_[Body::LOW].modelId_);
				MV1SetAttachAnimBlendRateToFrame(modelId_, playAnim_[Body::LOW].attachNo_, 60, 0.0f, true);
				isAttach_[newType] = true;
				attachedTypeNum_[newType] = playAnim_[Body::LOW].attachNo_;

				playAnim_[Body::LOW].step_ = startStep;
			}
			else
			{
				//�A�^�b�`�ς݂̏ꍇ���łɂ���A�^�b�`�ԍ������̂܂܎g�p
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
			//������
			//playAnim_[DW].step_ = startStep;
			playAnim_[Body::LOW].isStop_ = isStop;
			playAnim_[Body::LOW].isLoop_ = isLoop;
			playAnim_[Body::LOW].priority_ = priority;
			blend_[Body::LOW].blendFlag_ = true;
			blend_[Body::LOW].rate_ = 0.0f;
		}
		//�A�j���[�V���������Ԃ̎擾
		if (endStep > 0.0f)
		{
			playAnim_[Body::LOW].totalTime_ = endStep;
		}
		else
		{
			playAnim_[Body::LOW].totalTime_ = MV1GetAttachAnimTotalTime(modelId_, playAnim_[Body::LOW].attachNo_);
		}
		//�A�j���[�V�������[�v
		playAnim_[Body::LOW].isLoop_ = isLoop;
		// �A�j���[�V�������Ȃ�
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
		DrawFormatString(0, 430+(20* num), 0xffffff, "�A�j���[�V����:%s", st.c_str());
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

