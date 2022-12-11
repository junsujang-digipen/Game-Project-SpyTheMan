#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in float vVertexTexPosition;

layout (location=0) out vec2 vTexCoord;

uniform mat3 uModel_to_NDC;
uniform vec4 texel;

void main() {
  gl_Position = vec4(vec2(uModel_to_NDC * vec3(vVertexPosition, 1.f)), 0.0, 1.0);
  if (vVertexTexPosition == 0) {
    vTexCoord = vec2(texel.x, texel.w);
  }
  else if (vVertexTexPosition == 1) {
    vTexCoord = vec2(texel.z, texel.w);
  }
  else if (vVertexTexPosition == 2) {
    vTexCoord = vec2(texel.x, texel.y);
  }
  else if (vVertexTexPosition == 3) {
    vTexCoord = vec2(texel.z, texel.y);
  }
}
