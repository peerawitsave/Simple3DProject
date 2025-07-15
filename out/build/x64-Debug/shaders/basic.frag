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
    vec3 norm = normalize(Normal);
    vec3 light = normalize(lightDir);

    // Ambient
    vec3 ambient = 0.3 * vColor;

    // Diffuse (Lambert)
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = diff * vColor;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(light + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0); // shininess
    vec3 specular = spec * vec3(1.0); // white highlight

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

    if (isShadow == 1) {
    FragColor = vec4(0.0, 0.0, 0.0, 0.5); // semi-transparent black
    return;
}
}
