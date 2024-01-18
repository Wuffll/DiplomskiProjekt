#shader VERT
#version 450 core

#define MAX_NUM_OF_BONES_PER_VERTEX 8

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in uvec4 boneIDs_1;
layout (location = 3) in uvec4 boneIDs_2;
layout (location = 4) in vec4 boneWeights_1;
layout (location = 5) in vec4 boneWeights_2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vColor;
flat out uvec4 vBoneIDs_1;
flat out uvec4 vBoneIDs_2;
out vec4 vBoneWeights_1;
out vec4 vBoneWeights_2;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vColor = color;
	vBoneIDs_1 = boneIDs_1;
	vBoneIDs_2 = boneIDs_2;
	vBoneWeights_1 = boneWeights_1;
	vBoneWeights_2 = boneWeights_2;
}

#shader FRAG
#version 450 core

#define MAX_NUM_OF_BONES_PER_VERTEX 8

uniform int uDisplayBoneIndex;

in vec3 vColor;
flat in uvec4 vBoneIDs_1;
flat in uvec4 vBoneIDs_2;
in vec4 vBoneWeights_1;
in vec4 vBoneWeights_2;

out vec4 FragColor;

void main()
{
	bool found = false;
	for (int j = 0; j < MAX_NUM_OF_BONES_PER_VERTEX / 2; j++)
	{
		if (vBoneIDs_1[j] == uDisplayBoneIndex)
		{
			if( vBoneWeights_1[j] >= 0.7 )
			{
				FragColor = vec4(1.0, 0.0, 0.0, 0.0) * vBoneWeights_1[j];
			} 
			else if( vBoneWeights_1[j] >= 0.4 && vBoneWeights_1[j] <= 0.6 )
			{
				FragColor = vec4(0.0, 1.0, 0.0, 0.0) * vBoneWeights_1[j];
			} 
			else if( vBoneWeights_1[j] >= 0.1 )
			{
				FragColor = vec4(1.0, 1.0, 0.0, 0.0) * vBoneWeights_1[j];
			}
			else 
			{
				FragColor = vec4(0.1, 0.1, 0.8, 1.0);
			}
			
			found = true;
			break;
		}
	}
	for (int j = 0; j < MAX_NUM_OF_BONES_PER_VERTEX / 2 && !found; j++)
	{
		int index = j + 4;
		if (vBoneIDs_2[index] == uDisplayBoneIndex)
		{
			if( vBoneWeights_2[index] >= 0.7 )
			{
				FragColor = vec4(1.0, 0.0, 0.0, 1.0) * vBoneWeights_2[index];
			} 
			else if( vBoneWeights_2[index] >= 0.4 && vBoneWeights_2[index] <= 0.6 )
			{
				FragColor = vec4(0.0, 1.0, 0.0, 1.0) * vBoneWeights_2[index];
			} 
			else if( vBoneWeights_2[index] >= 0.1 )
			{
				FragColor = vec4(1.0, 1.0, 0.0, 1.0) * vBoneWeights_2[index];
			}
			else 
			{
				FragColor = vec4(0.1, 0.1, 0.8, 1.0);
			}

			found = true;
			break;
		}
	}
	
	if(!found)
		FragColor = vec4(0.1, 0.1, 0.8, 1.0);
}