#version 330 core

in vec3 vColor;
out vec4 FragColor;

uniform float time;

void main()
{
    float intensity = 0.5 + 0.5 * sin(time * 2.0); // range: 0.0 to 1.0
    vec3 animatedColor = vColor * intensity;
    FragColor = vec4(animatedColor, 1.0);
}
