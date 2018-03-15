#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexture;

uniform sampler2D textureID;
//uniform vec4 vColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix; //camera matrix
uniform mat4 proj_matrix;

uniform vec4 fCamera;

uniform vec4 lightPos;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular;
uniform int enabled1;

uniform vec4 lightPos2;
uniform vec4 lightAmbient2, lightDiffuse2, lightSpecular2;
uniform int enabled2;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

out vec4 color;

void main(){
    vec4 color1 = vec4(0,0,0,0);
    vec4 color2 = vec4(0,0,0,0);
    vec4 t = texture(textureID, vTexture);
    
    //sun
    if (enabled1 == 1) {
        vec3 pos = (view_matrix * model_matrix * vPosition).xyz;
        vec3 lightPosInCam = (view_matrix * lightPos).xyz;
        
        vec3 L = normalize(lightPosInCam.xyz - pos);
        float dist = 1.0;
        
        vec3 E = normalize(vec3(0, 0, 0) - pos);
        
        vec3 N = normalize(view_matrix * model_matrix * vec4(vNormal, 0)).xyz;
        
        vec3 H = normalize(L + E);
        
        vec4 ambient = lightAmbient * matAmbient;
        
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * lightDiffuse * matDiffuse;
        
        float Ks = pow(max(dot(N, H), 0.0), matAlpha);
        vec4 specular = Ks * lightSpecular * matSpecular;
        
        if (dot(L, N) < 0.0) {
            specular = vec4(0, 0, 0, 1);
        }
        float intensity = max(lightPos.y/(length(L) * length(L)), 0);
        color1 = intensity*(ambient + diffuse + specular);

        color1.a = 1.0;
    }
    
    //flashlight
    if (enabled2 == 1) {
        vec3 pos = (view_matrix * model_matrix * vPosition).xyz;
        vec3 lightPosInCam = (view_matrix * lightPos2).xyz;
        
        //ray from vertext to light
        vec3 L = normalize(lightPosInCam.xyz - pos);
        float dist = 1.0;
        
        //vertext towards camera
        vec3 E = normalize(vec3(0, 0, 0) - pos);
        
        //normal in camera coordinates
        vec3 N = normalize(view_matrix * model_matrix * vec4(vNormal, 0)).xyz;
        
        vec3 H = normalize(L + E);
        
        vec4 ambient = lightAmbient2 * matAmbient;
        
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * lightDiffuse2 * matDiffuse;
        
        float Ks = pow(max(dot(N, H), 0.0), matAlpha);
        vec4 specular = Ks * lightSpecular2 * matSpecular;
        
        if (dot(L, N) < 0.0) {
            specular = vec4(0, 0, 0, 1);
        }
        
        float theta = acos(dot(fCamera.xyz, L));
        float dropoff = 10.0;
        float c;
        if(abs(theta) > 35.0 * 3.14/180.0){
            c = 0;
        } else {
            c = pow(max(dot(fCamera.xyz, L), 0.0), dropoff);
        }
        color2 = (ambient + diffuse + specular) * c;
        color2.a = 1.0;
        
    }
    
    //color = color1 + color2;
    color = t;
    color.a = 1.0;
    gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;
    
}

