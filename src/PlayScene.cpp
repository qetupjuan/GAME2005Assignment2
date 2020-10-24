#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

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
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
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
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	m_pGranade = new Target();
	addChild(m_pGranade);

	m_pTrooper = new Ship();
	addChild(m_pTrooper);

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


void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Game Physics Controller", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Throw"))
	{
		m_pGranade->ThrowPosition = m_pPlayer->getTransform()->position;
		m_pGranade->launch();
	}

	ImGui::Separator();

	static bool EnabledGrav = false;
	if (ImGui::Checkbox("Gravity", &EnabledGrav))
	{
		m_pGranade->EnableGravity = EnabledGrav;
	}

	static int xPlayerPos = m_pPlayer->getTransform()->position.x;
	if (ImGui::SliderInt("Player Position x", &xPlayerPos, 0, 800))
	{
		m_pPlayer->getTransform()->position.x = xPlayerPos;
		//m_pGranade->ThrowPosition = glm::vec2 (xPlayerPos, 300);
	}

	static int xEnemyPos = m_pTrooper->getTransform()->position.x;
	if (ImGui::SliderInt("Stormtrooper Position x", &xEnemyPos, 0, 800))
	{
		m_pTrooper->getTransform()->position.x = xEnemyPos;
	}

	static float ThrowAngle;
	static float ThrowVel;
	if (ImGui::SliderFloat2("Throw Angle (degrees) and Velocity", &ThrowAngle, 0, 180))
	{
		float radi = (ThrowAngle * 3.14159f) / 180.0f;
		float x_vel = cos(radi) * ThrowVel;
		float y_vel = -sin(radi) * ThrowVel;
		m_pGranade->ThrowSpeed = glm::vec2(x_vel, y_vel);
	}

	if (ImGui::Button("Test Angle 1"))
	{
		ThrowAngle = 15.8896f;
		ThrowVel = 95.0f;
		float radi = (ThrowAngle * 3.14159f) / 180.0f;
		float x_vel = cos(radi) * ThrowVel;
		float y_vel = -sin(radi) * ThrowVel;
		m_pGranade->ThrowSpeed = glm::vec2(x_vel, y_vel);
	}

	if (ImGui::Button("Test Angle 2"))
	{
		ThrowAngle = 74.1104;
		ThrowVel = 95.0f;
		float radi = (ThrowAngle * 3.14159f) / 180.0f;
		float x_vel = cos(radi) * ThrowVel;
		float y_vel = -sin(radi) * ThrowVel;
		m_pGranade->ThrowSpeed = glm::vec2(x_vel, y_vel);
	}

	if (ImGui::Button("Set Trooper at 485m away from Wookie"))
	{
		m_pTrooper->getTransform()->position.x = m_pPlayer->getTransform()->position.x + 485.0f;
	}

	if (ImGui::Button("1G Acceleration"))
	{
		m_pGranade->gravity.y = 9.8f;
	}

	if (ImGui::Button("2G Acceleration"))
	{
		m_pGranade->gravity.y = 19.6f;
	}

	if (ImGui::Button("10G Acceleration"))
	{
		m_pGranade->gravity.y = 98.0f;
	}
	
	ImGui::Text("Granade Mass: 2.2kg");
	ImGui::Text(m_pGranade->displayInfo(1).c_str());
	ImGui::Text(m_pGranade->displayInfo(2).c_str());
	ImGui::Text(m_pGranade->displayInfo(3).c_str());
	ImGui::Text(m_pGranade->displayInfo(4).c_str());

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
