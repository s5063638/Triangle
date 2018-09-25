#include <SDL2/SDL.h>

#include <exception>
#include <GL/glew.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Define triangle points
const GLfloat positions[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

const GLchar *vertexShaderSrc =
	"attribute vec3 in_Position;              "\
	"                                         "\
	"void main()                              "\
	"{                                        "\
	"	gl_Position = vec4(in_Position, 1.0); "\
	"}                                        ";

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

  GLuint positionsVboId = 0;

  //Create a new VBO on the GPU and bind it
  glGenBuffers(1, &positionsVboId);

  if (!positionsVboId)
  {
	  throw std::exception();
  }

  glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

  //Upload a copy of the data from memory into the new VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

  //Reset the state
  glBindBuffer(GL_ARRAY_BUFFER, 0);

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
  glBindVertexArray(0);

  //Create a new vertext shader, attach source code, compile it and check for errors.
  GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShaderId);
  GLint success = 0;
  glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

  if (!success)
  {
	  throw std::exception();
  }

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
	  SDL_GL_SwapWindow(window);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}