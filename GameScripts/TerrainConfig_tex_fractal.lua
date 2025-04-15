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
    rows = 3000,
    cols = 3000,
    spacing = 1.0,

    parameters =
    {
        iterations = 250,
        initialDisplacement = 1000.0,
        displacementDecay = 0.97,
        heightScale = 150,
        seed = 3,
        smoothness = 0.9,
        smoothingPasses = 10
    },

    --texture =
    --{
    --    path = "Assets/Terrain/Textures/sand.png",
    --    repeatX = 20,
    --    repeatY = 20
    --}

    multitexture =
    {
        height = "Assets/Terrain/Textures/Terrain003_2K.png",
        tex1 = "Assets/Terrain/Textures/Mountain_01.png",
        tex2 = "Assets/Terrain/Textures/Mountain_03.png",
        tex3 = "Assets/Terrain/Textures/Mountain_02.png",
        blend1 = 0.01,
        blend2 = 0.7,
        repeatX = 1,
        repeatY = 1
    }
}
