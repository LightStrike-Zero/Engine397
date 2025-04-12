//
// Created by light on 12/04/2025.
//

#ifndef OGL_CUBEMAP_H
#define OGL_CUBEMAP_H

#include "Interfaces/ICubeMap.h"
#include <array>
#include <string>
#include <GL/glew.h> // or your preferred OpenGL header

// OpenGLCubeMap: Concrete OpenGL implementation of a cubemap.
class OpenGLCubeMap : public ICubeMap {
public:
  // Constructor takes an array of six file paths (order must match OpenGL convention).
  explicit OpenGLCubeMap(const std::array<std::string, 6>& faces);
  ~OpenGLCubeMap() override;

  // Bind and unbind the cubemap.
  void bind(unsigned int slot) const override;
  void unbind() const override;

  // Get dimensions (all faces are assumed to be square and of the same size).
  [[nodiscard]] uint32_t getWidth() const override { return m_width; }
  [[nodiscard]] uint32_t getHeight() const override { return m_height; }

  // Return the GPU texture handle.
  [[nodiscard]] uint32_t getID() const override { return m_textureID; }

private:
  GLuint m_textureID;
  uint32_t m_width, m_height;
  // Optionally: number of channels or other metadata can be stored, if needed.
};


#endif //OGL_CUBEMAP_H
