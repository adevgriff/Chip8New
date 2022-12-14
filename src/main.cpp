#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

#include "chip/chip_encodings.hpp"
#include "shader/shader.hpp"
#include "opglBuffers/vb.hpp"
#include "opglBuffers/ib.hpp"
#include "constants.hpp"
#include "display/display.hpp"
#include "chip/ram.hpp"
#include "chip/cpu.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

int main()
{
   Display display;
   Ram ram;
   ram.loadProgram("Most Dangerous Game [Peter Maruhnic].ch8", 512);
   CPU cpu(&ram, &display);
   cpu.setStart(START_OF_PROGRAM);
   //---------------------------------------------------glfw setup----------------------------------------------------------------------
   glfwInit();

   // require version 3.3
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// workaround for working on an apple device
#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // used to make code complient on OS X
#endif

   // create window
   GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE, "Chip8", NULL, NULL);
   if (window == NULL)
   {
      std::cout << "could not create window" << std::endl;
      glfwDestroyWindow(window);
      glfwTerminate();
      return -1;
   }

   // set window context
   glfwMakeContextCurrent(window);
   glfwSetWindowUserPointer(window, &cpu);

   // initalize glad
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "failed to load GLAD" << std::endl;
      glfwDestroyWindow(window);
      glfwTerminate();
      return -1;
   }

   // set callbacks
   glfwSetKeyCallback(window, key_callback);
   glViewport(0, 0, SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   //-------------------------------------------------------------------------------------------------------------------------------------

   //--------------------------------------------------Compile Shaders--------------------------------------------------------------------
   Shader shader("../assets/vertex_core.glsl", "../assets/fragment_core.glsl");
   //-------------------------------------------------------------------------------------------------------------------------------------

   //---------------------------------------------------Setup Containers------------------------------------------------------------------

   // Vertex array
   float vertices[SCREEN_VERTICES];
   setupDisplayVertices(vertices);

   unsigned int indices[TOTAL_INDICES];
   setupDisplayIndeices(indices);

   // VAO, VBO, EBO creation
   unsigned int VAO;
   glGenVertexArrays(1, &VAO);

   // Bind VAO
   glBindVertexArray(VAO);

   // Bind VBO
   VertexBuffer *VBO = new VertexBuffer(vertices, sizeof(vertices));

   IndexBuffer *EBO = new IndexBuffer(indices, 6 * SCREEN_WIDTH * SCREEN_HEIGHT);

   // Set attribute pointers
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);

   //-------------------------------------------------------------------------------------------------------------------------------------

   display.clearScreen();
   double currentFrame = glfwGetTime();
   double lastFrame = currentFrame;
   double deltaTime;
   double accumulator = 0;
   // six hundreths of a second here so I can change while I am working on this
   // move this as a constant in constants TODO
   double speed = 6.0 / 1000.0;

   // main window loop
   while (!glfwWindowShouldClose(window))
   {
      // get delta time
      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      accumulator += deltaTime;

      // process input

      // render
      glClearColor(0.1f, 0.5f, 0.4f, 1.0f); // set color for clearing window to black
      glClear(GL_COLOR_BUFFER_BIT);         // clear window

      // run one execute cycle
      if (accumulator >= speed)
      {
         srand(glfwGetTime());
         cpu.execute();
         accumulator -= speed;
      }
      //------------------------draw screen---------------------------
      // bind vertex array
      glBindVertexArray(VAO);
      shader.activate();
      display.drawScreen();
      //-------------------------------------------------------
      glfwSwapBuffers(window);

      // swap buffers
      glfwPollEvents();
   }

   // make sure everything is cleaned up
   delete (VBO);
   delete (EBO);
   glfwDestroyWindow(window);
   glfwTerminate();

   return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
   glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
   switch (key)
   {
   case GLFW_KEY_1:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x01, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_2:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x02, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_3:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x03, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_Q:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x04, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_W:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x05, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_E:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x06, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_A:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x07, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_D:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x09, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_F:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x0E, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_Z:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x0A, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_X:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x00, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_C:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x0B, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_S:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x08, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_V:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x0F, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_R:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x0D, (action == GLFW_PRESS));
      break;
   case GLFW_KEY_4:
      reinterpret_cast<CPU *>(glfwGetWindowUserPointer(window))->keyUpdate(0x0C, (action == GLFW_PRESS));
      break;
   }
}