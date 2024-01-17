#shader VERT
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec4 boneIDs_1;
layout (location = 3) in vec4 boneIDs_2;
layout (location = 4) in vec4 boneWeights_1;
layout (location = 5) in vec4 boneWeights_2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vColor;

void main()
{
	boneIDs_1; boneIDs_2; boneWeights_1; boneWeights_2;
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