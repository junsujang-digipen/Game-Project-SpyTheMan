#version 330

#ifdef GL_ES
precision mediump float;
#endif

out vec4 FragColor;
uniform float u_time;
uniform vec2 u_resolution;
uniform vec4 uColor;
uniform int u_skip = 0;

float circleshape(vec2 position, vec2 ballPosition, float radius)
{
    return step(radius, length(position - ballPosition));
}

float noise1d(float v){
    return cos(v + cos(v * 90.1415) * 100.1415) * 0.5 + 0.5;
}



void main()
{
    vec2 PixelPosition = gl_FragCoord.xy / u_resolution;
    vec2 ballPosition =  vec2(0.5,0.5);
    float ratio = u_resolution.x/u_resolution.y;
    PixelPosition.x *= ratio;
    float circle = 1.5;  
    if(u_skip == 1)
    {
        ballPosition =  vec2(0.5 ,0.5);
        ballPosition.x  *= ratio;
        circle -= (1 - circleshape(PixelPosition,ballPosition,0.7));  
    }
    else
    {
        for(int i = 1; i < 6; ++i)
        {
        ballPosition =  vec2(noise1d(i*3 + u_time*0.0003),noise1d(i*10 + u_time * 0.0004));
        ballPosition.x  *= ratio;
        circle -= (1 - circleshape(PixelPosition,ballPosition,0.3));
        }
    }

    FragColor = vec4(uColor.r,uColor.g,uColor.b, circle - 0.8);
}
