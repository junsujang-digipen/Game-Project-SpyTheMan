#version 450 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform int Type;

void main () {
  fFragClr = texture(uTex2d, vTexCoord);
  if (Type == 2) {
    if (fFragClr.w != 0.0) {
      fFragClr.x = 1.0;
      fFragClr.y = 0.0;
      fFragClr.z = 0.0;
    }
  }
  if (Type == 3) {
    if (fFragClr.w != 0.0) {
      fFragClr.x = 0.0;
      fFragClr.y = 0.0;
      fFragClr.z = 1.0;
    }
  }
}