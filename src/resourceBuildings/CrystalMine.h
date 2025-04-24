#pragma once
#include "../Building.h"

class CrystalMine : public Building
{
public:
    CrystalMine();

    // Override virtual methods to provide specific behavior
    void UpdateEfficiency(float deltaTime) override;
    float CalculateProduction(float deltaTime) const override;
};