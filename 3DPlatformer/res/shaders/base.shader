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
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 normal;
in vec3 fragPos;
in vec2 uv;

out vec4 FragColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
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

		// Diffuse calculation
		vec3 lightDir = normalize(light.position - fragPos);
		float lightDist = length(light.position - fragPos);

		float diff = max(dot(normal, lightDir), 0.0);
		diff = diff * (light.intensity / (1.0 + (0.25 * lightDist * lightDist)));
		
		vec3 diffuse = diff * u_Material.diffuse * light.color;

		// Specular calculation
		vec3 viewDir = normalize(u_ViewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
		vec3 specular = u_Material.specular * spec * light.color;

		result_color += (diffuse + specular) * vec3(u_Color);
	}

	return result_color + (u_Material.ambient * vec3(u_Color));
}

void main()
{
	vec3 result_color = calculateLight(u_Light, u_Light_Count);

	FragColor = vec4(result_color, u_Color.w) * texture(u_Texture, uv);
};