#include "Target.h"
#include "TextureManager.h"
#include <string>


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/detonator1.png", "circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(-100.0f, -100.0f);
	InitialPos = getTransform()->position;
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

void Target::launch()
{
	getTransform()->position = ThrowPosition;
	InitialPos = ThrowPosition;
	FinalPos = getTransform()->position;
	getRigidBody()->velocity = ThrowSpeed;
	landed = false;
}

std::string Target::displayInfo(int ind)
{
	std::string XPosM = "Detonator X Pos: " + std::to_string(getTransform()->position.x) + " m";
	std::string YPosM = " and Y Pos: " + std::to_string(600.0f - getTransform()->position.y) + " m";

	std::string XVelM = "Detonator X Vel: " + std::to_string(getRigidBody()->velocity.x) + " m/s";
	std::string YVelM = " and Y Vel: " + std::to_string(-getRigidBody()->velocity.y) + " m/s";

	std::string XAccM = "Detonator X Acc: " + std::to_string(getRigidBody()->acceleration.x) + " m/s^2";
	std::string YAccM = " and Y Acc: " + std::to_string(getRigidBody()->acceleration.y) + " m/s^2";

	std::string TravelDistM = "Horizontal Distance Travelled: " + std::to_string(TravelDistX()) + " m";

	std::string Message;

	switch (ind)
	{
	case 1: //Position
		Message = XPosM + YPosM;
		break;
	case 2: //Velocity
		Message = XVelM + YVelM;
		break;
	case 3: //Acceleration
		Message = XAccM + YAccM;
		break;
	case 4: //Travel distance or how far it lands
		Message = TravelDistM;
		break;
	}
	return Message;
}

float Target::TravelDistX()
{
	if (landed)
	{
		return (FinalPos.x - InitialPos.x);
	}
	else
	{
		return (getTransform()->position.x - InitialPos.x);
	}
}

void Target::m_move()
{
	float deltaTime = 1.0f / 60.0f;

	if (EnableGravity)
	{
		if (getTransform()->position.y <= 450.0f)
		{
			getRigidBody()->acceleration = gravity;
			getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
		}
		else //When hitting the floor, will roll on floor
		{
			getRigidBody()->acceleration.y = 0.0f;
			getRigidBody()->velocity.y = 0.0f;
			if (!landed)
			{
				FinalPos = getTransform()->position;
				landed = true;
			}
		}
	}
	else
	{
		//getRigidBody()->velocity.y = 0; //No acceleration = no force acting against an already moving object, so it will stay in whatever motion it is in
		getRigidBody()->acceleration.y = 0;
	}

	getTransform()->position += getRigidBody()->velocity * deltaTime; //multiply it with some sort of pixel per meter variable

}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}
