#pragma once
#include "../Building.h"

class PowerPlant : public Building
{
public:
    PowerPlant();

    // Override virtual methods to provide specific behavior
    void UpdateEfficiency(float deltaTime) override;
    float CalculateProduction(float deltaTime) const override;
};