#version 410 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColors; // <- zmiana location na 1
out vec3 v_vertexColors;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_PerspectiveMatrix;

void main()
{
    v_vertexColors = vertexColors;

    vec4  newPosition = u_PerspectiveMatrix * u_ViewMatrix* u_ModelMatrix * vec4(position,1.0);

    gl_Position = vec4(newPosition.x , newPosition.y, newPosition.z, newPosition.w);
}
