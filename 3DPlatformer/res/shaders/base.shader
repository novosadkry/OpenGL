#shader vertex
#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec3 normal;
out vec3 fragPos;
out vec2 uv;

uniform mat4 u_Transform;
uniform mat4 u_MVP;
uniform mat3 u_Normal;

void main()
{
   gl_Position = u_MVP * u_Transform * vec4(vertex_pos, 1.0);
   fragPos = vec3(u_Transform * vec4(vertex_pos, 1.0));
   normal = u_Normal * vertex_normal;
   uv = vertex_uv;
};

#shader fragment
#version 330 core

struct Light
{
	vec3 position;
	vec3 color;
	float intensity;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	vec3 ambient;
	vec4 color;
};

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

out vec4 FragColor;

uniform Material u_Material;

uniform Light[16] u_Light;
uniform int u_Light_Count;

uniform vec3 u_ViewPos;

vec3 calculateLight(Light[16] lights, int light_count)
{
	vec3 result_color;

	for (int i = 0; i < light_count; i++)
	{
		Light light = lights[i];

		vec3 _normal = normalize(normal);

		// Diffuse calculation
		vec3 lightDir = normalize(light.position - fragPos);
		float lightDist = length(light.position - fragPos);

		float diff = max(dot(_normal, lightDir), 0.0);
		diff = diff * (light.intensity / (1.0 + (0.25 * lightDist * lightDist)));
		
		vec3 diffuse = diff * texture(u_Material.diffuse, uv).rgb * light.color;

		// Specular calculation
		vec3 viewDir = normalize(u_ViewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, _normal);
		float viewDist = length(u_ViewPos - fragPos);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
		vec3 specular = spec * texture(u_Material.specular, uv).rgb * light.color * light.intensity * (1 / viewDist);

		result_color += (diffuse + specular) * vec3(u_Material.color);
	}

	return result_color + (u_Material.ambient * vec3(u_Material.color));
}

void main()
{
	vec3 result_color = calculateLight(u_Light, u_Light_Count);

	FragColor = vec4(result_color, u_Material.color.w) * texture(u_Material.diffuse, uv);
};