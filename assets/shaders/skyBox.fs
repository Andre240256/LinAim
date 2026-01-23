#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    vec4 texColor = texture(skybox, TexCoords);

    float exposure = 0.4;
    vec3 finalColor = texColor.rgb * exposure;

    FragColor = vec4(finalColor, 1.0);
}