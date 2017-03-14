#version 330 core
out vec4 color;
in vec3 pos;
in vec2 TexPos;
uniform sampler2D terrain;
uniform vec4 uColor;
void main()
{
  
  color = vec4(vec3(texture(terrain,TexPos)),1.0);
  if ( uColor.y > 0 )
  {
	color = uColor;
  }

}
