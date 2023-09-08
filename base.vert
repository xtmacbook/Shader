
#version 120

varying vec2 texCoord;

uniform mat4 osg_ViewMatrixInverse;

vec4 sPlane = vec4(1.0, 0.0, 0.0, 0.0);
vec4 tPlane = vec4(0.0, 0.0, 1.0, 0.0);

void main(void)
{
    
    mat4 modelMatrix = osg_ViewMatrixInverse * gl_ModelViewMatrix;
    vec4 worldPos = modelMatrix * gl_Vertex;

    texCoord.x = dot(worldPos , sPlane);
    texCoord.y = dot(worldPos , tPlane);
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
