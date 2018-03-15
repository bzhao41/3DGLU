#version 150
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexture;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec3 fL2;
out vec2 Texture;

uniform vec4 lightPos;
uniform vec4 lightPos2;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;


void main(){
    
    Texture = vTexture;
    vec3 pos = (view_matrix * model_matrix * vPosition).xyz;
    vec3 lightPosInCam = (view_matrix * lightPos).xyz;
    vec3 lightPosInCam2 = (view_matrix * lightPos2).xyz;
    
    fN = (view_matrix * model_matrix * vec4(vNormal, 0)).xyz;
    fE = vec3(0, 0, 0) - pos;

    //sun
    fL = (lightPosInCam.xyz - pos);
    //flashlight
    fL2 = (lightPosInCam2.xyz - pos);
    
    gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;
}
