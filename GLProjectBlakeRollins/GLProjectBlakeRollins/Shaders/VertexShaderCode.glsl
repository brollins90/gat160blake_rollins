#version 430

in layout(location=0) vec3 v_position;
in layout(location=1) vec3 v_color;
in layout(location=2) mat4 v_mvp;

out vec3 frag_color;

void main()
{
    vec4 v = vec4(v_position, 1.0f);
    gl_Position = v_mvp * v;
    frag_color = v_color;
}