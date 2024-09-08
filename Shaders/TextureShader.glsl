#Shader:Vertex
#version 400 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in float sampler_array_id;

out vec3 VertexColor;
flat out float TextureSamplerID;
out vec2 FragUV;
uniform mat4 orthographicProjection;

void main()
{
	gl_Position = orthographicProjection*vec4(position,1);
	VertexColor = color;
	TextureSamplerID = sampler_array_id;
	FragUV = uv;
}

#Shader:Fragment
#version 400 core

out vec4 FragColor;
in vec3 VertexColor;
flat in float TextureSamplerID;
in vec2 FragUV;

uniform sampler2D u_Textures[10];
void main()
{
	int index_to_sample = int(TextureSamplerID);
	FragColor = vec4(texture(u_Textures[index_to_sample],FragUV).rgb,1);
}
