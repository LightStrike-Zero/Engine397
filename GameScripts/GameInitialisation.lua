-- This will be the Game Manager script
-- manage scene setup
-- asset/scene load
-- game state : win/lose
-- other stuff I can't think of



-- load asset(s) ---------------------------------------------
-- print("Loading:" .. tank_assetPath)

-- Load skybox textures
skybox = {
    "Assets/skybox/right.jpg",
    "Assets/skybox/left.jpg",
    "Assets/skybox/top.jpg",
    "Assets/skybox/bottom.jpg",
    "Assets/skybox/front.jpg",
    "Assets/skybox/back.jpg"
}


tank_assetPath = "Assets/game_tank/tank.gltf"
-- scene:loadModelToRegistry(tank_assetPath)
--scene:loadModelToRegistry(jeep_assetPath)
--scene:loadModelToRegistry(jeep_assetPath)
--scene:loadModelToRegistry(jeep_assetPath)



-- Toggling the type of terrain you want
-- --------------------------------------
--terrainType = "Heightmap"
--terrainType = "Fractal"
terrainType = "TexturedHeightMap"
--  terrainType = "TexturedFractal"
-- --------------------------------------
-- when no texture is chosen in Lua (all commented) game will crash NEED TO FIX


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


-- load images ----------------------------------------------
print("Loading images...")
img_splashScreen_path = "Assets/images/exit_pic.png"

-- load text files ----------------------------------------------
print("Loading help manual file...")
text_helpManual_path = "Assets/text_files/text_manual.txt"


--
print("End of GameInitialisation.lua")