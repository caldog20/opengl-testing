#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void init() {
    glfwSetErrorCallback(
            [](int error, const char* description) { fmt::print("GLFW Error {}: {}\n", error, description); });

    // Init GLFW
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW. Quitting\n");
}





int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
