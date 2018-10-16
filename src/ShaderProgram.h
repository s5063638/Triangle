#pragma once
#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>

class VertexArray;

class ShaderProgram
{
private:
	GLuint id;
	std::string vertShader;
	std::string fragShader;

public:
	ShaderProgram();
	ShaderProgram(std::string _vert, std::string _frag);

	void Draw(VertexArray* _vertexArray);

	void SetUniform(std::string _uniform, glm::mat4 _value);
	void SetUniform(std::string _uniform, float value);
	void SetUniform(std::string _uniform, int value);

	GLuint GetId();
};

#endif // !_SHADER_PROGRAM_H_