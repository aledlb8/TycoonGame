#pragma once
#include "../Building.h"

class ResearchLab : public Building
{
public:
    ResearchLab();

    // Override virtual methods to provide specific behavior
    void UpdateEfficiency(float deltaTime) override;
    float CalculateProduction(float deltaTime) const override;
};