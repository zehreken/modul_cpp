#version 330 core
in vec3 vLocalPos;
in vec2 vUV;

out vec4 FragColor;
uniform float time;

float circle(vec2 pos, float radius)
{
    float r = length(pos) - radius;
    return r;
}

void main()
{
    float radius = 0.25f + time;

    vec3 pos = vLocalPos;
    float c = circle(pos.xy, radius);
    float signed = -sign(c);
    vec3 col = vec3(signed, signed, signed);
    FragColor = vec4(col, 1.0f);
}