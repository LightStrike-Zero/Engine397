#ifndef ENTTFACADE_H
#define ENTTFACADE_H

#include <entt/entt.hpp>
#include <utility>

class EnttFacade {
public:
    EnttFacade() = default;
    ~EnttFacade() = default;

    using Entity = entt::entity;
    using Registry = entt::registry;

    // Create an entity.
    Entity createEntity() {
        return m_registry.create();
    }

    // Destroy an entity.
    void destroyEntity(Entity entity) {
        m_registry.destroy(entity);
    }

    // Templated method: add a component to an entity.
    template<typename Component, typename... Args>
    Component& addComponent(Entity entity, Args&&... args) {
        return m_registry.emplace<Component>(entity, std::forward<Args>(args)...);
    }

    // Templated method: get a component from an entity.
    template<typename Component>
    Component& get(Entity entity) {
        return m_registry.get<Component>(entity);
    }

    // Templated method: get a view for components.
    template<typename... Components>
    auto view() {
        return m_registry.view<Components...>();
    }

    // Optionally add other helper functions (e.g., removeComponent, hasComponent, etc.)
    template<typename Component>
    void removeComponent(Entity entity) {
        m_registry.remove<Component>(entity);
    }

    template<typename Component>
    bool hasComponent(const Entity entity) const
    {
        return m_registry.all_of<Component>(entity);
    }

    // Expose the underlying registry if necessary (but try to keep usage internal).
    Registry& getRegistry() { return m_registry; }

private:
    Registry m_registry;
};

#endif // ENTTFACADE_H
