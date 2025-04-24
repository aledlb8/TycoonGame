#include "ResearchLab.h"
#include "../GameConstants.h"
#include "../ResourceManager.h"

ResearchLab::ResearchLab()
    : Building(
          BuildingType::RESEARCH_LAB,
          "Research Lab",
          2000.0f,
          0.1f,
          {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false),
           Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false)},
          {}, // no direct outputs
          GameConstants::RESEARCH_LAB_MAINTENANCE,
          1000.0f,
          50)
{
}

void ResearchLab::UpdateEfficiency(float deltaTime)
{
    Building::UpdateEfficiency(deltaTime);

    auto &rm = ResourceManager::Instance();
    bool e = rm.Get(ResourceType::ENERGY) > 0.0f;
    bool c = rm.Get(ResourceType::CRYSTAL) > 0.0f;
    float factor = (!e && !c)   ? 0.1f
                   : (!e || !c) ? 0.3f
                                : 1.0f;
    SetEfficiency(GetEfficiency() * factor);
}

float ResearchLab::CalculateProduction(float /*dt*/) const
{
    return 0.0f; // no direct resource output
}