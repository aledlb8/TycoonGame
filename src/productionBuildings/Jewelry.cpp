#include "Jewelry.h"
#include "../GameConstants.h"

Jewelry::Jewelry()
    : Production(ProductionType::JEWELRY,                                                                         // type
               "Jewelry Manufacturing",                                                                           // name
               1180.0f,                                                                                           // cost
               {},                                                                                                // no input resources
               { Resource(ResourceType::MONEY, "Money", 0.0f, 1000, false) },                                     // output resources
               0.0f,                                                                                              // time
               1000.0f,                                                                                           // completionTime
               2000.0f,                                                                                           // completionAmount
               26)                                                                                                // required reputation
{
}
