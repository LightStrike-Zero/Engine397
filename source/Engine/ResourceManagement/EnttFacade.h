#ifndef ENTTFACADE_H
#define ENTTFACADE_H

#include <entt/entt.hpp>
#include <utility>

/// by Hugo. This is a wrapper that helps to facade entt:: when using entt::exclude.
template<typename... Components>
constexpr auto exclude = entt::exclude<Components...>;


class EnttFacade {
public:
    EnttFacade() = default;
    ~EnttFacade() = default;

    using Entity = entt::entity;
    using Registry = entt::registry;

    Entity createEntity() {
        return m_registry.create();
    }

    void destroyEntity(Entity entity) {
        m_registry.destroy(entity);
    }

    template<typename Component, typename... Args>
    Component& addComponent(Entity entity, Args&&... args) {
        return m_registry.emplace<Component>(entity, std::forward<Args>(args)...);
    }

    template<typename Component>
    Component& get(Entity entity) {
        return m_registry.get<Component>(entity);
    }

    template<typename... Components>
    auto view() {
        return m_registry.view<Components...>();
    }

    //created by Hugo, because I need view<Component>(entt::exclude<Component>)
    template<typename... Include, typename... Exclude>
    auto view(entt::exclude_t<Exclude...> ex) {
        return m_registry.view<Include...>(ex);
    }



    template<typename Component>
    void removeComponent(Entity entity) {
        m_registry.remove<Component>(entity);
    }

    template<typename Component>
    bool hasComponent(const Entity entity) const
    {
        return m_registry.all_of<Component>(entity);
    }

    Registry& getRegistry() { return m_registry; }

private:
    Registry m_registry;
};

#endif // ENTTFACADE_H
