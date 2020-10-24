#pragma once
#ifndef __BOX__
#define __BOX__

#include "DisplayObject.h"
#include <string>

class Box final : public DisplayObject
{
public:
	Box();
	~Box();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	bool EnableGravity = true;
	bool landed = false;
	glm::vec2 ThrowSpeed;
	glm::vec2 ThrowPosition;
	glm::vec2 InitialPos;
	glm::vec2 FinalPos;
	glm::vec2 gravity = glm::vec2(0, 9.8f);
	void launch();
	std::string displayInfo(int ind);
	float TravelDistX();
	float deceleration = -41.16f;
	float rotate = 0.0f;
	float velMag = 0.0f;
	float exitVel = 0.0f;

private:
	float length_travel;
	void m_move();
	void m_checkBounds();
	void m_reset();
};

#endif // !__BOX__