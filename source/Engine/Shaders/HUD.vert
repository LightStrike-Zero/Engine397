#version 330 core

uniform vec2 u_Position;     // Bottom-left corner in pixels
uniform vec2 u_Size;         // Width/height in pixels
uniform vec2 u_ScreenSize;   // Viewport dimensions in pixels

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    // Convert to Normalized Device Coordinates ([-1, 1])
    vec2 scaledPos = (aPos * u_Size + u_Position) / u_ScreenSize * 2.0 - 1.0;
    scaledPos.y = -scaledPos.y; // Invert Y axis if your screen origin is top-left

    gl_Position = vec4(scaledPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
