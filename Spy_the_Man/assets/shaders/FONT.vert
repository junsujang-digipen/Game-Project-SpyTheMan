#version 450 core
layout (location = 0) in vec4 vVertexPosition;
out vec2 vTexCoords;

uniform mat3 matrix;
uniform mat4 projection;

void main()
{
   gl_Position = projection * vec4(vec2(matrix * vec3(vVertexPosition.x,vVertexPosition.y,1.f)),0.0,1.0);
   vTexCoords = vec2(vVertexPosition.z,vVertexPosition.w);
} 