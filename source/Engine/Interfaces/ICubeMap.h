//
// Created by Shaun Matthews on 12/04/2025.
//

#ifndef ICUBEMAP_H
#define ICUBEMAP_H
#include <cstdint>

// ICubeMap: The abstract interface for cubemaps.
class ICubeMap {
public:
  virtual ~ICubeMap() = default;

  // Bind the cubemap to a given texture unit.
  virtual void bind(unsigned int slot) const = 0;

  // Unbind any cubemap.
  virtual void unbind() const = 0;

  // Return the width (all faces must have the same dimensions).
  [[nodiscard]] virtual uint32_t getWidth() const = 0;

  // Return the height.
  [[nodiscard]] virtual uint32_t getHeight() const = 0;

  // Return the underlying GPU texture handle.
  [[nodiscard]] virtual uint32_t getID() const = 0;
};


#endif //ICUBEMAP_H
