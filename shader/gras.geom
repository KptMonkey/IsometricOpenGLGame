#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices=8) out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform float wind;

out vec2 texPos;

void main()
{        
        mat4 u = projection * view * model;
	gl_Position= u *(gl_in[0].gl_Position + vec4(-0.5, 0.0,0.0, 0.0));
	texPos = vec2(0.0,1.0);
	EmitVertex();
	gl_Position=u*( gl_in[0].gl_Position + vec4(-0.5+sin(wind)/3.0, 0.0, 1.5, 0.0));
	texPos = vec2(0.0,0.0);
        EmitVertex();
	gl_Position=u*(gl_in[0].gl_Position + vec4(0.5, 0.0, 0.0, 0.0));
	texPos = vec2(1.0,1.0);
	EmitVertex();
	gl_Position=u*(gl_in[0].gl_Position +vec4(0.5+sin(wind)/3.0, 0.0, 1.5, 0.0));
	texPos = vec2(1.0,0.0);
	EmitVertex();

	gl_Position= u *(gl_in[0].gl_Position + vec4(0.0, -0.5,0.0, 0.0));
	texPos = vec2(0.0,1.0);
	EmitVertex();
	gl_Position=u*( gl_in[0].gl_Position + vec4(sin(wind)/3.0, -0.5, 1.5, 0.0));
	texPos = vec2(0.0,0.0);
        EmitVertex();
	gl_Position=u*(gl_in[0].gl_Position + vec4(0.0, 0.5, 0.0, 0.0));
	texPos = vec2(1.0,1.0);
	EmitVertex();
	gl_Position=u*(gl_in[0].gl_Position +vec4(sin(wind)/3.0, 0.5, 1.5, 0.0));
	texPos = vec2(1.0,0.0);
	EmitVertex();

	EndPrimitive();

}
