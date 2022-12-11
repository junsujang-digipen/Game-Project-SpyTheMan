#version 450 core

layout (location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform int Type;
uniform int Ttime;
uniform bool Closet;

void main () {
  fFragClr = texture(uTex2d, vTexCoord);
  if (Type == 1) {
    if (fFragClr.w != 0.0) {
      fFragClr.x = 0.0;
      fFragClr.y = 0.0;
      fFragClr.z = 0.0;
    }
  }
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
  if (fFragClr.w != 0.0) {
    fFragClr.w -= Ttime * 0.01;
    if (fFragClr.w <= 0.3) {
      fFragClr.w = 0.3;
    }
  }
  if (Closet == true) {
    fFragClr = vec4(0.0);
  }
}