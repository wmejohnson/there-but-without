#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

void main()
{
    // send the vertices to the fragment shader
    gl_Position = modelViewProjectionMatrix * position;
}
