#include "Player.h"

Player::Player() 
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 t(1.0f); //Create Arbitary Matrix

	t = glm::rotate(t, glm::radians(rotation.y), glm::vec3(0, 1, 0)); //rotate it by the camera's y rotation

	t = glm::translate(t, glm::vec3(0.0f, 0.0f, 1.0f)); //Move by movement vector

	forward = t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //apply to the initial position

	forward = glm::normalize(forward); //Normalize to get the unit vector

	right = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);

	right = glm::normalize(right);

	//Change forward vector to allow up and down movement
	//forward = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), forward);
}
Player::~Player() {}
void Player::Move(Direction _dir, float _deltaT)
{
	switch (_dir)
	{
	case FORWARD:
		position -= forward * 5.0f * _deltaT;
		break;
	case BACK:
		position += forward * 5.0f * _deltaT;
		break;
	case LEFT:
		position -= right * 5.0f * _deltaT;
		break;
	case RIGHT:
		position += right * 5.0f * _deltaT;
		break;
	}
}
glm::vec3 Player::GetPos()
{
	return position;
}