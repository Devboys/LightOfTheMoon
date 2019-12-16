/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [16/12/19] - Simple GUI to display player and boss HP during game time
 * + Alberto Giudice [16/12/19] - Implemented muting audio with M key press
 * + Alberto Giudice [16/12/19] - Added sprites for different boss phases
 * + Alberto Giudice [15/12/19] - Deactivate physics on bullets when despawned
 * + Alberto Giudice [15/12/19] - Modified object clearing to support the bullet pool items
 * + Alberto Giudice [15/12/19] - Modified the physics lookup functions to support the second fixture
 * + Alberto Giudice [14/12/19] - Added a test linear/wave/spiral bullet in the direction of the player
 * + Francesco Frassineti [07/12/19] - Added the idle animations for the player
 * + Francesco Frassineti [07/12/19] - The mouse cursor is now visible (it's still trapped inside the window)
 * + Francesco Frassineti [06/12/19] - Added mouse handling
 * + Francesco Frassineti [06/12/19] - Added a temporary object to test the damaging system
 * + Jeppe Faber     [05/12/19] - Added TileMapRenderer-object to render loop
 * + Alberto Giudice [05/12/19] - Implemented sprite atlas with a sample character animation
 * + Alberto Giudice [05/12/19] - Added game title on the game window
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include "LightOfTheMoon.hpp"
#include <sre/Inspector.hpp>
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "sre/RenderPass.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterController.hpp"
#include "HealthComponent.hpp"
#include "AudioLocator.hpp"
#include "GameAudio.hpp"
#include "NullAudio.hpp"
#include "AssetLocator.hpp"
#include "GameAssetManager.hpp"
#include "BulletComponent.hpp"
#include "MovementLinearComponent.hpp"
#include "MovementWaveComponent.hpp"
#include "MovementSpiralComponent.hpp"
#include "Boss1Component.hpp"


#define BOSS_SPRITE_SCALE 0.002f

using namespace std;
using namespace sre;

const glm::vec2 LightOfTheMoon::windowSize(1200, 900);

LightOfTheMoon* LightOfTheMoon::instance = nullptr;

LightOfTheMoon* LightOfTheMoon::getInstance() {
	return instance;
}

LightOfTheMoon::LightOfTheMoon()
	:debugDraw(physicsScale)
{
	assert(instance == nullptr);
	instance = this;

	r.setWindowSize(windowSize);
	r.setWindowTitle("Light of the Moon");

	//intialize SRE
	r.init();

	SDL_SetWindowGrab(r.getSDLWindow(), SDL_TRUE); //The cursor will stay inside the window
	//SDL_SetRelativeMouseMode(SDL_TRUE); //Hide the mouse cursor

	backgroundColor = { .13f,.13f,.13f,1.0f };

	//init default services for locators
	AudioLocator::initialize();
	AssetLocator::initialize(); //must be initialized AFTER sre

	//Provide basic services
	AudioLocator::setService(std::make_shared<GameAudio>());
	if (AudioLocator::getService() != nullptr)
		audioOn = true;
	AssetLocator::setService(std::make_shared<GameAssetManager>());

	changeState(GameState::Menu);

	// setup callback functions
	r.keyEvent = [&](SDL_Event& e) {
		onKey(e);
	};
	r.mouseEvent = [&](SDL_Event& e) {
		onMouse(e);
	};
	r.frameUpdate = [&](float deltaTime) {
		update(deltaTime);
	};
	r.frameRender = [&]() {
		render();
	};
	// start game loop
	r.startEventLoop();
}

std::shared_ptr<sre::SpriteAtlas> LightOfTheMoon::getSpriteAtlas() {
	return spriteAtlas;
}

void LightOfTheMoon::requestChangeState(GameState state) {
	requestedState = state;
}

void LightOfTheMoon::changeState(GameState state) {
	//clear previous state
	player = nullptr;
	boss = nullptr;
	sceneObjects.clear();
	initPhysics();
	currentTileMap.clearMap();

	//init new state
	switch (state)
	{
	case GameState::Menu:
		initMenu();
		break;
	case GameState::Running:
		initLevel();
		break;
	case GameState::GameOver:
		initGameOver();
		break;
	case GameState::Win:
		initWin();
		break;
	default:
		break;
	}

	currentState = state;
	requestedState = state;
}

void LightOfTheMoon::initMenu() {
	
	auto camObj = createGameObject();
	camObj->name = "Camera";
	camera = camObj->addComponent<CameraComponent>();
	camObj->setPosition(windowSize * 0.5f);

	std::shared_ptr<sre::SpriteAtlas> uiAtlas = AssetLocator::getService()->getSpriteAtlas("Assets/Sprites/MenuSprites.json");

	//fetch menu title sprites
	auto titleSprite = uiAtlas->get("Title.png");
	titleSprite.setScale(glm::vec2(0.001f, 0.001f));

	auto startTextSprite = uiAtlas->get("EnterStart.png");
	startTextSprite.setScale(glm::vec2(0.001f, 0.001f));

	auto quitTextSprite = uiAtlas->get("EscapeQuit.png");
	quitTextSprite.setScale(glm::vec2(0.001f, 0.001f));

	//make menu title objects
	auto titleObj = createGameObject();
	titleObj->name = "Title";
	titleObj->position = glm::vec2(0, 0);

	auto spr = titleObj->addComponent<SpriteComponent>();
	spr->setSprite(titleSprite);

	auto startTextObj = createGameObject();
	startTextObj->name = "StartText";
	startTextObj->position = glm::vec2(0, -(startTextSprite.getSpriteSize().y * 2) * startTextSprite.getScale().y);

	spr = startTextObj->addComponent<SpriteComponent>();
	spr->setSprite(startTextSprite);

	auto quitTextObj = createGameObject();
	quitTextObj->name = "QuitText";
	quitTextObj->position = glm::vec2(0, startTextObj->getPosition().y - quitTextSprite.getSpriteSize().y * 1.1f *  quitTextSprite.getScale().y);

	spr = quitTextObj->addComponent<SpriteComponent>();
	spr->setSprite(quitTextSprite);

	//start music
	AudioLocator::getService()->playLooped("Assets/Sounds/renovation_airtone.wav");
}


void LightOfTheMoon::initLevel() {
	spriteAtlas = AssetLocator::getService()->getSpriteAtlas("Assets/Sprites/LOTMSprites.json");

	auto camObj = createGameObject();
	camObj->name = "Camera";
	camera = camObj->addComponent<CameraComponent>();
	camObj->setPosition(windowSize * 0.5f);

	// Create all the things in the level

	// Create TileMap object
	currentTileMap.loadSprites(spriteAtlas);
	currentTileMap.loadMap("Assets/Levels/level1.json");
	//currentTileMap.printMap();
	currentTileMap.generateColliders();

	//PLAYER
	player = initPlayer();

	//BOSS
	boss = initBoss(player);
}

std::shared_ptr<GameObject> LightOfTheMoon::initPlayer() {

	auto playerObj = createGameObject();
	playerObj->name = "Player";
	playerObj->setPosition({ 0, -0.4 });

	//< Player Animation>
	auto anim = playerObj->addComponent<AnimatorComponent>();

	vector<Sprite> sprites_right({ spriteAtlas->get("cowboy-right-1.png"), spriteAtlas->get("cowboy-right-2.png") });
	vector<Sprite> sprites_top_right({ spriteAtlas->get("cowboy-top-right-1.png"), spriteAtlas->get("cowboy-top-right-2.png") });
	vector<Sprite> sprites_top({ spriteAtlas->get("cowboy-top-1.png"), spriteAtlas->get("cowboy-top-2.png") });
	vector<Sprite> sprites_top_left({ spriteAtlas->get("cowboy-top-left-1.png"), spriteAtlas->get("cowboy-top-left-2.png") });
	vector<Sprite> sprites_left({ spriteAtlas->get("cowboy-left-1.png"), spriteAtlas->get("cowboy-left-2.png") });
	vector<Sprite> sprites_down_left({ spriteAtlas->get("cowboy-down-left-1.png"), spriteAtlas->get("cowboy-down-left-2.png") });
	vector<Sprite> sprites_down({ spriteAtlas->get("cowboy-down-1.png"), spriteAtlas->get("cowboy-down-2.png") });
	vector<Sprite> sprites_down_right({ spriteAtlas->get("cowboy-down-right-1.png"), spriteAtlas->get("cowboy-down-right-2.png") });
	for (auto& s : sprites_right) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_top_right) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_top) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_top_left) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_left) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_down_left) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_down) { s.setScale({ 0.001f, 0.001f }); }
	for (auto& s : sprites_down_right) { s.setScale({ 0.001f, 0.001f }); }

	std::shared_ptr<Animation> player_idle_right_anim = std::make_shared<Animation>(sprites_right, 1, true);
	std::shared_ptr<Animation> player_idle_top_right_anim = std::make_shared<Animation>(sprites_top_right, 1, true);
	std::shared_ptr<Animation> player_idle_top_anim = std::make_shared<Animation>(sprites_top, 1, true);
	std::shared_ptr<Animation> player_idle_top_left_anim = std::make_shared<Animation>(sprites_top_left, 1, true);
	std::shared_ptr<Animation> player_idle_left_anim = std::make_shared<Animation>(sprites_left, 1, true);
	std::shared_ptr<Animation> player_idle_down_left_anim = std::make_shared<Animation>(sprites_down_left, 1, true);
	std::shared_ptr<Animation> player_idle_down_anim = std::make_shared<Animation>(sprites_down, 1, true);
	std::shared_ptr<Animation> player_idle_down_right_anim = std::make_shared<Animation>(sprites_down_right, 1, true);

	anim->setAnimation(player_idle_down_anim, true);

	//</Player Animation>

	auto phys = playerObj->addComponent<PhysicsComponent>(); 
	phys->initCircle(b2_dynamicBody, 2.0f, { playerObj->getPosition().x / physicsScale, playerObj->getPosition().y / physicsScale }, 1);
	phys->initSensorBox({ 2.0f, 4.5f }, { playerObj->getPosition().x / physicsScale, playerObj->getPosition().y / physicsScale }, 0, { 0.0f, 0.025f / physicsScale });
	phys->fixRotation();

	auto characterHealth = playerObj->addComponent <HealthComponent>();
	float characterHealthAmount = 100;
	characterHealth->setMaxHealth(characterHealthAmount);
	characterHealth->setCurrentHealth(characterHealthAmount);

	auto characterController = playerObj->addComponent<CharacterController>();
	characterController->setAnimations(
		player_idle_right_anim,
		player_idle_top_right_anim,
		player_idle_top_anim,
		player_idle_top_left_anim,
		player_idle_left_anim,
		player_idle_down_left_anim,
		player_idle_down_anim,
		player_idle_down_right_anim);

	return playerObj;
}

std::shared_ptr<GameObject> LightOfTheMoon::initBoss(std::shared_ptr<GameObject> player) {

	auto bossObj = createGameObject();
	bossObj->name = "Boss1";
	bossObj->setPosition({ 0, 0 });

	//< Boss Animation>
	auto anim = bossObj->addComponent<AnimatorComponent>();

	// Sprites for boss1 first phase
	vector<Sprite> sprites_right({ spriteAtlas->get("enemy-right.png") });
	vector<Sprite> sprites_top_right({ spriteAtlas->get("enemy-top-right.png") });
	vector<Sprite> sprites_top({ spriteAtlas->get("enemy-top.png") });
	vector<Sprite> sprites_top_left({ spriteAtlas->get("enemy-top-left.png") });
	vector<Sprite> sprites_left({ spriteAtlas->get("enemy-left.png") });
	vector<Sprite> sprites_down_left({ spriteAtlas->get("enemy-down-left.png") });
	vector<Sprite> sprites_down({ spriteAtlas->get("enemy-down.png") });
	vector<Sprite> sprites_down_right({ spriteAtlas->get("enemy-down-right.png") });
	for (auto& s : sprites_right) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_right) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_left) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_left) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_left) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_right) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }

	// Sprites for boss1 second phase
	vector<Sprite> sprites_right_2({ spriteAtlas->get("enemy-right-2.png") });
	vector<Sprite> sprites_top_right_2({ spriteAtlas->get("enemy-top-right-2.png") });
	vector<Sprite> sprites_top_2({ spriteAtlas->get("enemy-top-2.png") });
	vector<Sprite> sprites_top_left_2({ spriteAtlas->get("enemy-top-left-2.png") });
	vector<Sprite> sprites_left_2({ spriteAtlas->get("enemy-left-2.png") });
	vector<Sprite> sprites_down_left_2({ spriteAtlas->get("enemy-down-left-2.png") });
	vector<Sprite> sprites_down_2({ spriteAtlas->get("enemy-down-2.png") });
	vector<Sprite> sprites_down_right_2({ spriteAtlas->get("enemy-down-right-2.png") });
	for (auto& s : sprites_right_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_right_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_left_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_left_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_left_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_right_2) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }

	// Sprites for boss1 third phase
	vector<Sprite> sprites_right_3({ spriteAtlas->get("enemy-right-3.png") });
	vector<Sprite> sprites_top_right_3({ spriteAtlas->get("enemy-top-right-3.png") });
	vector<Sprite> sprites_top_3({ spriteAtlas->get("enemy-top-3.png") });
	vector<Sprite> sprites_top_left_3({ spriteAtlas->get("enemy-top-left-3.png") });
	vector<Sprite> sprites_left_3({ spriteAtlas->get("enemy-left-3.png") });
	vector<Sprite> sprites_down_left_3({ spriteAtlas->get("enemy-down-left-3.png") });
	vector<Sprite> sprites_down_3({ spriteAtlas->get("enemy-down-3.png") });
	vector<Sprite> sprites_down_right_3({ spriteAtlas->get("enemy-down-right-3.png") });
	for (auto& s : sprites_right_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_right_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_top_left_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_left_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_left_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }
	for (auto& s : sprites_down_right_3) { s.setScale({ BOSS_SPRITE_SCALE, BOSS_SPRITE_SCALE }); }

	std::shared_ptr<Animation> enemy_idle_right_anim[3] = { std::make_shared<Animation>(sprites_right, 1, true), 
															std::make_shared<Animation>(sprites_right_2, 1, true), 
															std::make_shared<Animation>(sprites_right_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_top_right_anim[3] = { std::make_shared<Animation>(sprites_top_right, 1, true),
															std::make_shared<Animation>(sprites_top_right_2, 1, true),
															std::make_shared<Animation>(sprites_top_right_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_top_anim[3] = { std::make_shared<Animation>(sprites_top, 1, true),
															std::make_shared<Animation>(sprites_top_2, 1, true),
															std::make_shared<Animation>(sprites_top_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_top_left_anim[3] = { std::make_shared<Animation>(sprites_top_left, 1, true),
															std::make_shared<Animation>(sprites_top_left_2, 1, true),
															std::make_shared<Animation>(sprites_top_left_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_left_anim[3] = { std::make_shared<Animation>(sprites_left, 1, true),
															std::make_shared<Animation>(sprites_left_2, 1, true),
															std::make_shared<Animation>(sprites_left_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_down_left_anim[3] = { std::make_shared<Animation>(sprites_down_left, 1, true),
															std::make_shared<Animation>(sprites_down_left_2, 1, true),
															std::make_shared<Animation>(sprites_down_left_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_down_anim[3] = { std::make_shared<Animation>(sprites_down, 1, true),
															std::make_shared<Animation>(sprites_down_2, 1, true),
															std::make_shared<Animation>(sprites_down_3, 1, true) };
	std::shared_ptr<Animation> enemy_idle_down_right_anim[3] = { std::make_shared<Animation>(sprites_down_right, 1, true),
															std::make_shared<Animation>(sprites_down_2, 1, true),
															std::make_shared<Animation>(sprites_down_3, 1, true) };

	anim->setAnimation(enemy_idle_down_anim[0], true); //Set initial animation
	//</Boss Animation>

	auto phys = bossObj->addComponent<PhysicsComponent>();
	phys->initCircle(b2_kinematicBody, 5.0f, { bossObj->getPosition().x / physicsScale, bossObj->getPosition().y / physicsScale }, 1);
	phys->setAutoUpdate(false);
	phys->fixRotation();

	auto health = bossObj->addComponent<HealthComponent>();
	float bossHealthAmount = 500;
	health->setMaxHealth(bossHealthAmount);
	health->setCurrentHealth(bossHealthAmount);

	
	auto bossController = bossObj->addComponent<Boss1Component>();
	bossController->setAnimations(
		enemy_idle_right_anim,
		enemy_idle_top_right_anim,
		enemy_idle_top_anim,
		enemy_idle_top_left_anim,
		enemy_idle_left_anim,
		enemy_idle_down_left_anim,
		enemy_idle_down_anim,
		enemy_idle_down_right_anim);

	bossController->setPlayer(player);

	return bossObj;
}

void LightOfTheMoon::initGameOver() {

	auto camObj = createGameObject();
	camObj->name = "Camera";
	camera = camObj->addComponent<CameraComponent>();
	camObj->setPosition(windowSize * 0.5f);

	std::shared_ptr<sre::SpriteAtlas> uiAtlas = AssetLocator::getService()->getSpriteAtlas("Assets/Sprites/MenuSprites.json");

	//fetch menu sprites
	auto gameOverSprite = uiAtlas->get("GameOver.png");
	gameOverSprite.setScale(glm::vec2(0.001f, 0.001f));

	auto restartTextSprite = uiAtlas->get("EnterRestart.png");
	restartTextSprite.setScale(glm::vec2(0.001f, 0.001f));

	auto menuTextSprite = uiAtlas->get("BackspaceMenu.png");
	menuTextSprite.setScale(glm::vec2(0.001f, 0.001f));

	//make menu objects
	auto gameOverObj = createGameObject();
	gameOverObj->name = "Title";
	gameOverObj->position = glm::vec2(0, 0);

	auto spr = gameOverObj->addComponent<SpriteComponent>();
	spr->setSprite(gameOverSprite);

	auto restartTextObj = createGameObject();
	restartTextObj->name = "RestartText";
	restartTextObj->position = glm::vec2(0, -(restartTextSprite.getSpriteSize().y * 2) * restartTextSprite.getScale().y);

	spr = restartTextObj->addComponent<SpriteComponent>();
	spr->setSprite(restartTextSprite);

	auto menuTextObj = createGameObject();
	menuTextObj->name = "MenuText";
	menuTextObj->position = glm::vec2(0, -(menuTextSprite.getSpriteSize().y * 3) * menuTextSprite.getScale().y);

	spr = menuTextObj->addComponent<SpriteComponent>();
	spr->setSprite(menuTextSprite);

}

void LightOfTheMoon::initWin() {

	auto camObj = createGameObject();
	camObj->name = "Camera";
	camera = camObj->addComponent<CameraComponent>();
	camObj->setPosition(windowSize * 0.5f);

	std::shared_ptr<sre::SpriteAtlas> uiAtlas = AssetLocator::getService()->getSpriteAtlas("Assets/Sprites/MenuSprites.json");

	//fetch menu sprites
	auto youWinSprite = uiAtlas->get("YouWin.png");
	youWinSprite.setScale(glm::vec2(0.001f, 0.001f));

	auto restartTextSprite = uiAtlas->get("EnterRestart.png");
	restartTextSprite.setScale(glm::vec2(0.001f, 0.001f));

	auto menuTextSprite = uiAtlas->get("BackspaceMenu.png");
	menuTextSprite.setScale(glm::vec2(0.001f, 0.001f));

	//make menu objects
	auto gameOverObj = createGameObject();
	gameOverObj->name = "Title";
	gameOverObj->position = glm::vec2(0, 0);

	auto spr = gameOverObj->addComponent<SpriteComponent>();
	spr->setSprite(youWinSprite);

	auto restartTextObj = createGameObject();
	restartTextObj->name = "RestartText";
	restartTextObj->position = glm::vec2(0, -(restartTextSprite.getSpriteSize().y * 2) * restartTextSprite.getScale().y);

	spr = restartTextObj->addComponent<SpriteComponent>();
	spr->setSprite(restartTextSprite);

	auto menuTextObj = createGameObject();
	menuTextObj->name = "MenuText";
	menuTextObj->position = glm::vec2(0, -(menuTextSprite.getSpriteSize().y * 3) * menuTextSprite.getScale().y);

	spr = menuTextObj->addComponent<SpriteComponent>();
	spr->setSprite(menuTextSprite);

}

void LightOfTheMoon::update(float time) {
	if (requestedState != currentState) {
		changeState(requestedState); //Change GameState in a safe way
	}

	updatePhysics();
	if (time > 0.03) // if framerate approx 30 fps then run two physics steps
	{
		updatePhysics();
	}

	//Remove all the gameObjects that were requested to be removed in a safe way
	while (!toBeRemoved.empty()) {
		GameObject* go = toBeRemoved.front();
		toBeRemoved.pop();
		for (int i = 0; i < sceneObjects.size(); i++) {
			if (go == &(*sceneObjects[i])) {//Check between a raw pointer and the raw pointer inside the smart pointer
				if (go->getComponent<BulletComponent>() != nullptr) { // If object to remove is a bullet, take action for not messing the bullet pool
					auto phys = go->getComponent<PhysicsComponent>();
					if (phys != nullptr) {
						// Deactivate and hide the physics component so that it doesn't mess up with active objects / slow down performance
						phys->setPositionAndRotation({ 2000.0f, 2000.0f }, 0.0f);
						phys->setActive(false);
						deregisterPhysicsComponent(phys.get());
					}
				}
				sceneObjects.erase(sceneObjects.begin() + i);
				break;
			}
		}
	}

	//Add all the gameObjects that were requested to be removed in a safe wa
	while (!toBeAdded.empty()) {
		std::shared_ptr<GameObject> go = toBeAdded.front();
		toBeAdded.pop();
		sceneObjects.push_back(go);
	}

	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(time);
	}
}

void LightOfTheMoon::render() {
	auto rp = RenderPass::create()
		.withCamera(camera->getCamera())
		.withClearColor(true, backgroundColor)
		.build();

	if (doDebugDraw) {
		static Inspector profiler;
		profiler.update();
		profiler.gui(false);
	}

	auto pos = camera->getGameObject()->getPosition();

	auto spriteBatchBuilder = SpriteBatch::create();

	//render level tiles
	currentTileMap.renderMap(spriteBatchBuilder);

	//render GameObjects
	for (auto& go : sceneObjects) {
		go->renderSprite(spriteBatchBuilder);
	}

	auto sb = spriteBatchBuilder.build();

	rp.draw(sb);

	if (doDebugDraw) {
		world->DrawDebugData();
		rp.drawLines(debugDraw.getLines());
		debugDraw.clear();
	}

	// Render the current HP of player and boss in a GUI if the game is running
	if (currentState == GameState::Running) {
		ImGui::SetNextWindowPos(ImVec2((Renderer::instance->getWindowSize().x - 250.0f) * 0.5f, 50.0f), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(250.0f, 70), ImGuiSetCond_Always);
		ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		if (player != nullptr) {
			std::shared_ptr<HealthComponent> playerHealth = player->getComponent<HealthComponent>();
			if (playerHealth != nullptr) {
				float currentPlayerHealthPercentage = (float)playerHealth->getCurrentHealth() / (float)playerHealth->getMaxHealth();
				ImGui::ProgressBar(currentPlayerHealthPercentage, ImVec2(150.0f, 2), "Player Bar");
				ImGui::LabelText("Player HP", "%i / %i", playerHealth->getCurrentHealth(), playerHealth->getMaxHealth());
			}
		}
		if (boss != nullptr) {
			std::shared_ptr<HealthComponent> bossHealth = boss->getComponent<HealthComponent>();
			if (bossHealth != nullptr) {
				float currentBossHealthPercentage = (float)bossHealth->getCurrentHealth() / (float)bossHealth->getMaxHealth();
				ImGui::ProgressBar(currentBossHealthPercentage, ImVec2(150.0f, 2), "Player Bar");
				ImGui::LabelText("Boss HP", "%i / %i", bossHealth->getCurrentHealth(), bossHealth->getMaxHealth());
			}
		}
		ImGui::End();
	}
}

void LightOfTheMoon::onMouse(SDL_Event& event) {
	for (auto& gameObject : sceneObjects) {
		for (auto& c : gameObject->getComponents()) {
			c->onMouse(event);
		}
	}
}

void LightOfTheMoon::onKey(SDL_Event& event) {
	for (auto& gameObject : sceneObjects) {
		for (auto& c : gameObject->getComponents()) {
			bool consumed = c->onKey(event);
			if (consumed) {
				return;
			}
		}
	}

	if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.sym) {
		
		//  Add all cases for key presses check here
		case SDLK_RETURN:
			if (currentState == GameState::Menu || currentState == GameState::GameOver || currentState == GameState::Win) {
				changeState(GameState::Running);
			}
			break;
		case SDLK_BACKSPACE:
			if (currentState == GameState::GameOver || currentState == GameState::Win) {
				changeState(GameState::Menu);
			}
			break;
		case SDLK_ESCAPE:
			if (currentState == GameState::Menu) {
				r.stopEventLoop(); //quit game
			}
			break;
		case SDLK_m:
			if (audioOn) { // deactivate audio
				AudioLocator::setService(std::make_shared<NullAudio>());
				audioOn = false;
			}
			else { // activate audio
				AudioLocator::setService(std::make_shared<GameAudio>());
				AudioLocator::getService()->playLooped("Assets/Sounds/renovation_airtone.wav");
				audioOn = true;
			}
			break;
		case SDLK_q:
			// Press 'Q' for physics debug
			doDebugDraw = !doDebugDraw;
			if (doDebugDraw) {
				world->SetDebugDraw(&debugDraw);
			}
			else {
				world->SetDebugDraw(nullptr);
			}
			break;
		
		default:
			break;
		}
	}
}

std::shared_ptr<GameObject> LightOfTheMoon::createGameObject() {
	auto obj = shared_ptr<GameObject>(new GameObject());
	toBeAdded.push(obj);//Add it safely (it will be added before updating the next frame)
	return obj;
}

void LightOfTheMoon::addGameObject(std::shared_ptr<GameObject> gameObject) {
	toBeAdded.push(gameObject);//Add it safely (it will be added before updating the next frame)
}

void LightOfTheMoon::destroyGameObject(GameObject* gameObject) {
	toBeRemoved.push(gameObject);//Remove it safely (it will be removed before updating the next frame)
}

void LightOfTheMoon::updatePhysics() {
	const int positionIterations = 4;
	const int velocityIterations = 12;
	world->Step(timeStep, velocityIterations, positionIterations);

	for (auto phys : physicsComponentLookup) {
		PhysicsComponent* physicsComponent = phys.second;
		if (physicsComponent->isAutoUpdate() == false) continue;
		auto position = physicsComponent->getBody()->GetPosition();
		float angle = physicsComponent->getBody()->GetAngle();
		auto gameObject = physicsComponent->getGameObject();
		gameObject->setPosition(glm::vec2((position.x + physicsComponent->getSpriteOffset().x) * physicsScale, (position.y + physicsComponent->getSpriteOffset().y) * physicsScale));
		gameObject->setRotation(angle);
	}
}

void LightOfTheMoon::initPhysics() {
	float gravity = 0; /// things shouldn't fall on their own
	delete world;
	world = new b2World(b2Vec2(0, gravity));
	world->SetContactListener(this);

	if (doDebugDraw) {
		world->SetDebugDraw(&debugDraw);
	}
}

void LightOfTheMoon::BeginContact(b2Contact* contact) {
	b2ContactListener::BeginContact(contact);
	handleContact(contact, true);
}

void LightOfTheMoon::EndContact(b2Contact* contact) {
	b2ContactListener::EndContact(contact);
	handleContact(contact, false);
}

void LightOfTheMoon::deregisterPhysicsComponent(PhysicsComponent* r) {
	auto iter = physicsComponentLookup.find(r->getFixture());
	if (iter != physicsComponentLookup.end()) {
		physicsComponentLookup.erase(iter);
	}
	if (r->getSecondFixture() != nullptr) { // needed for objects with collider + sensor (playerObj)
		auto iter2 = physicsComponentLookup.find(r->getSecondFixture());
		if (iter2 != physicsComponentLookup.end()) {
			physicsComponentLookup.erase(iter2);
		}
	}
}

void LightOfTheMoon::registerPhysicsComponent(PhysicsComponent* r) {
	physicsComponentLookup[r->getFixture()] = r;
}

void LightOfTheMoon::registerSecondPhysicsComponent(PhysicsComponent* r) {
	physicsComponentLookup[r->getSecondFixture()] = r;
}

void LightOfTheMoon::handleContact(b2Contact* contact, bool begin) {
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();
	auto physA = physicsComponentLookup.find(fixA);
	auto physB = physicsComponentLookup.find(fixB);
	if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
		auto& aComponents = physA->second->getGameObject()->getComponents();
		auto& bComponents = physB->second->getGameObject()->getComponents();
		for (auto& c : aComponents) {
			if (begin) {
				c->onCollisionStart(physB->second);
			}
			else {
				c->onCollisionEnd(physB->second);
			}
		}
		for (auto& c : bComponents) {
			if (begin) {
				c->onCollisionStart(physA->second);
			}
			else {
				c->onCollisionEnd(physA->second);
			}
		}
	}
}

LightOfTheMoon::~LightOfTheMoon() {
	// clear the physics component lookup map before deleting everything, to make sure not to reference already deleted stuff
	physicsComponentLookup.clear();
	// delete the world explicitly not to leave behind any memory leak
	delete world;
}