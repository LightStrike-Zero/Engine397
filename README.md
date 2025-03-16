Changelist version 0.1.3
========================

### Changelist v0.1.3
**15/03/2025** SM
- Moved all rendering related code into the renderer
- Camera moved into renderer
- FrameBuffer moved into renderer
- Shader moved into renderer
- Created IRenderer interface
- Created OpenGLRenderer class to implement IRenderer
- Created IShader interface
- Created OpenGLShader class to implement IShader
- Modified Renderer to accept Scene object only

### Changelist v0.1.2
**15/03/2025** SM
- Created an IDataBuffer interface to abstract the OpenGL buffer object
- Created an OpenGLDataBuffer class to implement the IDataBuffer interface
- Created the OpenGLShadowMapBuffer class to handle shadow map
- Created an OpenGLQuadBuffer to represent a screen quad for the viewport rendering

### Changelist v0.1.1
**10/03/2025** SM
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
