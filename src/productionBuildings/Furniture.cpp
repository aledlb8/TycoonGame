#include "Furniture.h"
#include "../GameConstants.h"

Furniture::Furniture()
    : Production(ProductionType::FURNITURE,                                                                       // type
               "Furniture Hut",                                                                                   // name
               200.0f,                                                                                            // cost
               {},                                                                                                // no input resources
               { Resource(ResourceType::MONEY, "Money", 0.0f, 1000, false) },                                     // output resources
               0.0f,                                                                                              // time
               50.0f,                                                                                             // completionTime
               250.0f,                                                                                            // completionAmount
               3)                                                                                                 // required reputation
{
}
