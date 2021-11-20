#version 410

uniform float opacity;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec2 inPosition;

out vec4 fragColor;

void main() {
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 0, 1);
  fragColor = vec4(1, 1, 1, -0.5 * cos(opacity) + 0.5);
}
