
uniform float osg_FrameTime;

uniform sampler2D waterDiffuse;
uniform sampler2D waterFlowMap;

varying vec2 texCoord;

const float speed = 0.5;

void main(void)
{
    vec4 diffuseTextureColor = texture2D(waterDiffuse, texCoord);
    
    //Flow Map 
    vec4 flowDir = texture2D(waterFlowMap,texCoord) * 2.0 - 1.0;
    flowDir *= speed;

    float phase0 = fract(osg_FrameTime*0.1);
    float phase1 = fract(osg_FrameTime*0.1 + 0.5);
    float lerpVal = abs((0.5 - phase0)/0.5);

    vec4 color1 = texture2D(waterDiffuse, (texCoord + flowDir.xy )* phase0);
    vec4 color2 = texture2D(waterDiffuse, (texCoord + flowDir.xy) * phase1);
    vec4 color = lerp(color1, color2, lerpVal);

    color = normalize(color);

    gl_FragColor = vec4(color.rgb,1.0);
}