#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player
{
private:
	glm::vec4 position;
	glm::vec4 rotation;
	glm::vec4 forward;
public:
	Player();
	~Player();
	void Move(glm::vec3 _movementVec);
};

#endif // !_PLAYER_H_