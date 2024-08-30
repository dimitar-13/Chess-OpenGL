#Shader:Vertex
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

out vec3 VertexColor;
uniform mat4 orthographicProjection;
void main()
{
	gl_Position = orthographicProjection*vec4(position,0,1);
	VertexColor = color;
}

#Shader:Fragment
#version 330 core

out vec4 FragColor;
in vec3 VertexColor;

void main()
{
	FragColor = vec4(VertexColor,1);
}
