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
    type = "Fractal",

    rows = 1000,
    cols = 1000,
    spacing = 0.1,

    parameters =
    {
        iterations = 500,
        initialDisplacement = 15.0,
        displacementDecay = 0.5,
        heightScale = 2.0,
        seed = 42,
        smoothness = 0.8,
        smoothingPasses = 3
    }
}

