//#version 330 core
//
//in vec2 TexCoord;
//out vec4 FragColor;
//
//uniform sampler2D u_Texture;
//
//void main()
//{
//    FragColor = texture(u_Texture, TexCoord);
//}


// TEST

#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0, 0.0, 1.0, 1.0); // bright pink
}