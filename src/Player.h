#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

enum Direction
{
	FORWARD,
	BACK,
	LEFT,
	RIGHT
};

class Player
{
private:
	glm::vec3 position; //Player position
	glm::vec3 rotation; //Player rotation
	glm::vec3 forward; //Forward vector
	glm::vec3 right; //Right vector
public:
	Player();
	~Player();
	void Move(Direction _dir);
	glm::vec3 GetPos();
};

#endif // !_PLAYER_H_