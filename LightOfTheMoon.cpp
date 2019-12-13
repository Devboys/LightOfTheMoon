/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Added 
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
#include "SpriteAnimationComponent.hpp"
#include "CharacterController.hpp"
#include "HealthComponent.hpp"
#include "FixedDamageComponent.hpp"
#include "AudioLocator.hpp"
#include "GameAudio.hpp"
#include "AssetLocator.hpp"
#include "GameAssetManager.hpp"

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

void LightOfTheMoon::requestChangeState(GameState state) {
	requestedState = state;
}

void LightOfTheMoon::changeState(GameState state) {
	//clear previous state
	initPhysics();
	sceneObjects.clear();

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

	//make menu title objects
	auto titleObj = createGameObject();
	titleObj->name = "Title";
	titleObj->position = glm::vec2(0, 0);

	auto spr = titleObj->addComponent<SpriteComponent>();
	spr->setSprite(titleSprite);

	auto startTextObj = createGameObject();
	startTextObj->name = "SubText";
	startTextObj->position = glm::vec2(0, -(startTextSprite.getSpriteSize().y * 2) * startTextSprite.getScale().y);

	spr = startTextObj->addComponent<SpriteComponent>();
	spr->setSprite(startTextSprite);
}


void LightOfTheMoon::initLevel() {
	spriteAtlas = AssetLocator::getService()->getSpriteAtlas("Assets/Sprites/LOTMSprites.json");

	auto camObj = createGameObject();
	camObj->name = "Camera";
	camera = camObj->addComponent<CameraComponent>();
	camObj->setPosition(windowSize * 0.5f);

	// Create all the things in the level

	// Create TileMapRenderer object
	currentTileMap.loadSprites(spriteAtlas);
	currentTileMap.loadMap("Assets/Levels/level0.json");
	//currentTileMap.printMap();
	currentTileMap.generateColliders();

	//PLAYER
	auto playerObj = createGameObject();
	playerObj->name = "Player";
	playerObj->setPosition({ 0, 0 });

	//<Animation>
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

	//</Animation>

	auto phys = playerObj->addComponent<PhysicsComponent>();
	phys->initBox(b2_dynamicBody, { 2.0f, 4.5f}, {playerObj->getPosition().x, playerObj->getPosition().y}, 1);
	phys->fixRotation();

	auto characterHealth = playerObj->addComponent <HealthComponent>();
	float characterHealthAmount = 5;
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

		// Do the debug draw here
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
			if (currentState == GameState::Menu || currentState == GameState::GameOver) {
				changeState(GameState::Running);
			}
			break;
		case SDLK_BACKSPACE:
			if (currentState == GameState::GameOver) {
				changeState(GameState::Menu);
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
		gameObject->setPosition(glm::vec2(position.x * physicsScale, position.y * physicsScale));
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
	else {
		assert(false); // cannot find physics object
	}
}

void LightOfTheMoon::registerPhysicsComponent(PhysicsComponent* r) {
	physicsComponentLookup[r->getFixture()] = r;
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