#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "node.h"

class Viewer {
public:
    Viewer(int width=640, int height=480);

    void run();
    void on_key(int key);
    void set_arm_nodes(Node* left_arm, Node* right_arm);
    void set_leg_nodes(Node* left_leg, Node* right_leg);
    Node *scene_root;

private:
    GLFWwindow* win;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    Node* left_arm_node_ = nullptr;
    Node* right_arm_node_ = nullptr;
    Node* left_leg_node_ = nullptr; 
    Node* right_leg_node_ = nullptr;
    float arm_angle_ = 0.0f;
};

#endif // VIEWER_H
