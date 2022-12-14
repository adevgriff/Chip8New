#include "ib.hpp"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
{
   this->indicesCount = count;
   glGenBuffers(1, &bufferID);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer()
{
   glDeleteBuffers(1, &bufferID);
}

void IndexBuffer::Bind() const
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}
void IndexBuffer::Unbind() const
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}