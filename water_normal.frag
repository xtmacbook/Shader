
uniform sampler2D defaultTex; 
uniform sampler2D reflection; 
uniform sampler2D refraction; 
uniform sampler2D normalTex; 

varying vec4 projCoords;

varying vec2 flowCoords;
varying vec2 rippleCoords;
 
varying vec3 tangentLightPos;
varying vec3 tangentViewPos;
varying vec3 tangentFragPos;

void main()
{
 
	gl_FragColor = (1.0,0.0,0.0,1.0);


}