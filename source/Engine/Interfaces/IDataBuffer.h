//
// Created by Shaun on 15/03/2025.
//

/**
 * @file IDataBuffer.h
 * @brief Interface for data buffer abstraction in rendering systems.
 * @author Shaun
 * @date 2025-03-15
 */

#ifndef IDATABUFFER_H
#define IDATABUFFER_H

/**
 * @class IDataBuffer
 * @brief Interface for GPU buffer objects.
 *
 * Provides basic functionality for binding and unbinding a GPU buffer.
 * Intended to be inherited by specific buffer types like VBOs or UBOs.
 */
class IDataBuffer {
  public:

    /**
     * @brief Bind the buffer for use.
     */
    virtual void bind() = 0;

    /**
     * @brief Unbind the buffer.
     */
    virtual void unbind() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IDataBuffer() = default;
    
    private:
      unsigned int m_dataBuffer;
};

#endif //IDATABUFFER_H
