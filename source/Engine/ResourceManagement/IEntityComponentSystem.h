//
// Created by Shaun on 8/04/2025.
//

#ifndef IENTITYCOMPONENTSYSTEM_H
#define IENTITYCOMPONENTSYSTEM_H
#include <cstdint>
#include <string>

class IEntityComponentSystem
{
public:

    using Entity = uint32_t; // Entity is just a number, and this guarantees the same size across platforms
    
    virtual ~IEntityComponentSystem() = default;
    
    virtual Entity createEntity(const std::string& name = "") = 0;
    virtual void destroyEntity(Entity entity) = 0;

    
};

#endif //IENTITYCOMPONENTSYSTEM_H
