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

struct Light
{
	vec3 position;
	vec3 color;
	float intensity;
};

in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform vec4 u_Color;

uniform Light[16] u_Light;
uniform int u_Light_Count;

vec3 calculateLight(Light[16] lights, int light_count)
{
	vec3 result_color;
	vec3 ambient = vec3(0.3, 0.3, 0.3);

	for (int i = 0; i < light_count; i++)
	{
		Light light = lights[i];

		vec3 light_Dir = normalize(light.position - fragPos);
		float light_Dist = length(light.position - fragPos);

		float diff = max(dot(normal, light_Dir), 0.0);
		diff = diff * (light.intensity / (1.0 + (0.25 * light_Dist * light_Dist)));

		vec3 diffuse = diff * light.color;

		result_color += diffuse * vec3(u_Color);
	}

	return result_color + (ambient * vec3(u_Color));
}

void main()
{
	vec3 result_color = calculateLight(u_Light, u_Light_Count);

	color = vec4(result_color, u_Color.w);
};