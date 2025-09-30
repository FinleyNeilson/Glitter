// Local Headers
#include "Shader.h"
#include "glitter.hpp"
#include "stb_image.h"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const std::string &path);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main(int argc, char *argv[]) {
  (void)argc, (void)argv;

  glfwInit();
  // Set opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create the window
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World!", nullptr, nullptr);

  // Throw error if failed to create window
  if (window == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    return EXIT_FAILURE;
  }

  // This connects GLAD to opengl
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  // tell GLFW to capture our mouse
  // This breaks WSL for some reason
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return -1;
  }

  // ..:: Initialization code ::..

  // Global openGL state

  glEnable(GL_DEPTH_TEST);

  // Load my shaders
  Shader mainShader("../Glitter/Shaders/vertexShader.vert",
                    "../Glitter/Shaders/fragmentShader.frag");

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  unsigned int indices[] = {
      0, 1, 2, 2, 3, 0,
  };

  glm::vec3 cubePositions[] = {
      glm::vec3(2.0f, 3.0f, -10.0f),  glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  unsigned int texture1 = loadTexture("container.jpg");
  unsigned int texture2 = loadTexture("awesomeface.png");

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // This stores the vertex config, as you can basically put whatever data
  // you like in the VBO, we need to store how we intepret it. This is set up
  // with glVertexAttribPointer below
  glBindVertexArray(VAO);

  // In this we store the actual vertices
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Here we store the indices for the vertices. This is to avoid duplicate
  // storage of vertices as this stores which vertices to connect into triangles
  // we only need each vertex stored once.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Sets how to intepret vertex data here we have index 1, thats size 3, its a
  // float, we aren't normalizing it, and the size of the whole vertex is 8
  // floats and its offset is 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Here its index is 1 size 2 and offset is 3 floats
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float))); // texCoords
  glEnableVertexAttribArray(1);

  // Model * View * Projection

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

  // Send info to the shaders
  mainShader.use();
  mainShader.setMat4("model", model);
  mainShader.setMat4("view", view);
  mainShader.setMat4("projection", projection);
  mainShader.setInt("texture1", 0);
  mainShader.setInt("texture2", 1);

  // ..:: Rendering Loop ::..
  while (glfwWindowShouldClose(window) == false) {
    processInput(window);
    // Color and texture

    // Changing green color
    float time = glfwGetTime();
    float greenValue = (sin(time) / 2.0f) + 0.5f;

    // Send the green color to the fragment shader.
    mainShader.use();
    mainShader.setVec4("uColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Background Fill Color
    glClearColor(0.95f, 0.80f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    mainShader.use();

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    mainShader.setMat4("projection", projection);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mainShader.setMat4("view", view);

    // DRAW

    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 30.0f * i;

      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      mainShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // this uses EBO
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return EXIT_SUCCESS;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.5f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  // Close the application
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // Moving the camera
  float cameraSpeed = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
