//
// Created by Shaun on 11/04/2025.
//

#ifndef ITEXTURE_H
#define ITEXTURE_H
#include <cstdint>

class ITexture {
public:
    virtual ~ITexture() = default;
    
    // Bind the texture to a specific slot.
    virtual void bind(unsigned int slot) const = 0;
    
    // Unbind the texture.
    virtual void unbind() const = 0;
    
    [[nodiscard]] virtual uint32_t getWidth() const = 0;
    [[nodiscard]] virtual uint32_t getHeight() const = 0;
    [[nodiscard]] virtual uint8_t getChannels() const = 0;
};

#endif //ITEXTURE_H
