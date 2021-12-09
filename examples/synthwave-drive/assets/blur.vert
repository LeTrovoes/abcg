#version 410

layout(location = 0) in vec2 inPosition;
out vec2 vUV;

void main() {
  gl_Position = vec4(inPosition*2.0-1.0,0.999,1);
  vUV = inPosition;
}
