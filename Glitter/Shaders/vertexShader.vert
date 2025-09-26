#version 330 core
// This is glVertexAttribPointer
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;
out vec2 texCord;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  vertexColor = aColor;
  texCord = aTexCoord;
}
