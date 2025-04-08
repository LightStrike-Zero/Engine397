//
// Created by Shaun on 8/04/2025.
//

#ifndef STUFFTHATNEEDSTOBELOADEDINLUA_H
#define STUFFTHATNEEDSTOBELOADEDINLUA_H

inline int terrainGridRows = 1000;
inline int terrainGridCols = 1000;
inline float terrainScale = 0.1f;
inline TerrainTypeEnum chosenType = TerrainTypeEnum::TEXTURED_FRACTAL;
inline std::map<std::string, std::string> chosenParams = {
    {"iterations", "250"},
    {"initialDisplacement", "1000.0f"},
    {"displacementDecay", "0.97f"},
    {"heightScale", "20.5f"},
    {"seed", std::to_string(4)},
    {"smoothness", std::to_string(0.8f)},
    {"smoothingPasses", "20"},
    {"texturePath", R"(Assets\Terrain\Textures\sand.png)"}
};
constexpr RendererType type = RendererType::OpenGL;

inline float playerHeight = 2.0f;

inline DirectionalLight dirLight(glm::vec3(-0.5f, -0.9f, -0.5f), glm::vec3(0.3f, 0.3f, 0.3f),
                                 glm::vec3(1.0f, 0.99f, 0.99f), glm::vec3(0.09f, 0.09f, 0.09f), 0.05f);

inline std::string playerTankPath = R"(Assets\game_tank\tank.gltf)";
inline glm::vec3 cameraOffset = {-0.f, -5.f, -10.f};

#endif //STUFFTHATNEEDSTOBELOADEDINLUA_H
