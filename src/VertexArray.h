#pragma once
#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <GL/glew.h>
#include <vector>

class VertexBuffer;

class VertexArray
{
private:
	GLuint id;
	std::vector<VertexBuffer*> buffers;
	bool dirty;
public:
	VertexArray();

	void SetBuffer(std::string _attribute, VertexBuffer* _buffer);

	int GetVertexCount();
	GLuint GetId();
};

#endif // !_VERTEX_ARRAY_H_