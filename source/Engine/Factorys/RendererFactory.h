// RendererFactory.h
#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#include "Interfaces/IRenderer.h"
// list all the renderers here  
#include "OpenGL/OpenGLRenderer.h"

enum class RendererType
{
    OpenGL,
    // Add other renderer types here (e.g., Metal, Vulkan, DirectX)
};

class RendererFactory
{
public:
    static IRenderer* CreateRenderer(RendererType type)
    {
        switch (type)
        {
        case RendererType::OpenGL:
            return new OpenGLRenderer();
            // Add cases for other renderer types here
        default:
            return nullptr;
        }
    }
};

#endif // RENDERERFACTORY_H