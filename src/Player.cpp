#include "Player.h"

Player::Player() 
{
	position = glm::vec4(0, 0, 0, 1);
	rotation = glm::vec4(0, 0, 0, 1);
}
Player::~Player() 
{

}
void Player::Move(glm::vec3 _movementVec)
{
	glm::mat4 t(1.0f); //Create Arbitary Matrix

	t = glm::rotate(t, glm::radians(rotation.y), glm::vec3(0, 1, 0)); //rotate it by the camera's y rotation

	t = glm::translate(t, _movementVec); //Move by movement vector
	
	forward = t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //apply to the initial position

	forward = glm::normalize(forward); //Normalize to get the unit vector

	position += forward;
}