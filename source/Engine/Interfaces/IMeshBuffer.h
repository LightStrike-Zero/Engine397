//
// Created by Shaun on 7/03/2025.
//

/**
 * @file IMeshBuffer.h
 * @brief Interface for mesh buffers used in rendering.
 * Defines the abstract interface for any mesh buffer.
 * @author Shaun
 * @date 2025-03-07
 */

#ifndef IMESHBUFFER_H
#define IMESHBUFFER_H

/**
 * @class IMeshBuffer
 * @brief Interface for GPU mesh buffer objects.
 */
class IMeshBuffer {
public:
    /**
     * @brief Binds the mesh buffer to the graphics pipeline.
     */
    virtual void bind() = 0;

    /**
     * @brief Draw call using the currently bound mesh buffer.
     */
    virtual void draw() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IMeshBuffer() = default;
};

#endif //IMESHBUFFER_H
