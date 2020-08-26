#shader vertex
#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 u_Transform;
uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * u_Transform * vec4(vertex_pos, 1.0);
   fragPos = vec3(u_Transform * vec4(vertex_pos, 1.0));
   normal = vec3(u_Transform * vec4(vertex_normal, 0.0));
};

#shader fragment
#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform vec4 u_Color;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform float u_LightIntensity;

void main()
{
	vec3 light_Dir = normalize(u_LightPos - fragPos);
	float light_Dist = length(u_LightPos - fragPos);

	float diff = max(dot(normal, light_Dir), 0.0);
	diff = diff * (u_LightIntensity / (1.0 + (0.25 * light_Dist * light_Dist)));

	vec3 diffuse = diff * u_LightColor;
	vec3 ambient = vec3(0.3, 0.3, 0.3);

	vec3 result_color = (ambient + diffuse) * vec3(u_Color);
	color = vec4(result_color, u_Color.w);
};