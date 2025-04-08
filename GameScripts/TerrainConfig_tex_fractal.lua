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
    type = "TexturedFractal",
    rows = 1000,
    cols = 1000,
    spacing = 0.1,

    parameters =
    {
        iterations = 250,
        initialDisplacement = 1000.0,
        displacementDecay = 0.97,
        heightScale = 20.5,
        seed = 3,
        smoothness = 0.5,
        smoothingPasses = 20
    },

    texture =
    {
        path = "Assets/Terrain/Textures/sand.png",
        repeatX = 10,
        repeatY = 10
    }
}
