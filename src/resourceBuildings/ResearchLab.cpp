#include "ResearchLab.h"
#include "../GameConstants.h"

ResearchLab::ResearchLab()
    : Building(BuildingType::RESEARCH_LAB,
               "Research Lab",
               2000.0f, // cost
               0.1f,    // base production rate
               {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false),
                Resource(ResourceType::CRYSTAL, "Crystal", 0.0f, GameConstants::CRYSTAL_BASE_PRICE, false)}, // input resources
               {},                                                                                           // no direct resource output, instead provides global production bonus
               GameConstants::RESEARCH_LAB_MAINTENANCE,                                                      // maintenance cost
               1000.0f,                                                                                      // upgrade cost
               50)                                                                                           // required reputation
{
}

void ResearchLab::UpdateEfficiency()
{
    // Research labs require both energy and crystals to operate efficiently
    bool hasEnergy = false;
    bool hasCrystal = false;
    for (const auto &resource : GetInputResources())
    {
        if (resource.GetType() == ResourceType::ENERGY && resource.GetAmount() > 0)
        {
            hasEnergy = true;
        }
        else if (resource.GetType() == ResourceType::CRYSTAL && resource.GetAmount() > 0)
        {
            hasCrystal = true;
        }
    }

    if (hasEnergy && hasCrystal)
    {
        SetEfficiency(1.0f);
    }
    else if (hasEnergy || hasCrystal)
    {
        SetEfficiency(0.3f); // Partial efficiency with only one resource
    }
    else
    {
        SetEfficiency(0.1f); // Very low efficiency without resources
    }
}

float ResearchLab::CalculateProduction(float deltaTime) const
{
    // Research labs don't produce resources directly
    // Instead, they provide a global production bonus through the CalculateProductionMultiplier method
    return 0.0f;
}