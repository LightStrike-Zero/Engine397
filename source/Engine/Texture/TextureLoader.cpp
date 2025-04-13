#include "TextureLoader.h"

#include <iostream>
#include <sstream>

#include "TextureManager.h"
#include "Factorys/TextureFactory.h"
#include "OpenGL/OGL_CubeMap.h"
#include "OpenGL/OGL_Texture.h"

std::shared_ptr<ITexture> TextureLoader::loadTexture(const std::string& filePath)
{
    return TextureFactory::createTexture(filePath);
}

std::shared_ptr<ITexture> TextureLoader::loadFromGLTF(const std::string& gltfTexturePath)
{
    return TextureFactory::createTexture(gltfTexturePath);
}

std::shared_ptr<ITexture> TextureLoader::loadFromMemory(const RawImageData& imageData)
{
    return TextureFactory::createTexture(imageData);
}

std::shared_ptr<ICubeMap> TextureLoader::loadCubeMap(const std::array<std::string, 6>& faces)
{
    return TextureFactory::createTexture(faces);
}


RawImageData TextureLoader::createCompositeTexture(const std::vector<std::string>& sourceTexturePaths,
                                                   const BlendParameters& blendingParams)
{
    if (sourceTexturePaths.size() < 4) {
        throw std::runtime_error("Need at least 4 textures: heightmap and 3 source textures");
    }

    OpenGLTexture heightMapTexture;
    OpenGLTexture tex0;
    OpenGLTexture tex1;
    OpenGLTexture tex2;

    heightMapTexture.loadImageData(sourceTexturePaths[0]);
    const unsigned char* heightMapData = heightMapTexture.getData();
    uint32_t heightMapWidth = heightMapTexture.getWidth();
    uint32_t heightMapHeight = heightMapTexture.getHeight();
    uint8_t heightMapChannels = heightMapTexture.getChannels();

    tex0.loadImageData(sourceTexturePaths[1]);
    const unsigned char* tex0Data = tex0.getData();
    uint32_t tex0Width = tex0.getWidth();
    uint32_t tex0Height = tex0.getHeight();
    uint8_t tex0Channels = tex0.getChannels();
    
    tex1.loadImageData(sourceTexturePaths[2]);
    const unsigned char* tex1Data = tex1.getData();
    uint32_t tex1Width = tex1.getWidth();
    uint32_t tex1Height = tex1.getHeight();
    uint8_t tex1Channels = tex1.getChannels();

    tex2.loadImageData(sourceTexturePaths[3]);
    const unsigned char* tex2Data = tex2.getData();
    uint32_t tex2Width = tex2.getWidth();
    uint32_t tex2Height = tex2.getHeight();
    uint8_t tex2Channels = tex2.getChannels();
    

    uint32_t width = heightMapWidth;
    uint32_t height = heightMapHeight;

    if (tex0Width != width || tex0Height != height ||
        tex1Width != width || tex1Height != height ||
        tex2Width != width || tex2Height != height)
    {
        throw std::runtime_error("Source texture dimensions do not match heightmap dimensions.");
    }
    
    const int compositeChannels = 4;
    std::vector<uint8_t> compositeData(width * height * compositeChannels, 0);

    auto sampleChannel = [](const unsigned char* data, uint8_t channels, uint32_t index, int ch) -> uint8_t {
        return (channels > ch) ? data[index * channels + ch] : 0;
    };

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t index = y * width + x;
            
            // Height value from the heightmap normalized to [0,1].
            float hValue = heightMapData[index * heightMapChannels] / 255.0f;
            
            uint8_t finalR = 0, finalG = 0, finalB = 0, finalA = 255;

            if (hValue < blendingParams.threshold1) {
                finalR = sampleChannel(tex0Data, tex0Channels, index, 0);
                finalG = sampleChannel(tex0Data, tex0Channels, index, 1);
                finalB = sampleChannel(tex0Data, tex0Channels, index, 2);
                finalA = sampleChannel(tex0Data, tex0Channels, index, 3);
                if (tex0Channels < 4) finalA = 255;
            }
            else if (hValue < blendingParams.threshold2) {
                // Blend between texture 0 and texture 1.
                float blendFactor = (hValue - blendingParams.threshold1) / 
                                   (blendingParams.threshold2 - blendingParams.threshold1);
                
                blendFactor = std::max(0.0f, std::min(blendFactor, 1.0f));

                finalR = static_cast<uint8_t>(sampleChannel(tex0Data, tex0Channels, index, 0) * (1.0f - blendFactor) +
                                             sampleChannel(tex1Data, tex1Channels, index, 0) * blendFactor);
                finalG = static_cast<uint8_t>(sampleChannel(tex0Data, tex0Channels, index, 1) * (1.0f - blendFactor) +
                                             sampleChannel(tex1Data, tex1Channels, index, 1) * blendFactor);
                finalB = static_cast<uint8_t>(sampleChannel(tex0Data, tex0Channels, index, 2) * (1.0f - blendFactor) +
                                             sampleChannel(tex1Data, tex1Channels, index, 2) * blendFactor);
                
                if (tex0Channels == 4 || tex1Channels == 4) {
                    finalA = static_cast<uint8_t>(sampleChannel(tex0Data, tex0Channels, index, 3) * (1.0f - blendFactor) +
                                                sampleChannel(tex1Data, tex1Channels, index, 3) * blendFactor);
                }
            }
            else {
                // Blend between texture 1 and texture 2.
                float blendFactor = (hValue - blendingParams.threshold2) / (1.0f - blendingParams.threshold2);
                
                blendFactor = std::max(0.0f, std::min(blendFactor, 1.0f));

                finalR = static_cast<uint8_t>(sampleChannel(tex1Data, tex1Channels, index, 0) * (1.0f - blendFactor) +
                                             sampleChannel(tex2Data, tex2Channels, index, 0) * blendFactor);
                finalG = static_cast<uint8_t>(sampleChannel(tex1Data, tex1Channels, index, 1) * (1.0f - blendFactor) +
                                             sampleChannel(tex2Data, tex2Channels, index, 1) * blendFactor);
                finalB = static_cast<uint8_t>(sampleChannel(tex1Data, tex1Channels, index, 2) * (1.0f - blendFactor) +
                                             sampleChannel(tex2Data, tex2Channels, index, 2) * blendFactor);
                
                if (tex1Channels == 4 || tex2Channels == 4) {
                    finalA = static_cast<uint8_t>(sampleChannel(tex1Data, tex1Channels, index, 3) * (1.0f - blendFactor) +
                                                sampleChannel(tex2Data, tex2Channels, index, 3) * blendFactor);
                }
            }

            // Write the final composite RGBA values.
            int compIndex = index * compositeChannels;
            compositeData[compIndex + 0] = finalR;
            compositeData[compIndex + 1] = finalG;
            compositeData[compIndex + 2] = finalB;
            compositeData[compIndex + 3] = finalA;
        }
    }
    

    RawImageData result;
    result.width = width;
    result.height = height;
    result.channels = compositeChannels;  
    result.isCompressed = false;
    size_t totalBytes = static_cast<size_t>(width) * height * compositeChannels;
    result.data = new unsigned char[totalBytes];
    std::memcpy(result.data, compositeData.data(), totalBytes);

    return result; 
}