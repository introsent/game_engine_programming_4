#pragma once
#include "SimpleEnemy.h"
#include "MultiQbertPositionProxy.h"
class SlickSam : public SimpleEnemy {
public:
    SlickSam(dae::GameObject* owner, Level* level, glm::ivec2 spawnGridPosition,
        const MultiQbertPositionProxy& qbertPositionProxy, bool isSlick);

    bool ShouldDamageQBert() const override { return false; }

private:
    void GenerateNextMove() override;
    void OnLand() override;

    bool m_isSlick; 
};