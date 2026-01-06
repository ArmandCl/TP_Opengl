#include "viewer.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

Viewer::Viewer(int width, int height)
{
    if (!glfwInit())    // initialize window system glfw
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // version hints: create GL window with >= OpenGL 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    // make win's OpenGL context current; no OpenGL calls can happen before
    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set user pointer for GLFW window to this Viewer instance
    glfwSetWindowUserPointer(win, this);

    // register event handlers
    glfwSetKeyCallback(win, key_callback_static);

    // useful message to check OpenGL renderer characteristics
    std::cout << glGetString(GL_VERSION) << ", GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
              << glGetString(GL_RENDERER) << std::endl;

    // initialize GL by setting viewport and default render characteristics
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer
    than anything already drawn at that pixel */
    glEnable( GL_DEPTH_TEST ); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
    glDepthFunc( GL_LESS );

    // initialize our scene_root
    scene_root = new Node();
}
void print_matrix(const glm::mat4& m, const std::string& name) {
    std::cout << "\n" << name << ":" << std::endl;
    std::cout << "  Position: [" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << "]" << std::endl;
    std::cout << "  Scale/Rot: [" << m[0][0] << ", " << m[1][1] << ", " << m[2][2] << "]" << std::endl;
}


void Viewer::run()
{
    float time = 0.0f;
    float last_time = glfwGetTime();
    int frame_count = 0;

    while (!glfwWindowShouldClose(win))
    {
        frame_count++;
        float current_time = glfwGetTime();
        float delta_time = current_time - last_time;
        last_time = current_time;
        time += delta_time;

        // ANIMATION DE COURSE
        float swing_angle = sin(time * 4.0f) * 0.7f;  // Course rapide

        // ANIMER LES BRAS
        if (left_arm_node_ && right_arm_node_) {
            static bool arm_first = true;
            static glm::mat4 left_arm_base, right_arm_base;

            if (arm_first) {
                left_arm_base = left_arm_node_->get_transform();
                right_arm_base = right_arm_node_->get_transform();
                arm_first = false;
            }

            // Bras en opposition (course)
            float left_arm_swing = swing_angle;
            float right_arm_swing = -swing_angle;

            // Axe pour mouvement avant/arrière
            glm::vec3 arm_axis = glm::normalize(glm::vec3(0.8f, 0.0f, 0.2f));

            left_arm_node_->set_transform(glm::rotate(glm::mat4(1.0f), left_arm_swing, arm_axis) * left_arm_base);
            right_arm_node_->set_transform(glm::rotate(glm::mat4(1.0f), right_arm_swing, arm_axis) * right_arm_base);
        }

        // ANIMER LES JAMBES
        if (left_leg_node_ && right_leg_node_) {
            static bool leg_first = true;
            static glm::mat4 left_leg_base, right_leg_base;

            if (leg_first) {
                left_leg_base = left_leg_node_->get_transform();
                right_leg_base = right_leg_node_->get_transform();
                leg_first = false;
            }

            // Jambes en opposition avec les bras
            float left_leg_swing = -swing_angle * 0.5f;   // Opposition + moins ample
            float right_leg_swing = swing_angle * 0.5f;

            // Axe pour jambes (un peu différent)
            glm::vec3 leg_axis = glm::normalize(glm::vec3(0.9f, 0.0f, 0.1f));

            left_leg_node_->set_transform(glm::rotate(glm::mat4(1.0f), left_leg_swing, leg_axis) * left_leg_base);
            right_leg_node_->set_transform(glm::rotate(glm::mat4(1.0f), right_leg_swing, leg_axis) * right_leg_base);
        }

        // DEBUG
        if (frame_count % 60 == 0) {
            std::cout << "Course - Swing: " << swing_angle << std::endl;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 rot_mat = glm::mat4(1.0f);
        glm::mat4 tra_mat = glm::mat4(1.0f);
        glm::mat4 sca_mat = glm::mat4(1.0f);
        glm::mat4 view = tra_mat * rot_mat * sca_mat;

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);

        scene_root->draw(model, view, projection);

        glfwPollEvents();
        glfwSwapBuffers(win);
    }

    glfwTerminate();
}

void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key);
}

void Viewer::on_key(int key)
{
    // 'Q' or 'Escape' quits
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

void Viewer::set_arm_nodes(Node* left_arm, Node* right_arm) {
    left_arm_node_ = left_arm;
    right_arm_node_ = right_arm;
}

void Viewer::set_leg_nodes(Node* left_leg, Node* right_leg) {
    left_leg_node_ = left_leg;
    right_leg_node_ = right_leg;

    std::cout << "Pointeurs jambes reçus: "
        << (left_leg_node_ ? "Gauche OK" : "Gauche NULL") << ", "
        << (right_leg_node_ ? "Droit OK" : "Droit NULL") << std::endl;
}