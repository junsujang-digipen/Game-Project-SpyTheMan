#version 330
//Seongwon Jang
precision mediump float;

out vec4 FragColor;
uniform float u_time;
uniform vec2 u_resolution;
uniform vec4 uColor;
void main()
{

    vec2 coord = gl_FragCoord.xy / u_resolution;
    vec3 color = vec3(uColor.r,uColor.g,uColor.b);
    

    float alpha = abs(sin((-coord.y * 5) + u_time));
    color -= alpha /5;

    FragColor = vec4(color,uColor.a);
}
