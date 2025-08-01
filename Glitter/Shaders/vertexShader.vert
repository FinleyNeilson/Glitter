#version 330 core
uniform mat4 uModel;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;

void main()
{
   gl_Position = uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   ourColor = aColor;
}
