-- This will be the Heightmap Terrain Config script
-- choose terrain config
-- choose type of terrain
-- pass setting to terrain system

-- this is a table/dictionary structure
-- nested with parameters being another table inside
-- access by:
-- terrainConfig["rows"] would give 1000
-- terrainConfig["parameters"]["heightscale"] would give 3.5

terrainConfig =
{
    type = "Heightmap",

    rows = 1000,
    cols = 1000,
    spacing = 0.2,

    parameters =
    {
        file = "Assets/Terrain/Textures/Terrain003_2K.png",
        heightScale = 12.0
    }
}