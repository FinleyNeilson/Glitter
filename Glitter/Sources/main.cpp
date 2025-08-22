// Local Headers
#include "Shader.h"
#include "glitter.hpp"
#include "stb_image.h"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const std::string &path);

int main(int argc, char *argv[]) {
  (void)argc, (void)argv;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *mWindow =
      glfwCreateWindow(mWidth, mHeight, "Hello World!", nullptr, nullptr);

  if (mWindow == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(mWindow);
  glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return -1;
  }

  // ..:: Initialization code :: ..

  Shader shader("../Glitter/Shaders/vertexShader.vert",
                "../Glitter/Shaders/fragmentShader.frag");

  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int indices[] = {
      0, 1, 2, 2, 3, 0,
  };

  unsigned int texture1 = loadTexture("container.jpg");
  unsigned int texture2 = loadTexture("awesomeface.png");

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s),
  // and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)0); // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float))); // color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float))); // texCoords
  glEnableVertexAttribArray(2);

  glEnableVertexAttribArray(1);

  // ..:: Rendering Loop :: ..
  while (glfwWindowShouldClose(mWindow) == false) {
    processInput(mWindow);

    float time = glfwGetTime();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));

    float greenValue = (sin(time) / 2.0f) + 0.5f;

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setMat4("uModel", model);
    shader.setVec4("ourColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Flip Buffers and Draw
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
  }
  glfwTerminate();
  return EXIT_SUCCESS;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

unsigned int loadTexture(const std::string &path) {
  unsigned int textureID;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load image
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true); // optional: flip vertically
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    GLenum format = GL_RGB;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture: " << path << std::endl;
  }
  stbi_image_free(data);

  return textureID;
}
