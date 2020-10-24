#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"
#include <string>

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	bool EnableGravity = false;
	bool landed = false;
	glm::vec2 ThrowSpeed;
	glm::vec2 ThrowPosition;
	glm::vec2 InitialPos;
	glm::vec2 FinalPos;
	glm::vec2 gravity = glm::vec2(0, 9.8f);
	void launch();
	std::string displayInfo(int ind);
	float TravelDistX();
private:
	float length_travel;
	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */