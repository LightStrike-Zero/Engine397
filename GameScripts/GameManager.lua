-- This will be the Game Manager script
-- manage scene setup
-- asset/scene load
-- game state : win/lose
-- other stuff I can't think of

backpack_assetPath = "Assets/survival_guitar_backpack_scaled/scene.gltf"
sponza_assetPath = "Assets/main1_sponza/NewSponza_Main_glTF_003.gltf"
tank_assetPath = "Assets/game_tank/tank.gltf"

print("Loading assets...")
scene:loadModelToRegistry(tank_assetPath)
print("Loading:" .. tank_assetPath)
print("Finished loading assets.")
--
print("End of GameManager.lua")