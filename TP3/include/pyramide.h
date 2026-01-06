#ifndef PYRAMIDE_H
#define PYRAMIDE_H

#include "shape.h"
#include "shader.h"
#include <GL/glew.h>

class Pyramide : public Shape {
public:
    Pyramide(Shader* shader_program);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Pyramide();

private:
    //GLuint shader_program_; je l'enleve parce que mtn c'est shape qui a se parametre
    GLuint VAO;
    GLuint VBO[2];
};

#endif

