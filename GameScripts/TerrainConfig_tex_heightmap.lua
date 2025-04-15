-- This will be the Fractal Terrain Config script
-- choose terrain config
-- choose type of terrain
-- pass setting to terrain system

-- this is a table/dictionary structure
-- nested with parameters being another table inside
-- access by:
-- terrainConfig["rows"] would give 1000
-- terrainConfig["parameters"]["iterations"] would give 500

terrainConfig =
{
    type = "TexturedHeightmap",

    rows = 1000,
    cols = 1000,
    spacing = 1.0,

    parameters =
    {
        heightmapPath = "Assets/Terrain/Textures/Terrain003_2K.png",
        heightScale = 100.0
    },

    -- This is a demonstration of using either a single solid texture
    -- or a multi-texture where we supply exactly 3 textures, along with
    -- the heightmap we want to blend, and blending parameters.
    
    --texture =
    --{
    --    path = "Assets/Terrain/Textures/Mountain_03.png",
    --    repeatX = 1,
    --    repeatY = 1
    --}

    multitexture =
    {
        height = "Assets/Terrain/Textures/Terrain003_2K.png",
        tex1 = "Assets/Terrain/Textures/Mountain_01.png",
        tex2 = "Assets/Terrain/Textures/Mountain_03.png",
        tex3 = "Assets/Terrain/Textures/Mountain_02.png",
        tex4 = "Assets/Terrain/Textures/detail.png",
        blend1 = 0.01,
        blend2 = 0.7,
        repeatX = 1,
        repeatY = 1
    }
}