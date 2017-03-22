#version 150

// this is how we receive the texture
uniform sampler2DRect tex0;
in vec2 tc;

out vec4 outputColor;
uniform vec2 u_resolution;

void main()
{
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    
    //texture input & chromatic abberation
    vec4 texCol = texture(tex0, vec2(tc.x, tc.y));
    vec4 texCol2 = texture(tex0, vec2(tc.x+10, tc.y));
    vec4 abCol = vec4(texCol.r, texCol2.g, texCol2.b, 1.0);
    
    vec4 blurAvg = vec4(0);
    int blurDist = 15;
    int blurIter = 15;
    for(int i = 1; i < blurIter+1; i++){
        blurAvg += texture(tex0, vec2(tc.x+(blurDist*i), tc.y)) + texture(tex0, vec2(tc.x-(blurDist*i), tc.y)) + texture(tex0, vec2(tc.x, tc.y+(blurDist*i))) + texture(tex0, vec2(tc.x, tc.y-(blurDist*i)))+ texture(tex0, vec2(tc.x-(blurDist*i), tc.y-(blurDist*i)))+ texture(tex0, vec2(tc.x+(blurDist*i), tc.y-(blurDist*i)))+ texture(tex0, vec2(tc.x-(blurDist*i), tc.y+(blurDist*i)))+ texture(tex0, vec2(tc.x+(blurDist*i), tc.y+(blurDist*i)));
    }
    blurAvg /= (8.0*blurIter);
    
    vec4 outCol = (blurAvg*0.95)+(abCol*0.05);
    outputColor = outCol;
}
