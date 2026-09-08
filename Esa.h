#pragma once
#include "Engine/GameObject.h"

enum EsaType
{
    ESATYPE_NORMAL,
    ESATYPE_POWER,
    ESATYPE_MAX
};

class Esa :
    public GameObject
{
public:
    Esa(GameObject* parent);
    ~Esa();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void SetEsaType(EsaType type);
    void OnCollision(GameObject* pTarget)override;
    void Esatype();
private:
    EsaType type_;
    int hModel_;
    int score_;
};

