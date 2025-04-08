-- This will be the Game Manager script
-- manage scene setup
-- asset/scene load
-- game state : win/lose
-- other stuff I can't think of

tank_assetPath = "Assets/game_tank/tank.gltf"
--terrainType = "Heightmap"
--terrainType = "Fractal"
--terrainType = "Textured_HeightMap"
terrainType = "Textured_Fractal"

-- load asset(s) ---------------------------------------------
print("Loading:" .. tank_assetPath)
scene:loadModelToRegistry(tank_assetPath)

-- load terrain ----------------------------------------------
print("Loading terrain...")

if terrainType == "Heightmap" then
    dofile("GameScripts/TerrainConfig_heightmap.lua")
elseif terrainType == "Fractal" then
    dofile("GameScripts/TerrainConfig_fractal.lua")
elseif terrainType == "Textured_HeightMap" then
    dofile("GameScripts/TerrainConfig_tex_fractal.lua")
elseif terrainType == "Textured_Fractal" then
    dofile("GameScripts/TerrainConfig_tex_heightmap.lua")
else
    error("Unknown terrain type: " .. tostring(terrainType))
end

--
print("End of GameManager.lua")