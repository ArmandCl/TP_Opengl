#include "viewer.h"
#include "triangle.h"
#include "pyramide.h"
#include "cylinder.h"
#include "node.h"
#include "shader.h"
#include <string>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer;

    // get shader directory
    std::string shader_dir = SHADER_DIR;

    Shader* color_shader = new Shader(shader_dir + "node.vert", shader_dir + "node.frag");

    // Toute la hiérarchie du squelette humain
    //schéma que  j'ai utilisé pour chaque partie du corps
    //glm::mat4 nom_mat = glm::translate(glm::mat4(1.0f), POSITION)
    //    * glm::scale(glm::mat4(1.0f), ECHELLE)
    //    * glm::rotate(glm::mat4(1.0f), ANGLE, AXE);


    // CORPS ENTIER (racine)
    glm::mat4 human_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* human = new Node(human_mat);

    // TÊTE (pyramide)
    glm::mat4 head_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* head_node = new Node(head_mat);
    Shape* head = new Pyramide(color_shader);
    head_node->add(head);
    human->add(head_node);


    // corps (cylindre)
    glm::mat4 body_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Node* body_node = new Node(body_mat);
    Shape* body = new Cylinder(color_shader, 2.0f, 0.5f, 16);  // Hauteur, Rayon, côtés
    body_node->add(body);
    human->add(body_node);

    // jambe droite (cylindre que je vais mettre debout en mettant les radians à 90.0f à la place de 0.0f comme j'ai fait pour le corps)
    glm::mat4 right_leg_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* right_leg_node = new Node(right_leg_mat);
    Shape* right_leg = new Cylinder(color_shader, 1.0f, 0.2f, 16);  // Hauteur, Rayon, côtés
    right_leg_node->add(right_leg);
    human->add(right_leg_node);

    // jambe gauche (cylindre que je vais mettre debout en mettant les radians à 90.0f à la place de 0.0f comme j'ai fait pour le corps)
    glm::mat4 left_leg_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -1.0f, 0.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Node* left_leg_node = new Node(left_leg_mat);
    Shape* left_leg = new Cylinder(color_shader, 1.0f, 0.2f, 16);  // Hauteur, Rayon, côtés
    left_leg_node->add(left_leg);
    human->add(left_leg_node);

    // bras gauche (cylindre pour le mettre horizontal, j'ai juste rotade 1.0f)
    glm::mat4 left_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-0.6f, 0.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f))  // 135° sur Z
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Node* left_arm_node = new Node(left_arm_mat);
    Shape* left_arm = new Cylinder(color_shader, 1.2f, 0.15f, 16);  // Hauteur, Rayon, côtés
    left_arm_node->add(left_arm);
    human->add(left_arm_node);


    // Bras droit - diagonal / (bas-droite)
    glm::mat4 right_arm_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f))   // 45° l'axe Z comme ça je peux le pencher
        * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Node* right_arm_node = new Node(right_arm_mat);
    Shape* right_arm = new Cylinder(color_shader, 1.2f, 0.15f, 16);
    right_arm_node->add(right_arm);
    human->add(right_arm_node);

    viewer.set_arm_nodes(left_arm_node, right_arm_node);
    viewer.set_leg_nodes(left_leg_node, right_leg_node);

    viewer.scene_root->add(human);

    viewer.run();

    // Nettoyage
    delete color_shader;

    return 0;
}