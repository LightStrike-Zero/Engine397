//
// Created by Shaun on 28/12/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include <entt/entt.hpp>

#include "Lights/DirectionalLight.h"
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

    void setDirectionalLight(const DirectionalLight& light) { m_dirLight = light; }
    [[nodiscard]] const DirectionalLight& getDirectionalLight() const { return m_dirLight; }

private:
    entt::registry m_registry;
    std::vector<Light> m_lights;
    DirectionalLight m_dirLight = DirectionalLight(glm::vec3(0.5f, -1.0f, -0.5f), glm::vec3(0.09f, 0.09f, 0.1f),
        glm::vec3(0.79f, 0.79f, 0.85f), glm::vec3(0.39f, 0.39f, 0.45f), 0.5f);

};

#endif //SCENE_H
