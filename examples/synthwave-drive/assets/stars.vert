#version 410
#define PI 3.1415926535897932384626433832795

uniform float opacity;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout(location = 0) in vec2 inPosition;

out vec4 fragColor;

void main() {
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 0, 1);
  fragColor = vec4(1, 1, 1, -0.5 * cos(opacity * 2.0 * PI) + 0.5);
}
