#Shader:Vertex
#version 400 core
layout(location = 0) in uint object_index;
layout(location = 1) in vec3 local_position;
layout(location = 2) in vec3 world_position;
layout(location = 3) in vec2 uv;
layout(location = 4) in vec3 color;
layout(location = 5) in float texture_sampler_index;

out vec3 VertexColor;
flat out float TextureSamplerID;
out vec2 FragUV;
uniform mat4 orthographicProjection;
void main()
{
	gl_Position = orthographicProjection*vec4(world_position,1);
	VertexColor = color;
	TextureSamplerID = texture_sampler_index;
	FragUV = uv;

}

#Shader:Fragment
#version 400 core

layout(location = 0) out vec4 FragColor;

in vec3 VertexColor;

flat in float TextureSamplerID;
in vec2 FragUV;

uniform sampler2D u_Textures[10];
void main()
{
	int index_to_sample = int(TextureSamplerID);
	float font_sampled = texture(u_Textures[index_to_sample],FragUV).r;
	FragColor = vec4(vec3(1),font_sampled);
}
