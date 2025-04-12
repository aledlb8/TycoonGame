#pragma once
#include "Building.h"
#include <memory>

class BuildingFactory
{
public:
    static std::unique_ptr<Building> CreateBuilding(BuildingType type);

    // Helper method to get all available building types
    static std::vector<BuildingType> GetAvailableBuildingTypes();
};