//
//  Sphere.cpp
//  AG3D2
//
//  Created by Bruce Zhao on 2/22/18.
//  Copyright © 2018 Bruce Zhao. All rights reserved.
//

#include "Sphere.hpp"

vec4 Sphere::unit(vec4 p) {
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    vec4 t;
    if (len > DivideByZeroTolerance) {
        t = p / sqrt(len);
        t.w = 1.0;
    }
    return t;
}

void Sphere::triangle(vec4 ai, vec4 bi, vec4 ci) {
    vec3 a = vec3(ai.x, ai.y, ai.z);
    vec3 b = vec3(bi.x, bi.y, bi.z);
    vec3 c = vec3(ci.x, ci.y, ci.z);
    
    float u, v;
    
    vec3 N = normalize(cross(b - a, c - a));
    
    vertexLocations[index] = a;
    vertexNormals[index] = N;
    u = .5 + asin(a.x)/M_PI;
    v = .5 + asin(a.y)/M_PI;
    vertexTextures[index] = vec2(u, -v);
    index++;
    
    vertexLocations[index] = b;
    vertexNormals[index] = N;
    u = .5 + asin(b.x)/M_PI;
    v = .5 + asin(b.y)/M_PI;
    vertexTextures[index] = vec2(u, -v);
    index++;
    
    vertexLocations[index] = c;
    vertexNormals[index] = N;
    u = .5 + asin(c.x)/M_PI;
    v = .5 + asin(c.y)/M_PI;
    vertexTextures[index] = vec2(u, -v);
    index++;
}

void Sphere::tetrahedron(int count) {
    vec4 v[4] {
        vec4(0, 0, 1.0, 1.0),
        vec4(0, 2.0f*sqrt2/3.0f, -1.0f/3.0f, 1.0f),
        vec4(-sqrt6/3.0f, -sqrt2/3.0f, -1.0f/3.0f, 1.0f),
        vec4(sqrt6/3.0f,-sqrt2/3.0f,-1.0f/3.0f, 1.0f)
    };
    
    divideTriangle(v[0], v[1], v[2], count);
    divideTriangle(v[3], v[2], v[1], count);
    divideTriangle(v[0], v[3], v[1], count);
    divideTriangle(v[0], v[2], v[3], count);
}

Sphere::Sphere(vec3 position) {
    sqrt2 = (float)sqrt(2);
    sqrt6 = (float)sqrt(6);
    origin = position;
    build();
    modelmatrix = Translate(position.x, position.y, position.z);
}

Sphere::Sphere(vec3 position, float r) {
    radius = r;
    origin = position;
    sqrt2 = (float)sqrt(2);
    sqrt6 = (float)sqrt(6);
    
    build();
    modelmatrix = Translate(position.x, position.y, position.z);
}

Sphere::~Sphere(){
    
}

void Sphere::build() {
    
    index = 0;
    tetrahedron(9);
    
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    
    assert((program = InitShader("./vshaderVertex_v150.glsl", "./fshaderVertex_v150.glsl"))!=-1);
    glUseProgram(program); //make it the active one
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals) + sizeof(vertexTextures), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexNormals), vertexNormals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals), sizeof(vertexTextures), vertexTextures);
    
    
    vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition); //enable it
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    nPosition = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(nPosition);
    glVertexAttribPointer(nPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
    
    vTexture = glGetAttribLocation(program, "vTexture");
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations) + sizeof(vertexNormals)));
    
    texLoc = glGetUniformLocation(program, "textureID");
    mmLoc = glGetUniformLocation(program, "model_matrix");
    vmLoc = glGetUniformLocation(program, "view_matrix");
    pmLoc = glGetUniformLocation(program, "proj_matrix");
    matDiffuse = glGetUniformLocation(program, "matDiffuse");
    matSpecular = glGetUniformLocation(program, "matSpecular");
    matAmbient = glGetUniformLocation(program, "matAmbient");
    matAlpha = glGetUniformLocation(program, "matAlpha");
    
    int width = 1600;
    int height = 800;
    GLubyte *data = ppmRead("./Planet.ppm", &width, &height);
    glGenTextures(1, &Texture[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    delete[] data;
}

void Sphere::divideTriangle(vec4 a,vec4 b, vec4 c, int count) {
    if (count > 0) {
        vec4 v1 = unit(a + b);
        v1.w = 1.0;
        vec4 v2 = unit(a + c);
        v2.w = 1.0;
        vec4 v3 = unit(b + c);
        v3.w = 1.0;
        
        divideTriangle(a, v1, v2, count - 1);
        divideTriangle(c, v2, v3, count - 1);
        divideTriangle(b, v3, v1, count - 1);
        divideTriangle(v1, v3, v2, count - 1);
    } else {
        triangle(radius * a, radius * b, radius * c);
    }
}

void Sphere::draw(Camera* cam, vector<Light*> lights) {
    glBindVertexArray(VAO);
    glUseProgram(program); //also switch to using this shader program
    
    //camera
    glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
    glUniformMatrix4fv(vmLoc, 1, GL_TRUE, cam->getView());
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam->getProjection());
    
    glUniform1i(texLoc, 0);
    
    GLuint fCamera = glGetUniformLocation(program, "fCamera");
    glUniform4fv(fCamera, 1, cam->getForward());
    
    //material
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
    glBindTexture(GL_TEXTURE_2D, Texture[0]);
    
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

void Sphere::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
    diffuse = d;
    specular = s;
    ambient = a;
    shine = sh;
}

void Sphere::assignGouradVertices() {
    vec3 normalSum[numVertices];
    int counts[numVertices];
    
    for (int i = 0; i < numVertices; i++) {
        normalSum[i] = vec3(0, 0, 0);
        counts[i] = 0;
    }
    
    for (int i = 0; i < numVertices; i++) {
        int count = 0;
        for (int j = 0; j < numVertices; j++) {
            if ((vertexLocations[i].x == vertexLocations[j].x)
                && (vertexLocations[i].y == vertexLocations[j].y)
                && (vertexLocations[i].z == vertexLocations[j].z)) {
                count++;
                normalSum[i] += vertexNormals[j];
            }
        }
        counts[i] = count;
    }
    for (int i = 0; i < numVertices; i++) {
        vertexNormals[i] = normalSum[i]/counts[i];
    }
}


