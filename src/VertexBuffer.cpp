#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	components = 0;
	dirty = false;

	glGenBuffers(1, &id);

	if (!id)
	{
		throw std::exception();
	}
}
void VertexBuffer::Add(glm::vec2 _value)
{
	if (components != 2 && components != 0)
	{
		throw std::exception();
	}

	data.push_back(_value.x);
	data.push_back(_value.y);
	components = 2;
	dirty = true;
}
void VertexBuffer::Add(glm::vec3 _value)
{
	if (components != 3 && components != 0)
	{
		throw std::exception();
	}

	data.push_back(_value.x);
	data.push_back(_value.y);
	data.push_back(_value.z);
	components = 3;
	dirty = true;
}
void VertexBuffer::Add(glm::vec4 _value)
{
	if (components != 4 && components != 0)
	{
		throw std::exception();
	}

	data.push_back(_value.x);
	data.push_back(_value.y);
	data.push_back(_value.z);
	data.push_back(_value.w);
	components = 4;
	dirty = true;
}
int VertexBuffer::GetComponents()
{
	return components;
}
int VertexBuffer::GetDataSize()
{
	return data.size();
}
GLuint VertexBuffer::GetId()
{
	if (dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		dirty = false;
	}

	return id;
}