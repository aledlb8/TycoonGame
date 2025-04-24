#pragma once
#include "../Building.h"

class DiamondMine : public Building
{
public:
    DiamondMine();
    virtual ~DiamondMine() = default;

    virtual void UpdateEfficiency(float deltaTime) override;
    virtual float CalculateProduction(float deltaTime) const override;
}; 