#shader VERT
#version 450 core

#define MAX_NUM_OF_BONES_PER_VERTEX 8
#define MAX_BONES 100

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in uvec4 boneIDs_1;
layout (location = 3) in uvec4 boneIDs_2;
layout (location = 4) in vec4 boneWeights_1;
layout (location = 5) in vec4 boneWeights_2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 uBones[MAX_BONES];

out vec3 vColor;
out vec3 vLocalPos;
out vec3 vFragPos;
out vec3 vNormal;

flat out uvec4 vBoneIDs_1;
flat out uvec4 vBoneIDs_2;
out vec4 vBoneWeights_1;
out vec4 vBoneWeights_2;

void main()
{
	mat4 boneTransform = mat4(0.0f);
	
	
	for (int j = 0; j < MAX_NUM_OF_BONES_PER_VERTEX; j++)
	{
		if(j < 4)
		{
			boneTransform += uBones[boneIDs_1[j]] * boneWeights_1[j];
		}
		else 
		{
			boneTransform += uBones[boneIDs_2[j-4]] * boneWeights_2[j-4];
		}
	}


	vFragPos = vec3(model * vec4(position, 1.0));
	gl_Position = projection * view * model * boneTransform * vec4(position, 1.0f);
	vLocalPos = position;
	vColor = color;
	vNormal = mat3(transpose(inverse(model))) * color;
	vBoneIDs_1 = boneIDs_1;
	vBoneIDs_2 = boneIDs_2;
	vBoneWeights_1 = boneWeights_1;
	vBoneWeights_2 = boneWeights_2;
}

#shader FRAG
#version 450 core

#define MAX_NUM_OF_BONES_PER_VERTEX 8

uniform int uDisplayBoneIndex;
uniform vec3 uViewPos;
uniform vec3 uLightColor;

in vec3 vColor;
in vec3 vLocalPos;
in vec3 vFragPos;
in vec3 vNormal;

flat in uvec4 vBoneIDs_1;
flat in uvec4 vBoneIDs_2;
in vec4 vBoneWeights_1;
in vec4 vBoneWeights_2;

out vec4 FragColor;

void main()
{
	vec3 lightPos = vec3(0.0f, 2.0f, 1.0f);
	
	float ambientStrenght = 0.25;
	vec3 ambient = ambientStrenght * uLightColor;
	
	vec3 norm = normalize(vNormal);
	vec3 lightDirection = normalize(lightPos - vFragPos);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * uLightColor;
	
	float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor; 

	bool found = false;
	
	for (int j = 0; j < MAX_NUM_OF_BONES_PER_VERTEX; j++)
	{
		// Bones 0 - 3
		if( j < 4)
		{
			if (vBoneIDs_1[j] == uDisplayBoneIndex)
			{
				if( vBoneWeights_1[j] >= 0.7 )
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(1.0, 0.0, 0.0), 0.0) * vBoneWeights_1[j];
				} 
				else if( vBoneWeights_1[j] >= 0.4 && vBoneWeights_1[j] <= 0.6 )
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(0.0, 1.0, 0.0), 0.0) * vBoneWeights_1[j];
				} 
				else if( vBoneWeights_1[j] >= 0.1 )
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(1.0, 1.0, 0.0), 0.0) * vBoneWeights_1[j];
				}
				else 
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(0.1, 0.1, 0.8), 1.0);
				}
				
				found = true;
				break;
			}
		}
		// Bones 4 - 7
		else
		{
			if (vBoneIDs_2[j - 4] == uDisplayBoneIndex)
			{
				if( vBoneWeights_2[j-4] >= 0.7 )
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(1.0, 0.0, 0.0), 0.0) * vBoneWeights_2[j-4];
				} 
				else if( vBoneWeights_2[j-4] >= 0.4 && vBoneWeights_2[j-4] <= 0.6 )
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(0.0, 1.0, 0.0), 0.0) * vBoneWeights_2[j-4];
				} 
				else if( vBoneWeights_2[j-4] >= 0.1 )
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(1.0, 1.0, 0.0), 0.0) * vBoneWeights_2[j-4];
				}
				else 
				{
					FragColor = vec4((ambient + diffuse + specular) * vec3(0.1, 0.1, 0.8), 1.0);
				}

				found = true;
				break;
			}
		}
	}
	
	// In case the selected display bone is not associated with this pixel
	if(!found)
		FragColor = vec4((ambient + diffuse + specular) * vec3(0.15, 0.15, 0.75), 1.0);
}