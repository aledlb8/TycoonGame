#pragma once
#include <map>
#include "Resource.h"

class ResourceManager
{
public:
    static ResourceManager &Instance()
    {
        static ResourceManager inst;
        return inst;
    }

    void Add(ResourceType type, float amount)
    {
        m_resources[type] += amount;
    }

    // Try to consume; returns false if insufficient
    bool Consume(ResourceType type, float amount)
    {
        auto &stored = m_resources[type];
        if (stored >= amount)
        {
            stored -= amount;
            return true;
        }
        return false;
    }

    // Query how much you have
    float Get(ResourceType type) const
    {
        auto it = m_resources.find(type);
        return it == m_resources.end() ? 0.0f : it->second;
    }

private:
    std::map<ResourceType, float> m_resources;
};