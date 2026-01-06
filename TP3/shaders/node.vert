#version 330 core

// input attribute variable, given per vertex
in vec3 position;

// global matrix variables
// TODO add model uniform
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// color given to the fragment shader
out vec3 fragColor;

void main() {

    gl_Position = projection * view * model * vec4(position, 1.0);
    fragColor = position;
}
