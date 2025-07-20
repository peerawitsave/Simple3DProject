#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 vColor;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform int isShadow;

void main()
{
    if (isShadow == 1) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.3); // Transparent black shadow
        return;
    }

    vec3 norm = normalize(Normal);
    vec3 light = normalize(lightDir);

    // Ambient
    vec3 ambient = 0.3 * vColor;

    // Diffuse (Lambert)
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = diff * vColor * lightColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(light + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0); // shininess
    vec3 specular = spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
