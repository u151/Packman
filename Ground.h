#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Esa.h"

class Ground :
	public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Ground(GameObject* parent);
	//初期化
	void Initialize() override;
	std::vector<std::vector<int>> GetMapData() { return mapData_; }
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//開放
	void Release() override;
	void DecEsaCount(EsaType type);
	void RespawnEsa();
private:
	int hModel_;
	int hEsaModel_;
	int hModelt_;
	int hPEsaModel_;
	std::vector<std::vector<int>> mapData_;
	std::vector<std::vector<int>> objMap_;
	int mapWidth_;
	int mapHeight_;
	int normalEsaCount_;
	int powerEsaCount_;
	bool respawnEsa_;
};