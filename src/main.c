#include "shader.h"
#include "util.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <cglm/vec2.h>

static vec2 vertex_positions[] = {
    { -1, -1 },
    { 1, -1 },
    { -1, 1 },
    //
    { 1, 1 },
    { 1, -1 },
    { -1, 1 }
};

int main() {
    if (!glfwInit()) {
        puts("Failed to initialize GLFW");
        exit(-1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
    if (win == NULL) {
        glfwTerminate();
        puts("Failed to open GLFW window");
        exit(-1);
    }   
    glfwMakeContextCurrent(win);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        puts("Failed to initialize GLEW");
        exit(-1);
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint vert_array;
    glGenVertexArrays(1, &vert_array);
    glBindVertexArray(vert_array);

    #define NUM_SHADER_PROGRAMS SIZEOF_ARRAY(shader_path_pairs)

    shader_path_pairs shader_path_pairs[] = {
        { "shader/vertex.glsl", "shader/fragment.glsl" }
    };
    GLuint shader_programs[NUM_SHADER_PROGRAMS];
    error_t code = load_shader_programs(NUM_SHADER_PROGRAMS, shader_path_pairs, shader_programs);
    if (code != 0) {
        printf("Error: %s\n", strerror(code));
        exit(-1);
    }

    #undef NUM_SHADER_PROGRAMS

    GLuint vertex_buf;
    glGenBuffers(1, &vertex_buf);

    glUseProgram(shader_programs[0]);

    for (;;) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        glDrawArrays(GL_TRIANGLES, 0, SIZEOF_ARRAY(vertex_positions) * 2);

        glDisableVertexAttribArray(0);

        glfwSwapBuffers(win);

        glfwPollEvents();

        if (glfwWindowShouldClose(win)) {
            break;
        }
    }

    return 0;
}