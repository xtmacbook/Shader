
#version 120

varying vec2 texCoord; 
uniform mat4 osg_ViewMatrixInverse;

void main(void)
{
    texCoord    = gl_MultiTexCoord0.xy; 
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
