#pragma once
#include "Building.h"
#include "Production.h"
#include <memory>

class BuildingFactory
{
public:
    static std::unique_ptr<Building> CreateBuilding(BuildingType type);
    static std::vector<BuildingType> GetAvailableBuildingTypes();         // Helper method to get all available building types

    static std::unique_ptr<Production> CreateProduction(ProductionType type);
    static std::vector<ProductionType> GetAvailableProductionTypes();     // Helper method to get all available production types

};