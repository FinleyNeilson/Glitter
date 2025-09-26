#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 ourVertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 ourColor;
                                         
void main()
{
  // Sample the texture
  // vec4 texColor = texture(texture2, TexCoord);
  vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);

  // Modulate it by ourColor (convert to vec4 with alpha 1.0)
  FragColor = texColor * ourColor * vec4(ourVertexColor, 1.0);
}
