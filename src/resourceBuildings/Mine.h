#pragma once
#include "../Building.h"

class Mine : public Building
{
public:
    Mine();

    // Override virtual methods to provide specific behavior
    void UpdateEfficiency(float deltaTime) override;
    float CalculateProduction(float deltaTime) const override;
};