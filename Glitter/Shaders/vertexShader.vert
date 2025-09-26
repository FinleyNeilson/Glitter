#version 330 core
// This is to do with how we load the vertices
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 transform;

out vec3 ourVertexColor;
out vec2 TexCoord;

void main()
{
  gl_Position = transform * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourVertexColor = aColor;
  TexCoord = aTexCoord;
}
