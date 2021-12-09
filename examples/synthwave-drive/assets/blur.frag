#version 410

in vec4 fragColor;
in vec2 vUV;
uniform sampler2D diffuseTex;
uniform vec2 blurDirection;
out vec4 outColor;

// https://github.com/Jam3/glsl-fast-gaussian-blur
// tampered params
vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(3) * direction;
  vec2 off2 = vec2(6) * direction;
  color += texture(image, uv) * 0.4;
  color += texture(image, uv + (off1 / resolution)) * 0.3162162162;
  color += texture(image, uv - (off1 / resolution)) * 0.3162162162;
  color += texture(image, uv + (off2 / resolution)) * 0.0702702703;
  color += texture(image, uv - (off2 / resolution)) * 0.0702702703;
  return color;
}

void main() {
  outColor = blur9(diffuseTex, vUV, vec2(1920, 1080), blurDirection);
}
