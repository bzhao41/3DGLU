#version 150

out vec4 fColor;

in vec3 fN;
in vec3 fL;
in vec3 fL2;
in vec3 fE;
in vec2 Texture;

uniform sampler2D textureID;

uniform vec4 fCamera;

uniform vec4 lightPos;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular;
uniform int enabled1;

uniform vec4 lightAmbient2, lightDiffuse2, lightSpecular2;
uniform int enabled2;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

void main() 
{
    vec4 color1 = vec4(0,0,0,0);
    vec4 color2 = vec4(0,0,0,0);
    vec4 t = vec4(0, 0, 0, 0);
    //sun
    if (enabled1 == 1) {
        t = texture(textureID,Texture);
        vec3 N = normalize(fN);
        vec3 E = normalize(fE);
        vec3 L = normalize(fL);
        
        vec3 H = normalize(L + E);
        vec4 ambient = lightAmbient * matAmbient;
        
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * lightDiffuse * matDiffuse;
        
        float Ks = pow(max(dot(N, H), 0.0), matAlpha);
        vec4 specular = Ks * lightSpecular * matSpecular;
        
        if(dot(L, N) < 0.0) {
            specular = vec4(0, 0, 0, 1);
        }
        //float intensity = *max(lightPos.y, 0);
        float attenuation = max(lightPos.y/(length(L) * length(L)), 0);
        //float attenuation = 1.0 / (1.0 + (length(fL) * length(fL)));
        color1 = attenuation*(ambient + diffuse + specular);
        t = t * attenuation;
        
        color1.a = 1.0;
    }
    
    //flashlight
    if (enabled2 == 1) {
        
        vec3 N = normalize(fN);
        vec3 E = normalize(fE);
        vec3 L = normalize(fL2);
        
        vec3 H = normalize(L + E);
        vec4 ambient = lightAmbient2 * matAmbient;
        
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * lightDiffuse2 * matDiffuse;
        
        float Ks = pow(max(dot(N, H), 0.0), matAlpha);
        vec4 specular = Ks * lightSpecular2 * matSpecular;
        
        if(dot(L, N) < 0.0) {
            specular = vec4(0, 0, 0, 1);
        }
        
        float theta = acos(dot(fCamera.xyz, L));
        float dropoff = 10.0;
        float c;
        
        if(abs(theta) > 35.0 * 3.14/180.0){
            c = 0;
        } else {
            c = pow(max(dot(fCamera.xyz, L), 0.0), dropoff);
            t = texture(textureID,Texture);
        }
        color2 = (ambient + diffuse + specular) * c;
        color2.a = 1.0;
    }

    fColor = t + color1 + color2;
 //   fColor = texture(textureID,Texture);
    fColor.a = 1.0;
}

