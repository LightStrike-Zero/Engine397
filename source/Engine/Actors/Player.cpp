// //
// // Created by hugo on 8/04/2025.
// //
//
// #include "Player.h"
//
// #include "Components/MaterialComponent.h"
// #include "Components/PlayerControllerComponent.h"
// #include "Components/TransformComponent.h"
// #include "Importers/ModelLoader.h"
//
// void Player::loadPlayerModelToRegistry(Scene scene,const std::string& filepath) {
//     // std::cout << "Registry address inside Scene: " << &m_registry << std::endl;
//     ModelLoader& loader = ModelLoader::getInstance();
//
//     // Load the raw model data
//     LoadedModel modelData = loader.loadModel(filepath);
//
//     // Process raw meshes into MeshComponents
//     AssimpImporter importer;
//     for (const auto& rawMesh : modelData.meshes) {
//         entt::entity entity = m_registry.create();
//
//         RenderableComponent meshComponent(rawMesh);
//         importer.setupMesh(meshComponent);
//         m_registry.emplace<RenderableComponent>(entity, meshComponent);
//
//         TransformComponent transformComponent;
//         transformComponent.setFromModelMatrix(rawMesh.transform);
//         m_registry.emplace<TransformComponent>(entity, transformComponent);
//
//         // for now im hard-coding the lighting shader into this, but it needs a way of being dynamically set
//         MaterialComponent materialComponent(rawMesh.material, "lightingShader");
//         m_registry.emplace<MaterialComponent>(entity, materialComponent);
//
//         PlayerControllerComponent controllerComponent{1};
//         m_registry.emplace<PlayerControllerComponent>(entity, controllerComponent);
//     }
// }
