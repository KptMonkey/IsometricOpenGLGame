#version 330 core
out vec4 color;
uniform sampler2D gras;
in vec2 texPos;
void main()
{

  vec4 colorTex = texture(gras,texPos);
  if( colorTex.a < 0.1)
	discard;
  color = colorTex;
}
