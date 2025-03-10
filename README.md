Changelist version 0.1.1
========================

### Changelist v0.1.1
**10/03/2025**
- Abstracted some of the OpenGL API out.
- Created IMeshBuffer as an interface to a graphics buffer
  OpenGLMeshBuffer now inherits from IMeshBuffer so anything that takes in the IMeshBuffer object is compatible with various APIs
- Changed AssimpImporter to now create a IMeshBuffer object
- Replaced MeshComponent with RenderableComponent to better reflect what the component is actually responsible for.
- **Updated gitignore to ignore the Asset and Dependencies directories**

### Changelist v0.1.0
- **PATCH**: Missing Assimp DLL in output directory, cmake configured to copy dll on build
- **PATCH**: Shader source filepath hardcoded, changed to relative path
- **PATCH**: Default texture path hardcoded, changed to relative path
- 
- **BUG**: Font filepath points to hard coded value, removed font path temporarily
