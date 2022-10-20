#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
private:
   unsigned int bufferID;
   unsigned int indicesCount;

public:
   IndexBuffer(const unsigned int *data, unsigned int count);
   ~IndexBuffer();

   void Bind() const;
   void Unbind() const;

   inline unsigned int GetCount() const { return indicesCount; }
};

#endif