#include "Cube.h"

Cube::Cube(vec3 position) {
    buildIndexedCube();
    init();
    modelmatrix = Translate(position.x, position.y, position.z);
}

void Cube::init() {
    program = InitShader("./vshaderFragCube_v150.glsl", "./fshaderFragCube_v150.glsl");
    glUseProgram(program); //make it the active one

    mmLoc = glGetUniformLocation(program, "model_matrix");
    vmLoc = glGetUniformLocation(program, "view_matrix");
    pmLoc = glGetUniformLocation(program, "proj_matrix");
    texLoc = glGetUniformLocation(program, "textureID");
    
    matDiffuse = glGetUniformLocation(program, "matDiffuse");
    matSpecular = glGetUniformLocation(program, "matSpecular");
    matAmbient = glGetUniformLocation(program, "matAmbient");
    matAlpha = glGetUniformLocation(program, "matAlpha");
    
     //make this VAO active
    glGenBuffers(1, &VBO);
    //put the data on the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals) + sizeof(vertexTextureLocations),NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexNormals), vertexNormals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals), sizeof(vertexTextureLocations), vertexTextureLocations);
    
    //set up stuff for the body of the cube
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //associate the VBO with the active VAO
    
    vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition); //enable it
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    nPosition = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(nPosition);
    glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
    
    vTexture = glGetAttribLocation(program, "vTexture");
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations) + sizeof(vertexNormals)));
    
    int imgsize = 512;
    
    int width = 1024;
    int height = 768;
    GLubyte *data1 = ppmRead("./crate_texture.ppm", &imgsize, &imgsize);
    GLubyte *data2 = ppmRead("./brks.ppm", &width, &height);
    glGenTextures(1, &Texture[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[0]);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);

    delete[] data1;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glGenTextures(1, &Texture[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    
    delete[] data2;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

Cube::~Cube(){

}

void Cube::makeIndexedQuad(GLubyte ind1, GLubyte ind2, GLubyte ind3, GLubyte ind4) {
    
    //Triangle 1
    vec3 N = normalize(cross(vertices[ind2] - vertices[ind1], vertices[ind3] - vertices[ind1]));
    vertexLocations[index] = vertices[ind1];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(0,0);
    index++;
    
    vertexLocations[index] = vertices[ind2];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(1,0);
    index++;
    
    vertexLocations[index] = vertices[ind3];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(1,1);
    index++;
    
    //Triangle 2
    N = normalize(cross(vertices[ind3] - vertices[ind1], vertices[ind4] - vertices[ind1]));
    vertexLocations[index] = vertices[ind3];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(1,1);
    index++;
    
    vertexLocations[index] = vertices[ind4];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(0,1);
    index++;
    
    vertexLocations[index] = vertices[ind1];
    vertexNormals[index] = N;
    vertexTextureLocations[index] = vec2(0,0);
    index++;
}

void Cube::buildIndexedCube() {
    index = 0;
    makeIndexedQuad(1, 0, 3, 2);  //front
    makeIndexedQuad(2, 3, 7, 6);  //right
    makeIndexedQuad(3, 0, 4, 7);  //bottom
    makeIndexedQuad(6, 5, 1, 2);  //top
    makeIndexedQuad(4, 5, 6, 7);  //back
    makeIndexedQuad(5, 4, 0, 1);  //left
}

void Cube::draw(Camera* cam, vector<Light*> lights){
    glBindVertexArray(VAO);
    glUseProgram(program); //also switch to using this shader program
    

    glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
    glUniformMatrix4fv(vmLoc, 1, GL_TRUE, cam->getView());
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam->getProjection());
    
    glUniform1i(texLoc, 0);
    
    GLuint fCamera = glGetUniformLocation(program, "fCamera");
    glUniform4fv(fCamera, 1, cam->getForward());
    
    glUniform4fv(matDiffuse, 1, diffuse);
    glUniform4fv(matSpecular, 1, specular);
    glUniform4fv(matAmbient, 1, ambient);
    glUniform1f(matAlpha, shine);
    
    //light one - sun
    GLuint light_loc = glGetUniformLocation(program, "lightPos");
    glUniform4fv(light_loc, 1, lights[0]->getPosition());
    GLuint ambient_loc = glGetUniformLocation(program, "lightAmbient");
    glUniform4fv(ambient_loc, 1, lights[0]->getAmbient());
    GLuint diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
    glUniform4fv(diffuse_loc, 1, lights[0]->getDiffuse());
    GLuint specular_loc = glGetUniformLocation(program, "lightSpecular");
    glUniform4fv(specular_loc, 1, lights[0]->getSpecular());
    GLuint light_on = glGetUniformLocation(program, "enabled1");
    glUniform1i(light_on, lights[0]->isOn());
    
    //light two - flashlight
    GLuint light_loc2 = glGetUniformLocation(program, "lightPos2");
    glUniform4fv(light_loc2, 1, lights[1]->getPosition());
    GLuint ambient_loc2 = glGetUniformLocation(program, "lightAmbient2");
    glUniform4fv(ambient_loc2, 1, lights[1]->getAmbient());
    GLuint diffuse_loc2 = glGetUniformLocation(program, "lightDiffuse2");
    glUniform4fv(diffuse_loc2, 1, lights[1]->getDiffuse());
    GLuint specular_loc2 = glGetUniformLocation(program, "lightSpecular2");
    glUniform4fv(specular_loc2, 1, lights[1]->getSpecular());
    GLuint light_on2 = glGetUniformLocation(program, "enabled2");
    glUniform1i(light_on2, lights[1]->isOn());
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[activeTexture]);
    
    glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
}

void Cube::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
    diffuse = d;
    specular = s;
    ambient = a;
    shine = sh;
}

