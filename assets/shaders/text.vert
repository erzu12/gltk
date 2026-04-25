#version 460 core
layout (location = 0) in vec2 aPos;

uniform mat3 transform;

out vec2 texCoords;

void main()
{
    texCoords = aPos;
    gl_Position = vec4((transform * vec3(aPos, 1.0f)).xy, 0.0, 1.0);
}

