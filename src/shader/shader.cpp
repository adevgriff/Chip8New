#include "shader.hpp"

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
{
   int success;
   char infoLog[512];

   GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
   GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

   id = glCreateProgram();
   glAttachShader(id, vertexShader);
   glAttachShader(id, fragShader);
   glLinkProgram(id);

   // catch errors
   glGetProgramiv(id, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(id, 512, NULL, infoLog);
      std::cout << "Linking error with " << id << ":"
                << infoLog << std::endl;
   }

   glDeleteShader(vertexShader);
   glDeleteShader(fragShader);
}
void Shader::activate()
{
   glUseProgram(id);
}

// utility functions
std::string Shader::loadShaderSrc(const char *filepath)
{
   std::ifstream file;
   std::stringstream buf;

   std::string ret = "";

   file.open(filepath);
   if (file.is_open())
   {
      buf << file.rdbuf();
      ret = buf.str();
   }
   else
   {
      std::cout << "could not open " << filepath << std::endl;
   }

   file.close();
   return ret;
}
GLuint Shader::compileShader(const char *filepath, GLenum type)
{
   int success;
   char infoLog[512];

   GLuint ret = glCreateShader(type);
   std::string shaderSrc = loadShaderSrc(filepath);
   const GLchar *shader = shaderSrc.c_str();
   glShaderSource(ret, 1, &shader, NULL);
   glCompileShader(ret);

   // catch error
   glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(ret, 512, NULL, infoLog);
      std::cout << "Error with fragment shader 0 comp.: " << std::endl
                << infoLog << std::endl;
   }

   return ret;
}
// glUniform4f(vertexColorLoc, 0.0f, 0.0f, blueValue, 1.0f);
void Shader::setBool(const std::string &name, bool value)
{
   glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value)
{
   glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value)
{
   glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set4Float(const std::string &name, float v1, float v2, float v3, float v4)
{
   glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}