#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>  // include glad to get all the required OpenGL headers

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glm/ext/matrix_float4x4.hpp"

using namespace std;

class Shader {
 public:
  // the program ID
  unsigned int ID;

  // constructor reads and builds the shader
  Shader(const char *vertexPath, const char *fragmentPath);
  // use/activate the shader
  void use();
  // utility uniform functions
  void setBool(const string &name, bool value) const;
  void setInt(const string &name, int value) const;
  void setFloat(const string &name, float value) const;
  void setMat4(const string &name, const glm::mat4 &mat) const;
  void setVec4(const string &name, const glm::vec4 &vec) const;
};

#endif
