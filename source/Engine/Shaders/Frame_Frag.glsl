#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D framebuf;

void main()
{
    FragColor = texture(framebuf, TexCoords);
//    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
