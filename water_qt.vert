
#version 120

varying vec2 texCoord;
varying vec2 movtexCoord;
varying vec2 multexCoord;
varying vec2 waveTexCoord;
varying vec2 skyTexCoord;

varying vec3 worldPosition;
varying vec3 worldNormal;
varying vec3 worldTangent;

varying vec3 position;

uniform mat4 osg_ViewMatrixInverse;
uniform float osg_FrameTime;

const float texCoordScale = 1.0;
const float waveheight = 0.1;

void main(void)
{

    float offsetx =  sin(osg_FrameTime) * 0.1 ;
    float offsety =  cos(osg_FrameTime) * 0.1;
    float vertYpos = sin(osg_FrameTime) * 0.1;

    texCoord    = gl_MultiTexCoord0.xy * texCoordScale;
    movtexCoord = gl_MultiTexCoord0.xy * texCoordScale;
    multexCoord = gl_MultiTexCoord0.xy * (texCoordScale*0.5);
    waveTexCoord= gl_MultiTexCoord0.xy * (texCoordScale * 6.0);
    skyTexCoord = gl_MultiTexCoord0.xy * (texCoordScale * 0.2);

    movtexCoord = vec2(texCoord.x+offsetx,texCoord.y+offsety);
    multexCoord = vec2(texCoord.x-offsetx,texCoord.y+offsety);
    skyTexCoord = vec2(texCoord.x-(offsetx/2.0),texCoord.y-(offsety/2.0));

    // Transform position, normal, and tangent to world coords
    mat4 modelMatrix = osg_ViewMatrixInverse * gl_ModelViewMatrix;
    mat3 modelMatrix3x3 = mat3(modelMatrix);
    worldNormal   = normalize( modelMatrix3x3 * gl_Normal);
    worldTangent   = normalize(modelMatrix3x3 * gl_MultiTexCoord1.xyz);
    worldPosition = (modelMatrix * gl_Vertex).xyz;

    // 计算点的位置
  
    position = gl_Vertex.xyz;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
