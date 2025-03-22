//
// Created by Shaun on 15/03/2025.
//

#ifndef IRENDERER_H
#define IRENDERER_H

class IRenderer
{
public:
    IRenderer() = default;
    virtual unsigned int Render(Scene& scene) = 0;
    virtual void Clear() = 0;
    virtual ~IRenderer() = default;
};

#endif //IRENDERER_H
