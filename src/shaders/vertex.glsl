#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in mat4 instanceMatrix;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
//should also pass inv transform of model when not Identity matrix
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = instanceMatrix * vec4(aPos, 1.0);
    FragPos = vec3(model * worldPos);
    Normal = mat3(model) * mat3(instanceMatrix) * aNormal; // should use inv trans
    gl_Position = projection * view * vec4(FragPos, 1.0);
}