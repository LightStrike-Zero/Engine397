// RendererFactory.h

/**
 * @file RendererFactory.h
 * @brief Factory class for creating renderer instances based on the selected rendering backend.
 * @author Probably Shaun???
 * @date unknown
 */

#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#include "Interfaces/IRenderer.h"
#include "OpenGL/OpenGLRenderer.h"

/**
 * @enum RendererType
 * @brief Enum listing available rendering backends.
 */
enum class RendererType
{
    OpenGL,                          ///< OpenGL rendering backend
};

/**
 * @class RendererFactory
 * @brief Factory class for creating renderers.
 */
class RendererFactory
{
public:
    /**
     * @brief Creates a renderer instance based on the specified type.
     * @param type The type of renderer to create.
     * @return Pointer to the created renderer. Caller is responsible for deletion.
     */
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