#pragma once
#ifndef  _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class VertexBuffer
{
private:
	GLuint id;
	int components;
	std::vector <GLfloat> data;
	bool dirty;	
public:
	VertexBuffer();

	void Add(glm::vec3 _value);
	void Add(glm::vec4 _value);

	int GetComponents();
	int GetDataSize();
	GLuint GetId();
};

#endif // ! _VERTEX_BUFFER_H_