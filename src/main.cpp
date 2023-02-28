#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

#include "global.hpp"
#include "shader.hpp"
#include "test.hpp"
#include "verts.hpp"

GLFWwindow* window = nullptr;
unsigned int vbo;
unsigned int vao;
unsigned int texture;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void init() {
    glfwSetErrorCallback(
        [](int error, const char* description) { fmt::print("GLFW Error {}: {}\n", error, description); });

    // Init GLFW
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW. Quitting\n");
    glewExperimental = true;

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Window is null. Quitting\n");
    }

    glfwMakeContextCurrent(window);

    glewInit();

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

//////////

std::array<Vertex, 0x10000> verts;
size_t arrayIndex = 0;
size_t vertCount = 0;
size_t vertOffset = 0;

int main() {
    init();
    Shader shader("../vert.glsl", "../frag.glsl");
    shader.activate();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0x10000, nullptr, GL_STREAM_DRAW);

    // verts
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // clut
    glVertexAttribPointer(2, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, clut));
    glEnableVertexAttribArray(2);

    // texpage
    glVertexAttribPointer(3, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texpage));
    glEnableVertexAttribArray(3);

    // uv
    glVertexAttribPointer(4, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(4);

    //    GL_RGBA8

    glUniform1i(glGetUniformLocation(shader.m_programID, "u_vramTex"), 0);
    glUniform4i(glGetUniformLocation(shader.m_programID, "u_texWindow"), 0, 0, 0, 0);
    tex();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1024, 512);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 640, 0, 60, 48, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, &texdata[0]);
    //    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1024, 512);
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 512, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, &texdata[0]);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    verts[vertCount++] = Vertex(23527764, 746619008, 30740, 15, 2014576640);
    verts[vertCount++] = Vertex(23527788, 746619008, 30740, 15, 983063);
    verts[vertCount++] = Vertex(24314196, 746619008, 30740, 15, 2816);
    verts[vertCount++] = Vertex(23527788, 746619008, 30740, 15, 983063);
    verts[vertCount++] = Vertex(24314196, 746619008, 30740, 15, 2816);
    verts[vertCount++] = Vertex(24314220, 746619008, 30740, 15, 2839);

    //   verts[vertCount++] = Vertex(25034952, 746619008, 30736, 14, 2014314496);
    //   verts[vertCount++] = Vertex(25035192, 746619008, 30736, 14, 917743);
    //   verts[vertCount++] = Vertex(28967112, 746619008, 30736, 14, 15104);
    //   verts[vertCount++] = Vertex(25035192, 746619008, 30736, 14, 917743);
    //   verts[vertCount++] = Vertex(28967112, 746619008, 30736, 14, 15104);
    //   verts[vertCount++] = Vertex(28967352, 746619008, 30736, 14, 15343);
    //
    //   verts[vertCount++] = Vertex(3670216, 746619008, 30732, 13, 2014052352);
    //   verts[vertCount++] = Vertex(3670456, 746619008, 30732, 13, 852207);
    //   verts[vertCount++] = Vertex(6815944, 746619008, 30732, 13, 12032);
    //   verts[vertCount++] = Vertex(3670456, 746619008, 30732, 13, 852207);
    //   verts[vertCount++] = Vertex(6815944, 746619008, 30732, 13, 12032);
    //   verts[vertCount++] = Vertex(6816184, 746619008, 30732, 13, 12271);
    //
    //   verts[vertCount++] = Vertex(23527764, 746619008, 30740, 15, 2014576640);
    //   verts[vertCount++] = Vertex(23527788, 746619008, 30740, 15, 983063);
    //   verts[vertCount++] = Vertex(24314196, 746619008, 30740, 15, 2816);
    //   verts[vertCount++] = Vertex(23527788, 746619008, 30740, 15, 983063);
    //   verts[vertCount++] = Vertex(24314196, 746619008, 30740, 15, 2816);
    //   verts[vertCount++] = Vertex(24314220, 746619008, 30740, 15, 2839);

    glBufferSubData(GL_ARRAY_BUFFER, vertOffset, sizeof(Vertex) * vertCount, &verts[0]);
    vertOffset = vertCount * sizeof(Vertex);

    while (!glfwWindowShouldClose(window)) {
        clear();
        shader.activate();

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUniform1i(glGetUniformLocation(shader.m_programID, "u_vramTex"), 0);
        glUniform4i(glGetUniformLocation(shader.m_programID, "u_texWindow"), 0, 0, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, vertCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}
