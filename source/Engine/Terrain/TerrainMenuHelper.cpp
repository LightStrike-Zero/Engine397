
#include "TerrainMenuHelper.h"

#include <iostream>

//void getTerrainMenu(TerrainTypeEnum &chosenType, std::map<std::string, std::string> &chosenParams)
//{
//    int choice = 0;
//    std::cout << "Select Terrain Type:" << std::endl;
//    std::cout << "1. Heightmap" << std::endl;
//    std::cout << "2. Fractal" << std::endl;
//    std::cout << "3. Textured Heightmap" << std::endl;
//    std::cout << "4. Textured Fractal" << std::endl;
//    std::cout << "Enter your choice (1-4): ";
//    std::cin >> choice;
//        unsigned int seed = 66;
//        float smoothing = 1.0f;
//    if (choice == 2 || choice == 4)
//    {
//        std::cout << "Enter seed as positive integer to generate terrain from: ";
//        std::cin >> seed;
//        std::cout << "Enter smoothing amount, where 0.0 is no smoothing and 1.0 is max: ";
//        std::cin >> smoothing;
//
//    }
//
//    const std::map<std::string, std::string> heightmapParam = {
//        {"heightmapPath", R"(Assets/Terrain/Textures/Terrain003_2K.png)"},
//        {"heightScale", "12.0f"}
//    };
//
//    const std::map<std::string, std::string> texturedHeightmapParam = {
//        {"heightmapPath", R"(Assets\Terrain\Textures\Terrain003_2K.png)"},
//        {"heightScale", "24.0f"},
//        {"texturePath", R"(Assets\Terrain\Textures\Color.png)"}
//    };
//
//    const std::map<std::string, std::string> fractalParam = {
//        {"iterations", "250"},
//        {"initialDisplacement", "1000.0f"},
//        {"displacementDecay", "0.97f"},
//        {"heightScale", "20.5f"},
//        {"seed", std::to_string(seed)},
//        {"smoothness", std::to_string(smoothing)},
//        {"smoothingPasses", "20"},
//        {"texturePath", R"(Assets\Terrain\Textures\sand.png)"}
//    };
//
//    switch (choice)
//    {
//        case 1:
//            chosenType = TerrainTypeEnum::HEIGHTMAP;
//            chosenParams = heightmapParam;
//            break;
//        case 2:
//            chosenType = TerrainTypeEnum::FRACTAL;
//            chosenParams = fractalParam;
//            break;
//        case 3:
//            chosenType = TerrainTypeEnum::TEXTURED_HEIGHTMAP;
//            chosenParams = texturedHeightmapParam;
//            break;
//        case 4:
//            chosenType = TerrainTypeEnum::TEXTURED_FRACTAL;
//            chosenParams = fractalParam;
//            break;
//        default:
//            std::cout << "Invalid selection. Defaulting to Heightmap." << std::endl;
//            chosenType = TerrainTypeEnum::HEIGHTMAP;
//            chosenParams = heightmapParam;
//            break;
//    }
//}
