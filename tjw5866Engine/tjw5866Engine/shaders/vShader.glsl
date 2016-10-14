#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;
layout (location = 3) uniform mat4 transform;
layout (location = 4) uniform mat4 camera;
layout (location = 5) uniform mat4 normalMat;
layout (location = 6) uniform vec3 cameraPos; // Calculating from matrix for every vertex is super expensive, so just pass it in

out vec3 camPos;
out vec3 fragPos;
out vec2 fragUV;
varying vec3 normalInterp;

void main(void)
{
  gl_Position = camera * transform * vec4(position, 1);
  fragPos = gl_Position.xyz;
  fragUV = uv;
  normalInterp = vec3(normalMat * vec4(norm, 0.0));
  camPos = cameraPos;
}