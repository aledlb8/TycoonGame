#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Resource.h"
#include "Building.h"
#include "GameConstants.h"

// Forward declarations
// struct Building;  // Remove these as we're using the proper classes
// struct Resource;

// Resource type enum is now defined in Resource.h
// enum class ResourceType { ... }

// Resource structure is now defined in Resource.h
// struct Resource { ... }

// Building type enum is now defined in Building.h
// enum class BuildingType { ... }

// Building structure is now defined in Building.h
// struct Building { ... }

// Player structure
class Player
{
public:
    std::string name;
    std::map<ResourceType, Resource> resources;
    std::vector<std::unique_ptr<Building>> buildings;
    float money;
    int reputation;
    float totalEarnings;
    float totalSpent;
    int achievements;
};

// Game state class
class TycoonGame
{
public:
    TycoonGame();
    ~TycoonGame();

    // Game initialization
    void Initialize();

    // Game loop functions
    void Update(float deltaTime);
    void Render();

    // Game mechanics
    bool BuildStructure(BuildingType type);
    bool SellStructure(int buildingIndex);
    void UpdateResources(float deltaTime);
    void UpdateEconomy(float deltaTime);
    bool BuyResource(ResourceType type, float amount);
    bool SellResource(ResourceType type, float amount);
    void UpdateReputation();
    bool UpgradeBuilding(int buildingIndex);

    // Save/Load functionality
    bool SaveGame(const std::string& filename = "savegame.json") const;
    bool LoadGame(const std::string& filename = "savegame.json");

    // Getters
    const Player &GetPlayer() const { return m_player; }
    float GetGameTime() const { return m_gameTime; }
    bool IsPaused() const { return m_isPaused; }
    float GetFPS() const { return m_fps; }

    // Setters
    void SetPaused(bool paused) { m_isPaused = paused; }

private:
    // Game state
    Player m_player;
    float m_gameTime;
    bool m_isPaused;
    float m_economyUpdateTimer;
    float m_resourceUpdateTimer;
    float m_reputationUpdateTimer;
    float m_maintenanceUpdateTimer;
    float m_lastFrameTime;
    float m_fps;
    int m_frameCount;
    float m_fpsUpdateTimer;

    // Helper functions
    void InitializeResources();
    void InitializeBuildingTypes();
    float CalculateResourcePrice(ResourceType type) const;
    float CalculateProductionMultiplier() const;

    // GUI rendering functions
    void RenderMainMenu();
    void RenderResourcesWindow();
    void RenderBuildingsWindow();
    void RenderMarketWindow();
    void RenderStatsWindow();
};