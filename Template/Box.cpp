#include "Box.h"
#include "TextureManager.h"

Box::Box()
{
	TextureManager::Instance()->load("../Assets/textures/box.png", "box");

	const auto size = TextureManager::Instance()->getTextureSize("box");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(-100.0f, -100.0f);
	InitialPos = getTransform()->position;
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(BOX);
}

Box::~Box() = default;

void Box::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("box", x, y, rotate, 255, true);
}

void Box::update()
{
	if (getTransform()->position.y < 442.0f)
	{
		landed = false;
	}
	m_move();
	velMag = sqrtf(getRigidBody()->velocity.x * getRigidBody()->velocity.x + getRigidBody()->velocity.y * getRigidBody()->velocity.y);
	m_checkBounds();
}

void Box::clean()
{
}

void Box::launch()
{
	//getTransform()->position = ThrowPosition;
	//InitialPos = ThrowPosition;
	//FinalPos = getTransform()->position;
	//getRigidBody()->velocity = ThrowSpeed;
	landed = false;
}

std::string Box::
Info(int ind)
{
	std::string Pos_Str = "PosX = " + std::to_string(getTransform()->position.x / 10.0f) + "m PosY = " + std::to_string((443.0f - getTransform()->position.y) / 10.0f) + "m";
	std::string Vel_Str = "VelX = " + std::to_string(getRigidBody()->velocity.x / 10.0f) + "m/s VelY = " + std::to_string(getRigidBody()->velocity.y / 10.0f) + "m/s";
	std::string Acc_Str = "AccX = " + std::to_string(getRigidBody()->acceleration.x / 10.0f) + "m/s^2 AccY = " + std::to_string(getRigidBody()->acceleration.y / 10.0f) + "m/s^2";
	std::string Travel_Str = "After Ramp Slide Distance = " + std::to_string((FinalPos.x - InitialPos.x) / 10.0f) + "m";

	switch (ind)
	{
	case 1: //Position
		return Pos_Str;
		break;
	case 2: //Velocity
		return Vel_Str;
		break;
	case 3: //Acceleration
		return Acc_Str;
		break;
	case 4: //Distance Slid
		return Travel_Str;
		break;
	}
	//return std::string();
}

float Box::TravelDistX()
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

void Box::m_move()
{
	float deltaTime = 1.0f / 60.0f;

	if (EnableGravity)
	{
		if (getTransform()->position.y <= 443.0f) //change the value
		{
			//getRigidBody()->acceleration = gravity;
		}
		else //When hitting the floor, will roll on floor
		{
			rotate = 0.0f;
			getRigidBody()->acceleration.y = 0.0f;
			getRigidBody()->velocity.y = 0.0f;
			if (!landed)
			{
				InitialPos = getTransform()->position;
				getRigidBody()->velocity.x = velMag;
				exitVel = velMag;
				landed = true;
			}
			if (getRigidBody()->velocity.x > 0)
			{
				getRigidBody()->acceleration.x = deceleration;
			}
			else
			{
				getRigidBody()->acceleration.x = 0.0f;
				getRigidBody()->velocity.x = 0.0f;
			}
			FinalPos = getTransform()->position;
		}
		getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	}
	else
	{
		getRigidBody()->acceleration.y = 0;
	}

	getTransform()->position += getRigidBody()->velocity * deltaTime; //multiply it with some sort of pixel per meter variable
}

void Box::m_checkBounds()
{
}

void Box::m_reset()
{
}
