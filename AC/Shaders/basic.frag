#version 330 core
out vec4 FragColor;

struct Material {
    vec3 kA;
    vec3 kD;
    vec3 kS;
    float shine;
};

in vec3 chNormal;
in vec3 chFragPos;
in vec2 chUV;
  
uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;

uniform sampler2D uDiffMap1;
uniform bool uUseTexture;
uniform Material uMaterial;
uniform float uAlpha = 1.0;

void main()
{    
    vec3 ambient = uLightColor * uMaterial.kA;

    vec3 norm = normalize(chNormal);
    vec3 lightDir = normalize(uLightPos - chFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLightColor * (diff * uMaterial.kD);    

    vec3 viewDir = normalize(uViewPos - chFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shine);
    vec3 specular = uLightColor * (spec * uMaterial.kS);

    vec3 result;
    if(uUseTexture) {
        vec3 texColor = texture(uDiffMap1, chUV).rgb;
        result = (ambient + (diff * texColor) + specular);
    } else {
        result = ambient + diffuse + specular;
    }

    FragColor = vec4(result, uAlpha);
}

