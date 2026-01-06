#include "pyramide.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Pyramide::Pyramide(Shader* shader_program) : Shape(shader_program) {

    // Base CARRÉE : 4 points au lieu de 3
    // Points de la base carrée (dans le plan y = -0.5) :
    // A(-0.5, -0.5,  0.5)  // avant-gauche
    // B( 0.5, -0.5,  0.5)  // avant-droite  
    // C( 0.5, -0.5, -0.5)  // arrière-droite
    // D(-0.5, -0.5, -0.5)  // arrière-gauche
    // Sommet : S(0.0, 0.5, 0.0)

    GLfloat vertex_buffer_data[] = {
        // Base carrée (2 triangles pour former un carré)
        // Triangle 1 de la base : A-B-D
        -0.5f, -0.5f,  0.5f,  // A
         0.5f, -0.5f,  0.5f,  // B
        -0.5f, -0.5f, -0.5f,  // D

        // Triangle 2 de la base : B-C-D  
         0.5f, -0.5f,  0.5f,  // B
         0.5f, -0.5f, -0.5f,  // C
        -0.5f, -0.5f, -0.5f,  // D

        // Face avant : A-B-S
        -0.5f, -0.5f,  0.5f,  // A
         0.5f, -0.5f,  0.5f,  // B
         0.0f,  0.5f,  0.0f,  // S (sommet)

         // Face droite : B-C-S
          0.5f, -0.5f,  0.5f,  // B
          0.5f, -0.5f, -0.5f,  // C
          0.0f,  0.5f,  0.0f,  // S

          // Face arrière : C-D-S
           0.5f, -0.5f, -0.5f,  // C
          -0.5f, -0.5f, -0.5f,  // D
           0.0f,  0.5f,  0.0f,  // S

           // Face gauche : D-A-S
           -0.5f, -0.5f, -0.5f,  // D
           -0.5f, -0.5f,  0.5f,  // A
            0.0f,  0.5f,  0.0f   // S
    };

    // 6 triangles × 3 vertices = 18 vertices au total
    // (2 pour la base + 4 pour les faces latérales)

    GLfloat color_buffer_data[] = {
        // Base (bleu pour les 6 vertices - 2 triangles)
        // Triangle 1 de la base
        0.0f, 0.0f, 0.1f, 1.0f,  // A (bas)
        0.0f, 0.0f, 0.1f, 1.0f,  // B (bas)
        0.0f, 0.0f, 0.1f, 1.0f,  // D (bas)

        // Triangle 2 de la base
        0.0f, 0.0f, 0.1f, 1.0f,  // A (bas)
        0.0f, 0.0f, 0.1f, 1.0f,  // B (bas)
        0.0f, 0.0f, 0.1f, 1.0f,  // D (bas)

        // Face avant
        0.0f, 0.1f, 0.3f, 1.0f,  // A (bas - bleu foncé)
        0.0f, 0.1f, 0.3f, 1.0f,  // B (bas - bleu foncé)
        0.4f, 0.7f, 1.0f, 1.0f,  // S (haut - bleu ciel clair)

        // Face droite
        0.0f, 0.1f, 0.3f, 1.0f,  // B (bas - bleu foncé)
        0.0f, 0.1f, 0.3f, 1.0f,  // C (bas - bleu foncé)
        0.4f, 0.7f, 1.0f, 1.0f,  // S (haut - bleu ciel clair)

        // Face arrière
        0.0f, 0.1f, 0.3f, 1.0f,  // C (bas - bleu foncé)
        0.0f, 0.1f, 0.3f, 1.0f,  // D (bas - bleu foncé)
        0.4f, 0.7f, 1.0f, 1.0f,  // S (haut - bleu ciel clair)

        // Face gauche 
        0.0f, 0.1f, 0.3f, 1.0f,  // D (bas - bleu foncé)
        0.0f, 0.1f, 0.3f, 1.0f,  // A (bas - bleu foncé)
        0.4f, 0.7f, 1.0f, 1.0f   // S (haut - bleu ciel clair)
    };


    // Créer le VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Créer 2 buffers
    glGenBuffers(2, &VBO[0]);

    // Configurer le VBO 0 pour les positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Configurer le VBO 1 pour les couleurs
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}

Pyramide::~Pyramide() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &VBO[0]);
}

void Pyramide::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    // Appeler la méthode draw de la classe parent Shape
    Shape::draw(model, view, projection);

    // Dessiner 18 vertices (6 triangles × 3 vertices)
    glDrawArrays(GL_TRIANGLES, 0, 18);

    glBindVertexArray(0);
}

void Pyramide::key_handler(int key) {
    return;
}

