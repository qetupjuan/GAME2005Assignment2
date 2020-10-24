#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "ship.h"
#include "Box.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	//Player* m_pPlayer;
	//bool m_playerFacingRight;
	//Target* m_pGranade;
	//Ship* m_pTrooper;
	Box* m_pBox;
	glm::vec2 Rise_Run;
	float box_mass = 12.8f;
	float coeff_frict = 0.42f;
	float grav = 98.1f;//10 pixels per meter
	void Calc_Box_Dynamics();


	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
};

#endif /* defined (__PLAY_SCENE__) */