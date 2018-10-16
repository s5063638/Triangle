#include <SDL2/SDL.h>

#include <exception>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

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

  //Load-In Texture
  int w = 0;
  int h = 0;
  int channels = 0;

  unsigned char *data = stbi_load("rock.jpg", &w, &h, &channels, 4);

  if (!data)
  {
	  throw std::exception();
  }

  //Create and bind texture
  GLuint textureId = 0;
  glGenTextures(1, &textureId);

  if (!textureId)
  {
	  throw std::exception();
  }

  glBindTexture(GL_TEXTURE_2D, textureId);

  //Upload the image data to the bound texture unit in the GPU
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  //Free the loaded data because we now have a copy on the GPU
  free(data);

  //Generate Mipmap so the texture can be mapped correctly
  glGenerateMipmap(GL_TEXTURE_2D);
  
  //Unbind the texture because we are done operating on it
  glBindTexture(GL_TEXTURE_2D, 0);

  VertexBuffer* positions = new VertexBuffer();
  positions->Add(glm::vec3(0.0f, 0.5f, 0.0f));
  positions->Add(glm::vec3(-0.5f, -0.5f, 0.0f));
  positions->Add(glm::vec3(0.5f, -0.5f, 0.0f));

  VertexBuffer* texCoords = new VertexBuffer();
  texCoords->Add(glm::vec2(0.5f, 0.0f));
  texCoords->Add(glm::vec2(0.0f, 1.0f));
  texCoords->Add(glm::vec2(1.0f, 1.0f));

  VertexArray* shape = new VertexArray();
  shape->SetBuffer("in_Position", positions);
  shape->SetBuffer("in_TexCoord", texCoords);

  ShaderProgram* shaderProgram = new ShaderProgram("../shaders/simple.vert", "../shaders/simple.frag");
  
  float angle = 0.0f;

  bool quit = false;

  while(!quit)
  {
    SDL_Event event = {0};

    while(SDL_PollEvent(&event))
    {
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
    }

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw with perspective projection matrix
	shaderProgram->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f));

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

	shaderProgram->SetUniform("in_Model", model);

	//Set Texture
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureId);
	shaderProgram->SetUniform("in_Texture", 1);

	shaderProgram->Draw(shape);

	//Draw with orthographic projection matrix
	model = glm::mat4(1.0f);

	shaderProgram->SetUniform("in_Projection", glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.0f, 1.0f));

	model = glm::translate(model, glm::vec3(100, WINDOW_HEIGHT - 100, 0));
	model = glm::scale(model, glm::vec3(100, 100, 1));

	shaderProgram->SetUniform("in_Model", model);

	//Set Texture
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureId);
	shaderProgram->SetUniform("in_Texture", 1);
	
	shaderProgram->Draw(shape);

	angle += 1.0f;

	SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}