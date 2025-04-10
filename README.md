Changelist version 0.1.4
========================
### Changelist v0.1.7
**10/04/2025** BS
- Help manual pop up is operational
- Reads its content from a text file
- Utility FileHandler created

### Changelist v0.1.6
**10/04/2025** BS
- Splash screen appears when X is pressed
- It can be toggled with X
- Clicking on the image exits the game 

### Changelist v0.1.5
**10/04/2025** BS
- Terrain generation dictated from Lua files
- LuaManager handles terrain parameters and calls on teh TerrainFactory to produce terrain

### Changelist v0.1.4
**27/03/2025** BS
- Created ScriptManager base class
- Created LuaManager that inherits from ScriptManager
- Code used in main uses ScriptManager so minimal changes are required if scripting language is changed in the future.
- Two methods are interfaced so far:
  - Run script
  - Load assets method from scene class is exposed to Lua
- Lua handles the asset loading
  - Any new asset paths should be added to the lua GameManager.lua script under GameScripts directory

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
