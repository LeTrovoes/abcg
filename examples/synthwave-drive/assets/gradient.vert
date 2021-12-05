#version 410

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 0, 1);
  fragColor = inColor;
}
