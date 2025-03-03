//
// Created by Shaun on 28/12/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include <entt/entt.hpp>

#include "Lights/Light.h"

class Scene
{
public:
    Scene();
    ~Scene();

    entt::entity createEntity(const std::string& name = "");
    void destroyEntity(entt::entity entity);

    entt::registry& getRegistry() { return m_registry; }

    void loadModelToRegistry(const std::string& filepath);

    void addLight(const Light& light) { m_lights.push_back(light); }
    [[nodiscard]] const std::vector<Light>& getLights() const { return m_lights; }

private:
    entt::registry m_registry;
    std::vector<Light> m_lights;

};

#endif //SCENE_H
