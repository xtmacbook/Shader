
uniform sampler2D waterDiffuse;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D waveTexture;
uniform sampler2D skyTexture;
uniform sampler2D foamTexture;


varying vec2 texCoord;
varying vec2 movtexCoord;
varying vec2 multexCoord;
varying vec2 waveTexCoord;
varying vec2 skyTexCoord;

varying vec3 worldPosition;
varying vec3 worldNormal;
varying vec3 worldTangent;

varying vec3 position;

const float waveStrenght = 0.1;
const float normalAmount = 2.0;
const float specularity = 1.0;

void main(void)
{

     // Move waveTexCoords
    vec2 waveMovCoord = waveTexCoord;    
    vec4 wave = texture2D(waveTexture, waveMovCoord);

    //Wiggle the newCoord by r and b colors of waveTexture
    vec2 newCoord = texCoord;
    newCoord.x += wave.r * waveStrenght;
    newCoord.y -= wave.b * waveStrenght;

    //1
    vec4 diffuseTextureColor = texture2D(waterDiffuse, newCoord);

    // 2 Animated Layers of specularTexture mixed with the newCoord
    vec4 specularTextureColor = texture2D( specularTexture, multexCoord+newCoord) + (texture2D( specularTexture, movtexCoord+newCoord ));

    // 3 Animated Layers of normalTexture mixed with the newCoord 为切线空间坐标系下的向量
    vec3 tNormal = normalAmount * texture2D( normalTexture, movtexCoord+newCoord ).rgb - vec3( 1.0 )+
            (normalAmount * texture2D( normalTexture, multexCoord+newCoord ).rgb - vec3( 1.0 ));

    // 4 Animated skyTexture layer
    vec4 skycolor = texture2D(skyTexture, skyTexCoord);
    skycolor = skycolor * 0.8;

    //5 Animated foamTexture layer
    vec4 foamTextureColor = texture2D(foamTexture, texCoord);
   
    vec4 diffuse = vec4(diffuseTextureColor);
    vec4 specular = vec4(specularTextureColor.a*specularity);

    vec3 worldBTangent   = cross(worldNormal, worldTangent); //B
    mat3 inverseTBN = transpose(mat3(worldTangent, worldBTangent, worldNormal));

    vec4 outputColor = specular + diffuse ;
    outputColor += vec4(skycolor.rgb, position.y);
    outputColor += (foamTextureColor.rgba*position.y);

    outputColor = normalize(outputColor);

    gl_FragColor = vec4(outputColor.rgb,1.0);
}