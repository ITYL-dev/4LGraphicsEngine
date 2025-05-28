#version 460 core

uniform float cyanValue;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0, cyanValue, cyanValue, 1);
} 