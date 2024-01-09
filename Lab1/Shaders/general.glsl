#shader VERT
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vColor;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vColor = color;
}

#shader FRAG
#version 450 core

in vec3 vColor;

out vec4 color;

void main()
{
	color = vec4(vColor, 1.0f);
}