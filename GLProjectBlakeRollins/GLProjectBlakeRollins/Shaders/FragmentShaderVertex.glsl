#version 430

in vec4 vertexColorWorld;
out vec4 out_color;

void main()
{
    out_color = vertexColorWorld;
}