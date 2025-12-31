#include "stdafx.h"
#include "Character.h"
#include "Source/Collision/CollisionManager.h"

// 静的メンバの定義
const std::string Character::ANIMATION_FILE_PATH = "Assets/animData/";
const std::string Character::ANIMATION_EXTENSION = ".tka";

namespace
{
	constexpr float GRAVITY_POWER = 9.8f * 10;		// 重力。
	constexpr float DEADZONE = 0.01f;				// スティック入力検知の基準値。
	constexpr float STICK_ACCEL = 2.0f;				// スティック入力による微小押し付けの強さ。

	const std::string MODEL_FILE_PATH = "Assets/modelData/Character/";
	const std::string MODEL_EXTENSION = ".tkm";

	/// <summary>
	/// レイキャストのコールバッククラス。
	/// </summary>
	struct MyRayResultCallback : public btCollisionWorld::RayResultCallback
	{
		Vector3 hitPos;					// rayが当たった位置。
		Vector3 hitNormal;				// rayが当たったポリゴンの法線。
		Vector3 rayStart;				// rayの開始位置。
		Vector3 rayEnd;					// rayの終了位置。
		Vector3 upDirection;			// 上方向ベクトル。
		bool	isHit = false;			// 当たったかどうか。
		float	dist = FLT_MAX;			// 当たった距離の最小値。
		float	maxHitAngle = Math::PI;	// 検知する最大角度（デフォルトは180度＝すべて検知）

		btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override
		{
			// 地面以外に当たった場合は無視する。
			if (rayResult.m_collisionObject->getCollisionFlags() != enCollisionAttr_Ground) {
				return 1.0f;
			}

			// ポインタキャストで法線の取得。
			// NOTE: 無理やり型を変換してコピーするため、安全ではない書き方なので注意。
			Vector3 hitNormalTmp = *(Vector3*)&rayResult.m_hitNormalLocal;

			// 安全な書き方（メンバーごとのコピー）
			//btVector3& btVec = rayResult.m_hitNormalLocal;
			//Vector3 hitNormalTmp;
			//hitNormalTmp.x = btVec.x();
			//hitNormalTmp.y = btVec.y();
			//hitNormalTmp.z = btVec.z();

			float dot = hitNormalTmp.Dot(upDirection);

			// 内積値のプランク。
			if (dot > 1.0f) {
				dot = 1.0f;
			}
			if (dot < -1.0f) {
				dot = -1.0f;
			}

			// 角度計算
			float angle = acosf(dot);
			angle = fabsf(angle);

			// 変数(maxHitAngle)と比較する。
			if (angle < maxHitAngle)
			{
				isHit = true;
				Vector3 hitPosTmp;
				hitPosTmp.Lerp(rayResult.m_hitFraction, rayStart, rayEnd);
				Vector3 vDist = hitPosTmp - rayStart;
				float distTmp = vDist.Length();

				// 最小距離の更新。
				if (dist > distTmp) {
					hitPos = hitPosTmp;
					hitNormal = hitNormalTmp;
					dist = distTmp;
				}
			}
			return rayResult.m_hitFraction;
		}
	};

	/// <summary>
	/// 指定した開始点から終了点までレイを飛ばし、最小距離かつ条件を満たす衝突点を検出します。
	/// 内部で `MyRayResultCallback` を使い、`upDirection` と法線のなす角が `maxAngle` 以下のもののみをヒットとして扱います。
	/// また、開始点と終了点がほぼ同一の場合は処理を行わず false を返します。
	/// </summary>
	/// <param name="rayStart">レイの開始座標。</param>
	/// <param name="rayEnd">レイの終了座標。</param>
	/// <param name="hitPosition">ヒットした位置を格納する出力引数（ヒットしなければ未変更のまま）。</param>
	/// <param name="outNormal">ヒットしたポリゴンの法線を格納する出力引数。</param>
	/// <param name="upDirection">比較用の上方向ベクトル（通常はキャラクターの上方向）。</param>
	/// <param name="maxAngle">許容する最大角度（ラジアン）。この角度より大きい法線は無視される。既定は Math::PI（すべて許可）。</param>
	/// <returns>ヒットした場合は true、ヒットしなければ false を返す。</returns>
	bool RayTest(const Vector3& rayStart, const Vector3& rayEnd, Vector3& hitPosition, Vector3& outNormal, const Vector3& upDirection, float maxAngle = Math::PI)
	{
		if ((rayStart - rayEnd).LengthSq() <= 0.01f) {
			return false;
		}

		MyRayResultCallback rayCallback;
		rayCallback.rayStart = rayStart;
		rayCallback.rayEnd = rayEnd;
		rayCallback.upDirection = upDirection;
		rayCallback.maxHitAngle = maxAngle;

		PhysicsWorld::GetInstance()->RayTest(rayStart, rayEnd, &rayCallback);
		if (rayCallback.isHit) {
			hitPosition = rayCallback.hitPos;
			outNormal = rayCallback.hitNormal;
			return true;
		}
		return false;
	}
}

/// <summary>
/// 地面に向かってレイを飛ばし、当たった座標と自分の座標の距離が一定未満なら接地していると判定します。
/// </summary>
bool Character::GetIsOnGround()
{
	// 移動処理でhitPositionをm_positionに代入しており、レイの判定が不安定になるため、rayStartをm_positionから少し離す。
	Vector3 rayStart = m_position + m_upDirection * 5.0f;
	Vector3 rayEnd = m_planetCenter;
	Vector3 hitPosition = Vector3::Zero;

	// インスタンスメソッドとしてPhysicsWorldのGetInstance()を使い、RayTestを呼び出す
	if (PhysicsWorld::GetInstance()->RayTest(rayStart, rayEnd, hitPosition)) {
		// キャラクター座標と当たった座標の距離を計算。
		Vector3 DistanceToGround = m_position - hitPosition;
		// 距離が一定未満なら接地していると判定。
		if (DistanceToGround.Length() < 2.0f) {
			return true;
		}
		return false;
	}
	return false;
}

/// <summary>
/// 指定されたアニメーション番号のアニメーションを再生します。
/// </summary>
/// <param name="animNo">再生するアニメーションの番号。</param>
void Character::PlayAnimation(const int animNo)
{
	m_modelRender.PlayAnimation(animNo);
}

/// <summary>
/// ジャンプの勢い（jumpPower）を適用し、内部のジャンプ速度を設定します。
/// </summary>
/// <param name="jumpPower">適用するジャンプの強さ（速度）。この値を内部の m_jumpSpeed に設定します。</param>
void Character::ApplyJumpImpulse(const float jumpPower)
{
	m_initialJumpSpeed = jumpPower;
}

/// <summary>
/// moveSpeedに基づいてモデルを回転させます。
/// </summary>
void Character::ModelRotation()
{
	// 惑星の中心からキャラクターへの上方向ベクトルを計算
	Vector3 upDirection = m_upDirection;
	upDirection.Normalize();

	// m_moveSpeedを惑星の接平面に投影し、ジャンプによる垂直成分を除去する。
	Vector3 forward = ProjectOnPlane(m_moveSpeed, upDirection);

	//////////////////////////////////////////////////////////////////////////////////////////////
	///--- 停止時・moveSpeedが真上を向いているときに現在のキャラクターの向きを維持する処理。---///
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (forward.Length() < DEADZONE) {
		//// 現在の回転から、ワールド空間での「前方向」を取得
		//Vector3 currentForward = Vector3::Front;
		//m_rotation.Apply(currentForward);
		//// それを現在の接平面に投影しなおす（惑星表面を移動して法線が変わっている可能性があるため）
		//forward = ProjectOnPlane(currentForward, upDirection);

		// 長さが0の場合は回転できないので、そのまま返す。
		return;
	}

	forward.Normalize();

	// モデルのデフォルトの上方向(0, 1, 0)を、惑星の上方向(upDirection)に回転させるクォータニオンを計算
	Quaternion planetAlignmentRotation;
	planetAlignmentRotation.SetRotation(Vector3::Up, upDirection);

	// 惑星にアライメントされた状態で、モデルの前方向（Vector3::Front）がどこに向いているかを求める
	Vector3 projectedDefaultForward = Vector3::Front;
	planetAlignmentRotation.Apply(projectedDefaultForward); // これが惑星に沿った状態での「前」
	projectedDefaultForward.Normalize();

	// キャラクターが今向いている方向。
	Vector3 projectedTargetForward = forward;
	projectedTargetForward.Normalize();

	// 惑星に沿った状態のデフォルトの前方向から、moveSpeedの方向への回転角度を求める。
	float dotResult = projectedDefaultForward.Dot(projectedTargetForward);
	// acosの引数をクランプ
	if (dotResult < -1.0f) {
		dotResult = -1.0f;
	}
	else if (dotResult > 1.0f) {
		dotResult = 1.0f;
	}
	float rotationAngle = acosf(dotResult);

	// 回転の向き（符号）を外積で判定。
	Vector3 crossProduct = Vector3::Zero;
	crossProduct.Cross(projectedDefaultForward, projectedTargetForward);
	if (crossProduct.Dot(upDirection) < 0.0f) {
		rotationAngle *= -1.0f;
	}

	// Y軸周りの回転クォータニオンを作成。
	Quaternion yRotation;
	yRotation.SetRotation(upDirection, rotationAngle);

	// 「惑星アライメント」と「Y軸回転」を乗算。
	Quaternion targetRotation = yRotation * planetAlignmentRotation;

	// 球面線形補間(Slerp)を入れると、回転が滑らかになる。
	// m_rotation.Slerp(m_rotation, targetRotation, 0.2f); 
	m_rotation = targetRotation;

	m_modelRender.SetRotation(m_rotation);
}

/// <summary>
/// 現在の座標に合わせて、強制的にキャラクターを惑星に対して直立させます。
/// </summary>
void Character::ResetRotation()
{
	UpdateUpDirection();

	Quaternion planetAlignment;
	planetAlignment.SetRotation(Vector3::Up, m_upDirection);

	m_rotation = planetAlignment;
	m_modelRender.SetRotation(m_rotation);
}



/// <summary>
/// キャラクターのモデルとアニメーションクリップを初期化します。
/// </summary>
/// <param name="count">アニメーションクリップの数。</param>
/// <param name="option">各アニメーションクリップの設定情報が格納されたAnimationOption型の配列。</param>
/// <param name="path">モデルファイルのパス。</param>
/// <param name="scale">モデルの拡大率。（規定値は1.0f）</param>
void Character::InitModel(const size_t count, const AnimationOption* option, const std::string path, const float scale)
{
	// ポインタに配列でnewすると、連続で確保される。
	m_animationClips = new AnimationClip[count];

	// アニメーション読み込み
	for (size_t i = 0; i < count; i++) {
		m_animationClips[i].Load(option[i].GetFullPath().c_str());
		m_animationClips[i].SetLoopFlag(option[i].is_loop);
	}

	// モデルの初期化。
	std::string fullModelPath = MODEL_FILE_PATH + path + MODEL_EXTENSION;
	m_modelRender.Init(fullModelPath.c_str(), m_animationClips, count, enModelUpAxisY);

	// モデルの拡大率を設定。
	m_scale = Vector3(scale, scale, scale);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);
}

/// <summary>
/// 「惑星の中心→キャラ」のベクトルを計算し、正規化します。
/// </summary>
void Character::UpdateUpDirection()
{
	m_beforeUpDirection = m_upDirection;
	m_upDirection = m_position - m_planetCenter;
	m_upDirection.Normalize();
}

/// <summary>
/// ジャンプや重力から、垂直方向の速度ベクトルを計算して返します。
/// </summary>
/// <returns> 垂直方向の速度。/returns>
const Vector3 Character::CalcVerticalVelocity()
{
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//速度 = 初速度 - 重力 * 時間。
	float jumpPower = m_initialJumpSpeed - (GRAVITY_POWER * m_fallTimer);

	// 垂直方向に加算。
	Vector3 velocity = m_upDirection * jumpPower;

	return velocity;
}

/// <summary>
/// 移動速度から移動後の座標を計算します。
/// </summary>
void Character::ComputePosition()
{
	// moveSpeedを分解
	Vector3 totalMove = m_moveSpeed;				// 全体の移動量
	Vector3 up = m_upDirection;						// 上方向（重力の逆）
	float verticalComponent = totalMove.Dot(up);	// 上方向の成分だけ抽出
	Vector3 vertMove = up * verticalComponent;		// 垂直移動ベクトル
	Vector3 horiMove = totalMove - vertMove;		// 水平移動ベクトル（＝全体 - 垂直）

	// 2. 水平移動処理
	if (horiMove.LengthSq() > 0.0001f)
	{
		const float WALKABLE_SLOPE_LIMIT = Math::PI * 0.4f; // 登れる角度
		const float CHAR_RADIUS = 15.0f;					// 体の厚み（半径）
		const float SKIN_WIDTH = 1.0f;						// 壁の手前の余白
		const float BACK_CHECK_DIST = 5.0f;					// 埋まり防止の引き撃ち距離
		// 埋まり時の押し出し係数。
		// 壁の表面ぴったりに移動させると壁の中に入ってしまう恐れがあるため、少し余分に壁から離すための補正値。
		const float RECOVERY_BUFFER = 1.0f;

		Vector3 currentPos = m_position;


		// --- 【第1段階】本来の移動方向へのトライ ---
		Vector3 attemptMove = horiMove;	// 水平移動のみを対象にする。
		bool needSlide = false;			// 滑り処理が必要かどうかのフラグ。

		// 水平移動量がある場合のみ処理。
		float moveDist = attemptMove.Length();
		if (moveDist > 0.0001f)
		{
			// 水平移動の方向ベクトルを保存。
			Vector3 dir = attemptMove;
			dir.Normalize();

			// 足元からレイを飛ばすと段差に引っかかるので、少し高い位置（腰など）から飛ばす。
			Vector3 rayOriginOffset = up * 30.0f;
			// startPos：現在地から少し「後ろ」に引いた座標。
			// NOTE：すでに壁に少しめり込んでいた場合、現在地から飛ばすと壁の裏側から飛ばすことになり、検知できないため。
			Vector3 startPos = currentPos + rayOriginOffset - (dir * BACK_CHECK_DIST);
			// checkLength：レイの長さ。
			// 式：引き撃ち分 + 移動したい距離 + 体の半径 + 余白
			// NOTE：中心が壁に到達する前に止めたいので、「体の半径分」余分に先読みする必要があります。
			float checkLength = BACK_CHECK_DIST + moveDist + CHAR_RADIUS + SKIN_WIDTH;
			// endPos：レイの終点。
			Vector3 endPos = startPos + (dir * checkLength);

			// rayが当たった座標を受け取る変数と、rayが当たったポリゴンの法線を受け取る変数を用意。
			Vector3 hitPos, hitNormal;

			if (RayTest(startPos, endPos, hitPos, hitNormal, up))
			{
				// 壁までの距離を算出。
				// distFromCurrent: 「本来の現在地（中心）」から「壁の表面」までの距離
				// 計算式：(レイの全長 - 引き撃ち分)
				float distFromCurrent = (hitPos - startPos).Length() - BACK_CHECK_DIST;
				// availableDistance: 「実際に進める距離」
				// 計算式: 壁までの距離 - 体の半径
				// NOTE：体の表面が壁に触れるところで止めるため。
				float availableDistance = distFromCurrent - CHAR_RADIUS;

				// 埋まったとき（体の表面の座標が壁の裏側になったとき）に、押し出す処理。
				// 法線方向に、埋まっている分と、少し余分に押し出す。
				if (availableDistance < 0.0f) {
					float penetrationDepth = -availableDistance;
					currentPos += hitNormal * (penetrationDepth + RECOVERY_BUFFER);
					availableDistance = 0.0f;
				}

				// upとrayが当たったポリゴンの法線との角度をクランプ・計算する。
				float dot = hitNormal.Dot(up);
				if (dot > 1.0f) {
					dot = 1.0f;
				}
				else if (dot < -1.0f) {
					dot = -1.0f;
				}
				float slopeAngle = acosf(dot);

				// 角度が登れる角度よりも急なら「壁」とみなす。
				if (slopeAngle > WALKABLE_SLOPE_LIMIT)
				{
					// 壁の手前まで移動
					float actualMove = max(0.0f, availableDistance - SKIN_WIDTH);
					Vector3 moveVec = dir * actualMove;
					currentPos += moveVec;

					// 本来の移動ベクトルから、実際に移動した分を引き、移動できなかった分を計算する。
					Vector3 currentRemaining = attemptMove - moveVec;

					// 移動できなかった分を、法線方向に投影して引き算を行う。
					float d = currentRemaining.Dot(hitNormal);
					if (d < 0.0f) {
						attemptMove = currentRemaining - hitNormal * d;
						// 張り付き防止に少しだけ法線方向に移動させる。
						attemptMove += hitNormal * 0.1f;
					}
					else {
						attemptMove = currentRemaining;
					}

					// 滑り移動を行うフラグを立てる
					needSlide = true;
				}
				else {
					// 登れるのでそのまま進んで終了
					currentPos += attemptMove;
					needSlide = false;
				}
			}
			else {
				// 障害物なし：そのまま進んで終了
				currentPos += attemptMove;
				needSlide = false;
			}
		}

		// 直前の移動で滑り処理が必要になった場合、滑り先のチェックを行う。
		// 途中までは直前の処理と同じ。
		if (needSlide && attemptMove.LengthSq() > 0.0001f)
		{
			moveDist = attemptMove.Length();
			Vector3 dir = attemptMove;
			dir.Normalize();

			Vector3 rayOriginOffset = up * 30.0f;
			Vector3 startPos = currentPos + rayOriginOffset - (dir * BACK_CHECK_DIST);
			float checkLength = BACK_CHECK_DIST + moveDist + CHAR_RADIUS + SKIN_WIDTH;
			Vector3 endPos = startPos + (dir * checkLength);
			Vector3 hitPos, hitNormal;

			if (RayTest(startPos, endPos, hitPos, hitNormal, up))
			{
				float distFromCurrent = (hitPos - startPos).Length() - BACK_CHECK_DIST;
				float availableDistance = distFromCurrent - CHAR_RADIUS;

				if (availableDistance < 0.0f) {
					float penetrationDepth = -availableDistance;
					currentPos += hitNormal * (penetrationDepth + RECOVERY_BUFFER);
					availableDistance = 0.0f;
				}

				float dot = hitNormal.Dot(up);
				if (dot > 1.0f) {
					dot = 1.0f;
				}
				else if (dot < -1.0f) {
					dot = -1.0f;
				}
				float slopeAngle = acosf(dot);

				if (slopeAngle > WALKABLE_SLOPE_LIMIT) {
					// 壁の手前まで移動して停止
					// ※ループしないので、ここで移動は打ち止め（角で止まる挙動）
					float actualMove = max(0.0f, availableDistance - SKIN_WIDTH);
					currentPos += dir * actualMove;
				}
				else {
					// 坂とみなし、登る。
					currentPos += attemptMove;
				}
			}
			else
			{
				// 障害物なし：スライド移動完了
				currentPos += attemptMove;
			}
		}

		// 最終位置を確定
		m_position = currentPos;
	}

	// 垂直移動。
	Vector3 rayStartPos = m_position + up * 30.0f;
	Vector3 rayEndPos = m_position + vertMove - (up * 5.0f);
	Vector3 hitPos, hitNormal;

	if (RayTest(rayStartPos, rayEndPos, hitPos, hitNormal, up, Math::PI))
	{
		float dot = hitNormal.Dot(up);
		if (dot > 1.0f) {
			dot = 1.0f;
		}
		if (dot < -1.0f) {
			dot = -1.0f;
		}
		float angle = acosf(dot);

		const float WALKABLE_LIMIT = Math::PI * 0.4f;

		if (angle <= WALKABLE_LIMIT) {
			m_position = hitPos;
			m_initialJumpSpeed = 0.0f;
			m_fallTimer = 0.0f;
			vertMove = Vector3::Zero;
		}
		else {
			m_position = hitPos;
			Vector3 slideVector = vertMove - hitNormal * vertMove.Dot(hitNormal);
			vertMove = slideVector;
		}
	}
	else
	{
		m_position += vertMove;
	}

	// 微小な移動量なら、強制的にゼロにする
	if (horiMove.Length() <= 0.0001f) {
		horiMove = Vector3::Zero;
	}

	// 垂直移動と合成する
	m_moveSpeed = horiMove + vertMove;
}