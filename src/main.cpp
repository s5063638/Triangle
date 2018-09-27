#include <SDL2/SDL.h>

#include <exception>
#include <GL/glew.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Define triangle points
const GLfloat positionsOne[] = {
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
};

const GLfloat positionsTwo[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
};

const GLchar *vertexShaderSrc =
	"attribute vec3 in_Position;              "\
	"                                         "\
	"void main()                              "\
	"{                                        "\
	"	gl_Position = vec4(in_Position, 1.0); "\
	"}                                        ";

const GLchar *fragmentShaderSrc =
	"void main()                         "\
	"{					                 "\
	"	gl_FragColor = vec4(0, 1, 1, 1); "\
	"}									 ";

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

  GLuint positionsOneVboId = 0;
  GLuint positionsTwoVboId = 0;

  //Create a new VBO on the GPU and bind it
  glGenBuffers(1, &positionsOneVboId);

  if (!positionsOneVboId)
  {
	  throw std::exception();
  }

  glBindBuffer(GL_ARRAY_BUFFER, positionsOneVboId);

  //Upload a copy of the data from memory into the new VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(positionsOne), positionsOne, GL_STATIC_DRAW);

  //Reset the state
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  //The same for the second buffer
  glGenBuffers(1, &positionsTwoVboId);
  
  if (!positionsTwoVboId)
  {
	  throw std::exception();
  }

  glBindBuffer(GL_ARRAY_BUFFER, positionsTwoVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positionsTwo), positionsTwo, GL_STATIC_DRAW);

  GLuint vaoId = 0;

  //Create a new VAO on the GPU and bind it
  glGenVertexArrays(1, &vaoId);

  if (!vaoId)
  {
	  throw std::exception();
  }

  glBindVertexArray(vaoId);

  //Bind the position VBO, assign it to the position 0 on the bound VAO and flag it to be used
  glBindBuffer(GL_ARRAY_BUFFER, positionsOneVboId);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

  glEnableVertexAttribArray(0);

  //Reset the state
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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

  //Perform the link and check for failure
  glLinkProgram(programId);
  glGetProgramiv(programId, GL_LINK_STATUS, &success);

  if (!success)
  {
	  throw std::exception();
  }

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

	  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	  glClear(GL_COLOR_BUFFER_BIT);

	  //Instruct OpenGL to use our shader program and our VAO
	  glUseProgram(programId);
	  glBindVertexArray(vaoId);

	  //Draw 3 vertices
	  glDrawArrays(GL_TRIANGLES, 0, 3);
	  glDrawArrays(GL_TRIANGLES, 3, 6);
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