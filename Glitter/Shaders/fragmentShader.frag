#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 texCord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 uColor;
                                         
void main()
{
  vec4 texColor = mix(texture(texture1, texCord), texture(texture2, texCord), 0.5);

  FragColor = texColor;
}
