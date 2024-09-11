#Shader:Vertex
#version 330 core
layout(location = 0) in vec3 local_position;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 color;
layout(location = 4) in float texture_sampler_index;

out vec3 in_FragColor;
out vec3 in_FragLocalPos;
uniform mat4 orthographicProjection;
void main()
{
	gl_Position = orthographicProjection*vec4(world_position,1);
	in_FragColor = color;
	in_FragLocalPos = local_position;
}

#Shader:Fragment
#version 330 core

out vec4 out_FragColor;
in vec3 in_FragColor;
in vec3 in_FragLocalPos;
void main()
{
    float radius = .9f;
    float thickness = .2f;
    float circle_AA = 0.09;

    float uv_len = length(in_FragLocalPos);
    
    float opacity_factor = min(
     1. -smoothstep(.0,circle_AA,uv_len - radius),
    smoothstep(0.0,circle_AA,uv_len - radius + thickness));

	out_FragColor = vec4(in_FragColor*opacity_factor,opacity_factor);
}
