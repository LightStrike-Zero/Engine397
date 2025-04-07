// RendererFactory.h
#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#include "Interfaces/IRenderer.h"
#include "OpenGL/OpenGLRenderer.h"

enum class RendererType
{
    OpenGL,
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
        default:
            return nullptr;
        }
    }
};

#endif // RENDERERFACTORY_H