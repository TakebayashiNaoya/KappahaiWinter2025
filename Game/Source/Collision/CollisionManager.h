#pragma once

class Character;

enum EnCollisionType
{
	enCollisionType_None = 0,
	enCollisionType_Player,
	enCollisionType_BasicEnemy,
	enCollisionType_TransformEnemy,
	enCollisionType_BossEnemy,
	//
	enCollisionType_Num,
};




struct CollisionInformation
{
	EnCollisionType m_type = enCollisionType_None;
	CollisionObject* m_collision = nullptr;
	IGameObject* m_object = nullptr;
	//
	CollisionInformation(const EnCollisionType type, CollisionObject* collision, IGameObject* object) : m_type(type), m_collision(collision), m_object(object) {}
};


struct CollisionPair
{
	CollisionInformation* m_left = nullptr;
	CollisionInformation* m_right = nullptr;
	//
	CollisionPair(CollisionInformation* left, CollisionInformation* right) : m_left(left), m_right(right) {}
};





// 当たり判定を管理するクラス
// 当たったという処理をまとめたい
class CollisionHitManager
{
private:
	/** 当たり判定オブジェクトのリスト */
	std::vector<CollisionInformation> m_collisionInformationList;
	/** 当たり判定のペア */
	std::vector<CollisionPair> m_collisionPairList;


private:
	CollisionHitManager();
	~CollisionHitManager();


public:
	void Update();


public:
	void Register(const EnCollisionType type, CollisionObject* collisionObject, IGameObject* gameObject);
	void Unregister(CollisionObject* collisionObject);


private:
	// NOTE: 衝突処理関数の共通の引数と戻り値に関する説明
	// 
	// <param name="pair">
	//     衝突したオブジェクトのペア（CollisionPair）を入れます。
	//     このペアは、m_collisionInformationListからの要素の組み合わせです。
	// </param>
	// <returns>
	//     ペアが該当する組み合わせ（例: プレイヤー＆基本エネミー）の場合は処理を実行し、trueを返します。
	//     それ以外はfalseを返し、Update関数内の次の判定に移ります。
	// </returns>


	/// <summary>
	/// 「プレイヤー」と「基本エネミー」の衝突処理を行います。
	/// </summary>
	bool UpdateHitPlayerBasicEnemy(CollisionPair& pair);

	/// <summary>
	/// 「プレイヤー」と「変形エネミー」の衝突処理を行います。
	/// </summary>
	bool UpdateHitPlayerDeformEnemy(CollisionPair& pair);

	/// <summary>
	/// 「プレイヤー」と「ボスエネミー」の衝突処理を行います。
	/// </summary>
	bool UpdateHitPlayerBossEnemy(CollisionPair& pair);

	/// <summary>
	/// 「基本エネミー」と「変形エネミー」の衝突処理を行います。
	/// </summary>
	bool UpdateHitBasicEnemyDeformEnemy(CollisionPair& pair);


private:
	/**
	 * 指定したクラスを取得する
	 * NOTE: 指定したクラスが存在しない場合はnullptrを返す
	 */
	template <typename T>
	T* GetTargetObject(CollisionPair& pair, const EnCollisionType type)
	{
		if (pair.m_left->m_type == type)
		{
			return static_cast<T*>(pair.m_left->m_object);
		}
		else if (pair.m_right->m_type == type)
		{
			return static_cast<T*>(pair.m_right->m_object);
		}
		return nullptr;
	}


private:
	// ここに関数を追加していく。


public:
	/// <summary>
	/// 箱型のコライダーを生成し、コリジョンヒットマネージャーに登録します。
	/// </summary>
	/// <param name="ins"> コライダーを作成するキャラクターのポインタ。</param>
	/// <param name="type"> 作成するコライダーの種類を指定する列挙型（EnCollisionType）。</param>
	/// <param name="size"> 箱のサイズ（幅・高さ・奥行き）。</param>
	/// <returns> コライダーのポインタ。</returns>
	CollisionObject* CreateCollider(Character* ins, const EnCollisionType type, const Vector3 size);
	/// <summary>
	/// 球型のコライダーを生成し、コリジョンヒットマネージャーに登録します。
	/// </summary>
	/// <param name="ins"> コライダーを作成するキャラクターのポインタ。</param>
	/// <param name="type"> 作成するコライダーの種類を指定する列挙型（EnCollisionType）。</param>
	/// <param name="size"> 球の半径。</param>
	/// <returns> コライダーのポインタ。</returns>
	CollisionObject* CreateCollider(Character* ins, const EnCollisionType type, const float radius);
	/// <summary>
	/// カプセル型のコライダーを生成し、コリジョンヒットマネージャーに登録します。
	/// </summary>
	/// <param name="ins"> コライダーを作成するキャラクターのポインタ。</param>
	/// <param name="type"> 作成するコライダーの種類を指定する列挙型（EnCollisionType）。</param>
	/// <param name="size"> カプセルのサイズ。（半径・高さ）。</param>
	/// <returns> コライダーのポインタ。</returns>
	CollisionObject* CreateCollider(Character* ins, const EnCollisionType type, const float radius, const float height);
	/// <summary>
	/// コライダーの座標と回転を更新します。
	/// NOTE:モデルの基準が足元、コライダーの基準が中心のため、up方向に位置補正を行う必要があります。
	/// MEMO:コライダーの実体自体を生成・削除するわけではないので、*（値渡し）でOK。
	/// </summary>
	/// <param name="ins"> コライダーを更新するキャラクターのポインタ。</param>
	/// <param name="collider"> 更新するコライダーのポインタ。</param>
	/// <param name="offset"> up方向の位置補正の値。</param>
	void UpdateCollider(const Character* ins, CollisionObject* collider, const float offset = 0.0f);


	/// <summary>
	/// コリジョンヒットマネージャーの登録解除を行い、コライダーをdelete、nullptrします。
	/// </summary>
	/// <param name="collision"> 削除するコライダーのポインタの参照。</param>
	static CollisionObject* DeleteCollider(CollisionObject* collision);


	/**
	 * シングルトン関連
	 */
private:
	static CollisionHitManager* m_instance;


public:
	static CollisionHitManager* CreateInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new CollisionHitManager();
		}
		return m_instance;
	}
	static CollisionHitManager* GetInstance()
	{
		return m_instance;
	}
	static bool IsAvailable()
	{
		return m_instance != nullptr;
	}
	static void Delete()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
};



/// <summary>
/// 当たり判定管理クラスを更新したりするためのゲームオブジェクト。
/// </summary>
class CollisionManagerObject : public IGameObject
{
public:
	CollisionManagerObject();
	~CollisionManagerObject();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& renderContext) override {}	// Renderはない
};

