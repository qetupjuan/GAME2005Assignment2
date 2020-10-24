#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("bg", 400, 200, 0.0, 255, true);
	TextureManager::Instance()->draw("fg", 400, 575, 0.0, 255, true);

	//Ramp Drawing
	Util::DrawLine(glm::vec2(0.0f, 475.0f), glm::vec2(0.0f, 475.0f - Rise_Run.y));
	Util::DrawLine(glm::vec2(0.0f, 475.0f - Rise_Run.y), glm::vec2(Rise_Run.x, 475.0f));
	Util::DrawLine(glm::vec2(Rise_Run.x, 475.0f), glm::vec2(0.0f, 475.0f));

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene::update()
{
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				//m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				//m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				//m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				//m_playerFacingRight = false;
			}
			else
			{
				//if (m_playerFacingRight)
				//{
				//	m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				//}
				//else
				//{
				//	m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				//}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			//m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			//m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			//m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			//m_playerFacingRight = true;
		}
		else
		{
			//if (m_playerFacingRight)
			//{
			//	m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			//}
			//else
			//{
			//	m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			//}
		}
	}


	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "bg");
	TextureManager::Instance()->load("../Assets/textures/foreground.png", "fg");

	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Player Sprite
	//m_pPlayer = new Player();
	//addChild(m_pPlayer);
	//m_playerFacingRight = true;

	//m_pGranade = new Target();
	//addChild(m_pGranade);

	//m_pTrooper = new Ship();
	//addChild(m_pTrooper);

	m_pBox = new Box();
	addChild(m_pBox);
	Rise_Run = glm::vec2(0.0f, 0.0f);

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 525.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 525.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 575.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Game Physics Controller", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	//if (ImGui::Button("Throw"))
	//{
	//	m_pGranade->ThrowPosition = m_pPlayer->getTransform()->position;
	//	m_pGranade->launch();
	//}

	ImGui::Separator();

	ImGui::Text("Scale approx. 10 pixels per meter");
	//ImGui::Text("Accuracy to the nearest tenth decimal due to roundings");

	if (ImGui::SliderFloat("Rise", &Rise_Run.y, 0.0f, 475.0f))
	{
	}

	if (ImGui::SliderFloat("Run", &Rise_Run.x, 0.0f, 800.0f))
	{
	}

	if (ImGui::SliderFloat("Gravity", &grav, 0.0f, 500.0f))
	{
	}

	if (ImGui::SliderFloat("Mass", &box_mass, 0.0f, 100.0f))
	{
	}

	if (ImGui::SliderFloat("Coeff. Friction", &coeff_frict, 0.0f, 1.0f))
	{
	}

	if (ImGui::Button("Set Box @ top of the ramp"))
	{
		m_pBox->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBox->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		m_pBox->getTransform()->position = glm::vec2(0.0f, 443.0f - Rise_Run.y);
	}

	if (ImGui::Button("GO"))
	{
		Calc_Box_Dynamics();
	}

	if (ImGui::Button("Assignment 2 Defaults"))
	{
		box_mass = 12.8f;
		coeff_frict = 0.42f;
		grav = 98.1f;
		Rise_Run = glm::vec2(40.0f, 30.0f); // at 10 pixels per meter
	}

	std::string Rise_String = "Rise = " + std::to_string(Rise_Run.y / 10.0f) + "m";
	std::string Run_String = "Run = " + std::to_string(Rise_Run.x / 10.0f) + "m";
	std::string Mass_String = "Box Mass = " + std::to_string(box_mass) + "kg";
	std::string Coeff_Frict_Str = "Coeff. Friction = " + std::to_string(coeff_frict);
	float Fx = box_mass * m_pBox->getRigidBody()->acceleration.x / 10.0f;
	float Fy = box_mass * m_pBox->getRigidBody()->acceleration.y / 10.0f;
	std::string Force1_Str = "Total Force Magnitude = " + std::to_string(sqrtf(Fx * Fx + Fy * Fy)) + "N";
	std::string Force_Str = "Fx = " + std::to_string(Fx) + "N Fy = " + std::to_string(Fy) + "N";
	ImGui::Text(Rise_String.c_str());
	ImGui::Text(Run_String.c_str());
	ImGui::Text(Mass_String.c_str());
	ImGui::Text(Coeff_Frict_Str.c_str());
	ImGui::Text(Force1_Str.c_str());
	ImGui::Text(Force_Str.c_str());
	ImGui::Text(m_pBox->displayInfo(1).c_str());
	ImGui::Text(m_pBox->displayInfo(2).c_str());
	ImGui::Text(m_pBox->displayInfo(3).c_str());

	//Fix
	float dist_Travel = m_pBox->exitVel * m_pBox->exitVel / (2.0f * coeff_frict * grav * 10.0f);
	std::string Trav_Str = "Sliding Distance after ramp = " + std::to_string(dist_Travel) + "m";
	ImGui::Text(Trav_Str.c_str());

	ImGui::Text(m_pBox->displayInfo(4).c_str());

	//static bool EnabledGrav = false;
	//if (ImGui::Checkbox("Gravity", &EnabledGrav))
	//{
	//	m_pGranade->EnableGravity = EnabledGrav;
	//}

	//static int xPlayerPos = m_pPlayer->getTransform()->position.x;
	//if (ImGui::SliderInt("Player Position x", &xPlayerPos, 0, 800))
	//{
	//	m_pPlayer->getTransform()->position.x = xPlayerPos;
	//	//m_pGranade->ThrowPosition = glm::vec2 (xPlayerPos, 300);
	//}

	//static int xEnemyPos = m_pTrooper->getTransform()->position.x;
	//if (ImGui::SliderInt("Stormtrooper Position x", &xEnemyPos, 0, 800))
	//{
	//	m_pTrooper->getTransform()->position.x = xEnemyPos;
	//}

	//static float ThrowAngle;
	//static float ThrowVel;
	//if (ImGui::SliderFloat2("Throw Angle (degrees) and Velocity", &ThrowAngle, 0, 180))
	//{
	//	float radi = (ThrowAngle * 3.14159f) / 180.0f;
	//	float x_vel = cos(radi) * ThrowVel;
	//	float y_vel = -sin(radi) * ThrowVel;
	//	m_pGranade->ThrowSpeed = glm::vec2(x_vel, y_vel);
	//}

	//if (ImGui::Button("Test Angle 1"))
	//{
	//	ThrowAngle = 15.8896f;
	//	ThrowVel = 95.0f;
	//	float radi = (ThrowAngle * 3.14159f) / 180.0f;
	//	float x_vel = cos(radi) * ThrowVel;
	//	float y_vel = -sin(radi) * ThrowVel;
	//	m_pGranade->ThrowSpeed = glm::vec2(x_vel, y_vel);
	//}

	//if (ImGui::Button("Test Angle 2"))
	//{
	//	ThrowAngle = 74.1104;
	//	ThrowVel = 95.0f;
	//	float radi = (ThrowAngle * 3.14159f) / 180.0f;
	//	float x_vel = cos(radi) * ThrowVel;
	//	float y_vel = -sin(radi) * ThrowVel;
	//	m_pGranade->ThrowSpeed = glm::vec2(x_vel, y_vel);
	//}

	//if (ImGui::Button("Set Trooper at 485m away from Wookie"))
	//{
	//	m_pTrooper->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 485.0f;
	//}

	//if (ImGui::Button("1G Acceleration"))
	//{
	//	m_pGranade->gravity.y = 9.8f;
	//}

	//if (ImGui::Button("2G Acceleration"))
	//{
	//	m_pGranade->gravity.y = 19.6f;
	//}

	//if (ImGui::Button("10G Acceleration"))
	//{
	//	m_pGranade->gravity.y = 98.0f;
	//}
	//
	//ImGui::Text("Granade Mass: 2.2kg");
	//ImGui::Text(m_pGranade->displayInfo(1).c_str());
	//ImGui::Text(m_pGranade->displayInfo(2).c_str());
	//ImGui::Text(m_pGranade->displayInfo(3).c_str());
	//ImGui::Text(m_pGranade->displayInfo(4).c_str());

	//static float float3[3] = { 0.0f, 1.0f, 1.5f };
	//if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	//{
	//	std::cout << float3[0] << std::endl;
	//	std::cout << float3[1] << std::endl;
	//	std::cout << float3[2] << std::endl;
	//	std::cout << "---------------------------\n";
	//}


	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::Calc_Box_Dynamics()
{
	float RotAngle = atan(Rise_Run.y / Rise_Run.x);
	float AngleDeg = RotAngle * 180.0f / 3.14159f;
	m_pBox->rotate = AngleDeg;
	m_pBox->deceleration = -coeff_frict * grav;
	//static float acc = sin(RotAngle) * grav;
	//m_pBox->getRigidBody()->acceleration = glm::vec2(cos(RotAngle), sin(RotAngle)) * acc;
	m_pBox->getRigidBody()->acceleration = glm::vec2(cos(RotAngle) * sin(RotAngle), sin(RotAngle) * sin(RotAngle)) * grav;
}
