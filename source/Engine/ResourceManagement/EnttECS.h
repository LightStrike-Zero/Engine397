//
// Created by Shaun on 8/04/2025.
//

#ifndef ENTTECS_H
#define ENTTECS_H

#include "IEntityComponentSystem.h"

#include <entt/entt.hpp>
#include <utility> 

class EnttECS : public IEntityComponentSystem {
public:
    EnttECS() = default;
    virtual ~EnttECS() = default;

    // Create an entity.
    Entity createEntity(const std::string& name = "") override {
        auto entity = m_registry.create();
        // Optionally assign a name component if desired.
        return static_cast<Entity>(entity);
    }

    // Destroy an entity.
    void destroyEntity(Entity entity) override {
        m_registry.destroy(static_cast<entt::entity>(entity));
    }

    // Templated helper function to add a component to an entity.
    template <typename Component, typename... Args>
    Component& addComponent(Entity entity, Args&&... args) {
        return m_registry.emplace<Component>(static_cast<entt::entity>(entity), std::forward<Args>(args)...);
    }

    // Templated helper function to remove a component from an entity.
    template <typename Component>
    void removeComponent(Entity entity) {
        m_registry.remove<Component>(static_cast<entt::entity>(entity));
    }

    // Templated helper function to check if an entity has a component.
    template <typename Component>
    bool hasComponent(Entity entity) {
        return m_registry.all_of<Component>(static_cast<entt::entity>(entity));
    }

    // Templated helper function to retrieve a component from an entity.
    template <typename Component>
    Component& getComponent(Entity entity) {
        return m_registry.get<Component>(static_cast<entt::entity>(entity));
    }

    // **Key addition:** Expose the templated view over a set of components.
    template <typename... Components>
    auto view() {
        return m_registry.view<Components...>();
    }

    // (Optional) Expose the underlying registry for advanced usage.
    // entt::registry& getRegistry() { return m_registry; }

private:
    entt::registry m_registry;
};

#endif //ENTTECS_H
