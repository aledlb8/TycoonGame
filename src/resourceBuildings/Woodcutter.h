#pragma once
#include "../Building.h"

class Woodcutter : public Building
{
public:
    Woodcutter();

    // Override virtual methods to provide specific behavior
    void UpdateEfficiency(float deltaTime) override;
    float CalculateProduction(float deltaTime) const override;
};