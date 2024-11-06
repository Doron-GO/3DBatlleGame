#pragma once
#include "../../Common/Quaternion.h"
#include <vector>
#include <DxLib.h>

class SceneManager;
class Transform;
class Collider;
class Controller;

class Camera
{

public:





	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW
	};

	Camera();
	~Camera();

	//初期設定
	void Init(void);

	//カメラの更新処理
	void SetBeforeDraw(void);

	//定点カメラ
	void SetBeforeDrawFixedPoint(void);

	//追従カメラ
	void SetBeforeDrawFollow(void);

	//トランスフォームの設定
	void SetTransform(const Transform* transform);

	//回転を取得
	Quaternion GetQuaRot(void) const;

	//X回転を無くした回転を取得
	Quaternion GetQuaRotOutX(void) const;

	//追従対象座標の設定
	void SetTargetPos(VECTOR pos);
	//座標の設定
	void SetCameraPos(VECTOR pos);

	//モードの変更
	void ChangeMode(MODE mode);

private:

	//カメラの初期設定
	void SetDefault(void);

	// Transformの位置と同期を取る
	void SyncTransform(void);
	// Transformの位置と同期を取る
	void SyncEnemyTransform(void);

	// カメラが追従対象とするTransform
	const Transform* syncTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR cameraPos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion quaRotOutX_;

	// カメラ角度
	Quaternion quaRot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	//敵の位置
	VECTOR enemyPos_;
 
};

