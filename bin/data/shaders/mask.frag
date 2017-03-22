#version 150

// this is how we receive the texture

uniform sampler2DRect blurred; //blurred
uniform sampler2DRect mask; //nosie
uniform sampler2DRect video; //no blur

in vec2 texCoordVarying;

out vec4 outputColor;
uniform vec2 u_resolution;

void main()
{
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    vec4 b  = texture(blurred, texCoordVarying.xy);
    vec4 m  = texture(mask, texCoordVarying.xy);
    vec4 v  = texture(video, texCoordVarying.xy);

    vec4 outCol = mix(v, b, m);
    outputColor = outCol;
}
