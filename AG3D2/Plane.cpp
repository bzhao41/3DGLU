//
//  Plane.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 3/3/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane() {
    assignGouraudVertices();
    build();
}

Plane::~Plane() {
    
}

void Plane::build() {
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    program = InitShader("./vshaderFragPlane_v150.glsl", "./fshaderFragPlane_v150.glsl");
    glUseProgram(program); //make it the active one

    mmLoc = glGetUniformLocation(program, "model_matrix");
    vmLoc = glGetUniformLocation(program, "view_matrix");
    pmLoc = glGetUniformLocation(program, "proj_matrix");
    texLoc = glGetUniformLocation(program, "textureID");
    matDiffuse = glGetUniformLocation(program, "matDiffuse");
    matSpecular = glGetUniformLocation(program, "matSpecular");
    matAmbient = glGetUniformLocation(program, "matAmbient");
    matAlpha = glGetUniformLocation(program, "matAlpha");
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vPlane) + sizeof(nPlane) + sizeof(tPlane), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vPlane), vPlane);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vPlane), sizeof(nPlane), nPlane);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vPlane) + sizeof(nPlane), sizeof(tPlane), tPlane);
    
    vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition); //enable it
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    nPosition = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(nPosition);
    glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vPlane)));

    vTexture = glGetAttribLocation(program, "vTexture");
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vPlane) + sizeof(nPlane)));
    
    int imgSize = 256;
    GLubyte *data = ppmRead("./grass256by256.ppm", &imgSize, &imgSize);
    glGenTextures(1, &Texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgSize, imgSize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Plane::draw(Camera* cam, vector<Light*> lights) {
    glBindVertexArray(VAO);
    glUseProgram(program);

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
    
    //light one
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
    
    //light two
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
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Plane::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
    diffuse = d;
    specular = s;
    ambient = a;
    shine = sh;
}

void Plane::assignGouraudVertices() {
    for (int i = 0; i < 6; i++) {
        nPlane[i] = vec3(0, 1, 0);
    }
}
