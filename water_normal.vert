 
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
    
    mat4 modelMatrix = osg_ViewMatrixInverse * gl_ModelViewMatrix;
    //mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    mat3 modelMatrix3x3 = mat3(modelMatrix);
    
    vec3 worldNormal  = normalize( modelMatrix3x3 * gl_Normal);
    vec3 worldTangent  = normalize(modelMatrix3x3 * gl_MultiTexCoord1.xyz);
    worldTangent = normalize(worldTangent - dot(worldTangent, worldNormal) * worldNormal);
    vec3 worldBTangent = cross(worldNormal, worldTangent);
    
    vec4 worldPos = modelMatrix*gl_Vertex;

    mat3 TBN = transpose(mat3(worldTangent, worldBTangent, worldNormal)); 

    tangentLightPos = TBN * gl_LightSource[0].position.xyz;
    tangentViewPos  = TBN * viewPos;
    tangentFragPos  = TBN * worldPos.xyz;

    vec2 t1 = vec2(osg_FrameTime*0.2, osg_FrameTime*0.2);
    vec2 t2 = vec2(osg_FrameTime*0.5, osg_FrameTime*0.5);
    flowCoords = gl_MultiTexCoord0.xy + t1/10.0;
    rippleCoords = gl_MultiTexCoord0.xy + t1;
        
    gl_Position = ftransform();
    projCoords = gl_Position;
}