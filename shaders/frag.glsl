#version 410 core
in vec3 v_vertexColors;
out vec4 color;

uniform mat4 u_ModelMatrix;

void main()
{
    
    vec4  newColors = u_ModelMatrix * vec4(v_vertexColors,1.0);
    
    color = vec4(newColors);
}
