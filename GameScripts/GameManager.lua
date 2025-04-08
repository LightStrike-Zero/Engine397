-- This will be the Game Manager script
-- manage scene setup
-- asset/scene load
-- game state : win/lose
-- other stuff I can't think of

tank_assetPath = "Assets/game_tank/tank.gltf"

-- load asset(s) ---------------------------------------------
print("Loading:" .. tank_assetPath)
scene:loadModelToRegistry(tank_assetPath)



-- Toggling the type of terrain you want
-- --------------------------------------
--terrainType = "Heightmap"
--terrainType = "Fractal"
--terrainType = "TexturedHeightMap"
terrainType = "TexturedFractal"
-- --------------------------------------



-- load terrain ----------------------------------------------
print("Loading terrain...")

if terrainType == "Heightmap" then
    dofile("GameScripts/TerrainConfig_heightmap.lua")
elseif terrainType == "Fractal" then
    dofile("GameScripts/TerrainConfig_fractal.lua")
elseif terrainType == "TexturedHeightMap" then
    dofile("GameScripts/TerrainConfig_tex_heightmap.lua")
elseif terrainType == "TexturedFractal" then
    dofile("GameScripts/TerrainConfig_tex_fractal.lua")
else
    error("Unknown terrain type: " .. tostring(terrainType))
end

--
print("End of GameManager.lua")