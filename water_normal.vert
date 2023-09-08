 
#version 120

varying vec4 projCoords;

varying vec2 flowCoords;
varying vec2 rippleCoords;
 
varying vec3 tangentLightPos;
varying vec3 tangentViewPos;
varying vec3 tangentFragPos;

uniform mat4 osg_ViewMatrixInverse;
uniform float osg_FrameTime;
uniform vec3 viewPos;

vec4 sPlane = vec4(1.0, 0.0, 0.0, 0.0);
vec4 tPlane = vec4(0.0, 0.0, 1.0, 0.0);

 void main()
 {
 
 
    gl_Position = ftransform();
  
}