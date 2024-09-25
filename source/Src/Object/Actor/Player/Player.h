#pragma once
#include"../../../Manager/RobotAnimeController.h"
#include"../../../Manager/EffectManager.h"
#include"../../../UI/UserInterface.h"
#include"../../Weapon/BeamRifle.h"
#include"../../Weapon/BeamSaber.h"
#include"../../State/StateBase.h"
#include"../../Comon/Transform.h"
#include"../../../Input/Input.h"
#include"../../Camera/Camera.h"
#include"../ActorBase.h"
#include<string>


class ResourceManager;
class BeamRifle;
class BeamSaber;
class Camera;

class Player:
	public ActorBase

{

public:

	//ジャンプ中の前後左右への最大移動スピード
	static constexpr float MAX_JUMP_MOVE_SPEED = 20.0f;

	//小無敵時間
	static constexpr float BIG_SAFE_TIME = 100.0f;
	//小無敵時間
	static constexpr float SMALL_SAFE_TIME = 70.0f;


	//ジャンプするときの最初の減少量
	static constexpr float JUMP_FAST_RATE = 10.0f; 
	//ブーストゲージ消費量定数
	static constexpr float BOOST_RATE = 20.0f; 
	static constexpr float BOOST_DASH_MOVE_SPEED = 50.0f;
	static constexpr int	MIN_JUMP_BOOST = 1; //ジャンプに必要なブーストゲージの量
	static constexpr float BOOST_MOVE_SPEED = 60.0f;
	static constexpr float MOVE_SPEED = 20.0f;
	static constexpr float MIN_BOOST = 20.0f; //ジャンプに必要なブーストゲージの量
	static constexpr float FALL_MAX_MOVE_SPEED = 35.0f;//FALL状態時移動スピード


	enum class STATE
	{
		NONE,
		RUN,
		JUMP,
		DAMAGE,
		DOWN,
		SHOT,
		IDLE,
		BOOST,
		BOOST_DASH,
		FALL,
		COMBAT1,
		COMBAT_RUN,
		WIN,
		LOSE,

	};	
	
	Player(int playerNum, int playMode);

	~Player();
	void Init(void);

	void Update();

	void Draw(void);

	void PlayEffect(STATE state);

	Camera& GetCamera()const;

	BeamRifle &GetBeamRifle(void)const;

	BeamSaber&GetBeamSaber(void)const;

	const Transform& GetTransform(void) const;

	int GetPlayerNum(void)const;

	float GetDeltaTime(void)const;

	void GetMoveDir(void);

	void GetMoveDirBoostDash(void);

	const bool IsGround(void) const;

	const bool& IsHorming(void) const;

	const VECTOR GetEnemyPos(void) const;

	const VECTOR& GetPlayerPos(void) const;

	const float& GetOffsetBossEnemy(void)const;
	
	void CameraDraw();

	//地面との当たり判定
	void CollisionGravity(void);

	//ダメージのないものとの当たり判定追加用
	void AddCollider(Collider* collider);

	//相手の座標
	void SetEnemyPosition(const VECTOR* enemyPos);

	//相手のHp
	void SetEnemyHp(const float* enemyHp);
	
	//敵のステート情報を設定
	void SetEnemyState(const STATE* enemyState);

	//moveSpeed_の値をセットする関数
	void SetMoveSpeed(float value);

	//maxMoveSpeed_の値をセットする関数
	void SetMaxMoveSpeed(float value);

	//maxMoveSpeed_の値をセットする関数
	void SetGravityPow(float value);

	//maxMoveSpeed_の値をセットする関数
	void SetJumpSpeed(float value);

	//maxMoveSpeed_の値をセットする関数
	void SetJumpPowY(float value);

	//ブ-ストダッシュが終わるときに必ず呼ぶ
	void BoostDashEnd(void);
	
	//プレイヤーの状態をへんこうする
	void ChangeState(std::unique_ptr<StateBase> state_);

	//コントローラー入力情報の取得
	const Input& GetInput() const;

	//移動する関数

	void Move(void);

	//移動する関数
	void MoveBoodtDash(void);

	//ジャンプする関数
	void Jump(void);

	//射撃する
	void Shot(void);

	//ダメージくらい状態に変える
	void ChangeDamageState(void);

	//格闘ダメージくらい状態に変える
	void ChangeDwonState(void);

	//ブーストゲージが一定量超えているかどうか
	 bool IsBoostGaugeSufficient(float RequiredGaugeAmount);

	//無敵時間が終わったかどうか
	//<returns>true:終わっている false:終わっていない</returns>
	 bool IsSafeTimeSufficient(void);

	//ダメージを受けた時や立ち止まるときに移動量を０にする
	void MoveStop(void);

	//jumpPow_を0にする
	void JumpStop(void);

	//JumpPowを徐々に減らしていく
	void JumpPowZero(void);


	//重力_を0にする
	void GravityZero(void);

	//ブーストダッシュをする
	void Boost(void);

	//無敵時間計測(引数の数値をsafeTime_を減算していく)
	void CountSafeTime( float value);

	//無敵時間をリセットする
	void SetSafeTime(float value);

	//ブーストゲージ回復開始計測をリセットする
	void RechargeBoostCountReset(void);

	//着地硬直時間が終わったかどうかを返す
	const bool LandingStunEnded(void)const;

	//攻撃硬直時間が終わったかどうかを返す
	const bool CombatStunEnded(void)const;

	//すべての硬直時間が終わっていたらtrue
	const bool AllStanEnded(void)const;

	//攻撃硬直時間設定
	void SetCombatStan(float stanTime);

	//重力を加算する
	void CalcGravity();

	void ResetShotFlame(void);

	//アニメーションコントローラーのパラメータを表示(デバッグ用)
	void RobotAnimDebugDraw(int playerType);

	//プレイヤーの各種パラメータを表示(デバッグ用)
	void PlayerDebugDraw(int playerType);

	//格闘攻撃状態に移行させる関数
	void Combat(void);

	//ブーストダッシュをする方向を決める関数
	void BoostQuaRot(void);

	//ブーストゲージを消費する関数
	void ConsumeBoostGauge(float rate);

	//ブーストゲージの回復を停止させる
	void StopRechargeBoost(void);

	bool IsDead(void);

	void Lose(void);

	void Win(void);

	const STATE& GetState(void);

	//スーパーアーマーを削る
	void DamageSuperArmor(void);

	//スーパーアーマーが残っているかどうかを判定
	bool IsSuperArmor(void);

	 std::unique_ptr<UserInterface> MoveUI(void);

	//回転の設定用変数
	Quaternion quaRot_;

	//プレイヤーの現在の状態
	STATE pState_;

private:

	enum class PLAYER_NUM
	{
		PLAYER_1, 
		PLAYER_2
	};

	enum class PLAY_MODE
	{
		SINGLE_MODE,
		BATTLE_MODE
	};


	bool isHorming_;

	//敵のステート情報
	const STATE* enemyState_;

	PLAY_MODE playMode_;

	//UI
	std::unique_ptr<UserInterface> userInterface_;

	//エフェクト再生
	std::unique_ptr<EffectManager> effectManager_;

	//
	Transform transform_;
	//ビームライフル
	std::unique_ptr<BeamRifle> beamRifle_;	

	//ビームサーベル
	std::unique_ptr<BeamSaber> beamSaber_;	

	//状態の管理
	std::unique_ptr<StateBase> state_;

	//コントローラ入力
	const std::unique_ptr<Input>input_;	

	//カメラ
	const std::unique_ptr<Camera> camera_;	

	//移動方向
	Quaternion moveQua_;

	//移動方向
	Quaternion moveBoostQua_;

	//移動後の座標
	VECTOR movedPos_;

	// 移動量
	VECTOR movePow_;

	//移動するスピード
	float moveSpeed_;

	//移動する最大スピード
	float maxMoveSpeed_;

	//ジャンプの力
	VECTOR jumpPow_;

	//ジャンプのスピード
	float jumpSpeed_;

	std::string debugString_;
	std::string enemyDebugString_;

	//左スティック入力をプレイヤーの移動方向と対応させるもの
	VECTOR padDir_;

	VECTOR padDirBoost_;

	float padX_,padY_;

	//ステージなどの障害物当たり判定用
	std::vector<Collider*>colliders_;

	//自分がプレイヤー1なのか２かの判断
	int playerNum_;	
	//自分がパッド1なのか２かの判断
	int padNum_;	

	//次射撃できるまでの時間
	float shotFlame_;
	//射撃可能フラグ
	bool shotFlag_;	

	//敵と自分との角度
	float angle_;

	float  offsetEnemy_; 

	//自分から敵へのベクトル

	VECTOR enemyVec_;

	//フォール中地面着地フラグ
	bool groundedFlag_;

	//上下半身の回転を元に戻すフラグ
	float revertFlag_;

	//到達するべき回転
	Quaternion goalUpperQuaRotY_;

	//始まりの回転
	Quaternion startUpperQuaRotY_;

	//重力の向き
	VECTOR dirGravity_;

	//重力の逆向き
	VECTOR dirUpGravity_;

	//足元当たり判定用
	float jumpDot_;	

	//重力の強さ
	float gravityPow_;

	//ブーストゲージ
	float boostGauge_;

	//ブーストゲージ回復開始判定 true:回復中　false:回復停止
	bool rechargeBoostFlag_;

	//ブーストゲージが回復し始めるまでの時間
	float rechargeBoostCount_;

	//プレイヤー無敵時間
	float safeTime_;

	//着地硬直時間
	float landingStanTime_;

	//近接攻撃硬直時間
	float combatStanTime_;

	//ホーミング弾実装用のテスト変数
	const VECTOR* enemyPos_;

	//敵との距離
	float enemyDistance_;

	//てきHP
	const float* enemyHp_;

	bool isWin_;

	//格闘移行状態時に受け止められる弾の数
	int superArmor_;

	/// ゲージが任意の数値以上かどうかを判定する
	/// <returns>true:以上である false:未満である</returns>
	const bool IsGaugeSufficient(float Gauge, float RequiredGaugeAmount)const;


	//プレイヤーの状態によって移動スピード等を変える
	void DebugPlayerState(void);

	//プレイヤーの状態によって移動スピード等を変える
	void EnemyState(void);

	//最大
	void SpeedAdd(void);

	void CalculateAngleToTarget();

	//上半身の回転を元に戻す関数
	void RevertRotate();

	//ブーストゲージを回復する
	void RecoverBoostGauge(void);

	void AddToGauge(float& gauge, float add);

	void ConsumeGauge(float& gauge, float rate);

	void InitValue(float& target);

	//行動不能時間計測
	void CountLandingStanTime(void);

	//行動不能時間計測
	void CountCombatStanTime(void);

	//スティック入力がされているときに移動方向を設定する
	void SetMoveQuaRot(void);

	//スティック入力がされているときに移動方向を設定する
	void SetBoostQuaRot(void);

	//スティック入力がされていないときにspeedを徐々に減らしていく
	void MoveSpeedZero(void);


	bool StickAdjustment(VECTOR v1, VECTOR v2);

	//回転、座標などを更新する
	//transform_.pos と transform_.quaRot
	void TransformUpdate(void);

	void WeaponUpdate(void);

	//相手との距離を測る関数
	void RangeDistance(void);

	//相手との距離によって弾が相手を追従するかどうかを判定する
	void Range(void);

	void MakeObjects(void); 

	void InitTransform(void);

	void InitParameter(void);

	virtual void InitAnimation(void) override;

	void InitCamera(void);

	void UpdateBattleMode(void);

	void UpdateSIngleMode(void);

};	


