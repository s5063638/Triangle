#include <SDL2/SDL.h>

#include <exception>
#include <GL/glew.h>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Define triangle points
const GLfloat positionsOne[] = {
	0.0f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
};

const GLfloat colors[] = {
	0.4f, 0.0f, 0.4, 1.0f,
	0.9f, 0.7f, 1.0f, 1.0f,
	0.7f, 0.2f, 0.9f, 1.0f
};

const GLchar *vertexShaderSrc =
	"attribute vec3 in_Position; "\
	"attribute vec4 in_Color; "\
	""\
	"varying vec4 ex_Color; "\
	""\
	"void main() "\
	"{ "\
	"	gl_Position = vec4(in_Position, 1.0); "\
	"	ex_Color = in_Color; "\
	"} ";

const GLchar *fragmentShaderSrc =
"varying vec4 ex_Color; "\
"void main() "\
"{ "\
"	gl_FragColor = ex_Color; "\
"} "\
"";

int main(int argc, char *argv[])
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw std::exception();
  }

  SDL_Window *window = SDL_CreateWindow("Triangle",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (!SDL_GL_CreateContext(window))
  {
	  throw std::exception();
  }

  if (glewInit() != GLEW_OK)
  {
	  throw std::exception();
  }

  VertexBuffer* positions = new VertexBuffer();
  positions->Add(glm::vec3(-0.5f, 0.5f, 0.0f));
  positions->Add(glm::vec3(-0.5f, -0.5f, 0.0f));
  positions->Add(glm::vec3(0.5f, -0.5f, 0.0f));

  VertexBuffer* colors = new VertexBuffer();
  colors->Add(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  colors->Add(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  colors->Add(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

  GLuint vaoId = 0;

  //Create a new VAO on the GPU and bind it
  glGenVertexArrays(1, &vaoId);

  if (!vaoId)
  {
	  throw std::exception();
  }

  glBindVertexArray(vaoId);

  //Bind the position VBO, assign it to the position 0 on the bound VAO and flag it to be used
  glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

  glEnableVertexAttribArray(0);

  //Reset the state
  glBindBuffer(GL_ARRAY_BUFFER, 0);
 
  //Create a new vertex shader, attach source code, compile it and check for errors.
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShaderId);
  GLint success = 0;
  glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

  if (!success)
  {
	  throw std::exception();
  }

  //Create a new fragment shader, attach source code, compile it and
  //check for errors
  GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShaderId);
  glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

  if (!success)
  {
	  throw std::exception();
  }

  //Create new shader program and attach our shader objects
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertexShaderId);
  glAttachShader(programId, fragmentShaderId);

  //Ensure the VAO "position" attribute stream gets set as the first position
  //during the link.
  glBindAttribLocation(programId, 0, "in_Position");
  glBindAttribLocation(programId, 1, "in_Color");

  //Perform the link and check for failure
  glLinkProgram(programId);
  glGetProgramiv(programId, GL_LINK_STATUS, &success);

  if (!success)
  {
	  throw std::exception();
  }

  //Uniform Stuff
  //GLint colorUniformId = glGetUniformLocation(programId, "in_Color");

  //if (colorUniformId == -1)
  //{
	 // throw std::exception();
  //}

  //Detatch and destroy the shader objects. These are no longer needed
  //because we now have a complete shader program
  glDetachShader(programId, vertexShaderId);
  glDeleteShader(vertexShaderId);
  glDetachShader(programId, fragmentShaderId);
  glDeleteShader(fragmentShaderId);
  
  bool quit = false;

  while(!quit)
  {
    SDL_Event event = {0};

    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        quit = true;
      }

	  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	  glClear(GL_COLOR_BUFFER_BIT);

	  //Instruct OpenGL to use our shader program and our VAO
	  glUseProgram(programId);
	  //glUniform4f(colorUniformId, 0, 1, 0, 1); //Set color from uniform
	  glBindVertexArray(vaoId);

	  //Draw 3 vertices
	  glDrawArrays(GL_TRIANGLES, 0, 3);

	  //Reset the state
	  glBindVertexArray(0);
	  glUseProgram(0);

	  SDL_GL_SwapWindow(window);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}