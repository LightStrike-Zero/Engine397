-- This will be the Game Manager script
-- manage scene setup
-- asset/scene load
-- game state : win/lose
-- other stuff I can't think of

tank_assetPath = "Assets/game_tank/tank.gltf"
terrainType = "Heightmap"
--terrainType = "Fractal"


-- load asset(s) ---------------------------------------------
print("Loading:" .. tank_assetPath)
scene:loadModelToRegistry(tank_assetPath)

-- load terrain ----------------------------------------------
print("Loading terrain...")

if terrainType == "Fractal" then
    dofile("GameScripts/TerrainConfig_fractal.lua")
elseif terrainType == "Heightmap" then
    dofile("GameScripts/TerrainConfig_heightmap.lua")
else
    error("Unknown terrain type: " .. tostring(terrainType))
end

--
print("End of GameManager.lua")