/**
 * @file EnttFacade.h
 * @brief A wrapper around EnTT registry to simplify entity/component operations.
 * @author Shaun
 * @date March 2025
 */

#ifndef ENTTFACADE_H
#define ENTTFACADE_H

#include <entt/entt.hpp>
#include <utility>
#include "Components/NameComponent.h"

/// by Hugo. This is a wrapper that helps to facade entt:: when using entt::exclude.
template<typename... Components>
constexpr auto exclude = entt::exclude<Components...>;


/**
 * @class EnttFacade
 * @brief A wrapper around EnTT registry to simplify entity/component operations.
 */
class EnttFacade {
public:
    EnttFacade() = default;
    ~EnttFacade() = default;

    using Entity = entt::entity;
    using Registry = entt::registry;

    /**
     * @brief Creates a new entity in the registry.
     * @return The created entity.
     */
    Entity createEntity() {
        return m_registry.create();
    }

    /**
     * @brief Destroys the specified entity.
     * @param entity The entity to destroy.
     */
    void destroyEntity(Entity entity) {
        m_registry.destroy(entity);
    }

    /**
     * @brief Adds a component to an entity.
     * @tparam Component The component type.
     * @tparam Args Constructor arguments for the component.
     * @param entity The entity to add the component to.
     * @param args The arguments used to construct the component.
     * @return Reference to the added component.
     */
    template<typename Component, typename... Args>
    Component& addComponent(Entity entity, Args&&... args) {
        return m_registry.emplace<Component>(entity, std::forward<Args>(args)...);
    }

    /**
    * @brief Gets a component from an entity.
    * @tparam Component The component type.
    * @param entity The entity to retrieve the component from.
    * @return Reference to the component.
    */
    template<typename Component>
    Component& get(Entity entity) {
        return m_registry.get<Component>(entity);
    }


    /**
     * @brief Returns a view of all entities with the specified components.
     * @tparam Components The component types.
     * @return A view over entities with those components.
     */
    template<typename... Components>
    auto view() {
        return m_registry.view<Components...>();
    }

    /**
     * @brief Returns a view of all entities with the specified components, but excluding specified components.
     * @param Include The desired component types
     * @param Exclude The excluded component types
     * @return A view over entities with those components.
     */
    template<typename... Include, typename... Exclude>
    auto view(entt::exclude_t<Exclude...> ex) {
        return m_registry.view<Include...>(ex);
    }


    /**
     * @brief Removes a component from an entity.
     * @tparam Component The component type.
     * @param entity The entity to remove the component from.
     */

    template<typename Component>
    void removeComponent(Entity entity) {
        m_registry.remove<Component>(entity);
    }



    /**
     * @brief Checks if an entity has a specific component.
     * @tparam Component The component type.
     * @param entity The entity to check.
     * @return True if the entity has the component, false otherwise.
     */
    template<typename Component>
    bool hasComponent(const Entity entity) const
    {
        return m_registry.all_of<Component>(entity);
    }



    template<typename Component>
    bool setComponentByName(const std::string& name, const Component& component) {
        bool flag = true;
        auto view = m_registry.view<NameComponent>();

        for (auto entity : view) {
            const auto& nameComp = view.get<NameComponent>(entity);
            if (nameComp.name == name) {
                if (m_registry.all_of<Component>(entity)) { //if the component exists, replace
                    m_registry.replace<Component>(entity, component);
                } else { //if the component do not exist, create one
                    m_registry.emplace<Component>(entity, component);
                }
                return flag; // setting done
            }
        }
        flag = false;
        return flag; //no matching entity found
    }

    template<typename Component>
    void replaceComponent(Entity entity, const Component& component) {
        m_registry.replace<Component>(entity, component);
    }


    /**
     * @brief Provides access to the underlying registry.
     * @return Reference to the registry.
     */
    Registry& getRegistry() { return m_registry; }

private:
    Registry m_registry;
};

#endif // ENTTFACADE_H
