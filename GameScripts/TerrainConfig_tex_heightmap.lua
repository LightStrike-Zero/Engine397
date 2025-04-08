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
    spacing = 0.1,

    parameters =
    {
        file = "Assets/Terrain/Textures/Terrain003_2K.png",
        heightScale = 30.0
    },

    texture =
    {
        path = "Assets/Terrain/Textures/Color.png",
        repeatX = 20,
        repeatY = 20
    }
}