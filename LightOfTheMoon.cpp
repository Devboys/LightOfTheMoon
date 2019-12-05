/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include <sre/Inspector.hpp>
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "sre/RenderPass.hpp"
#include "sre/Texture.hpp"
#include "LightOfTheMoon.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"

using namespace std;
using namespace sre;

const glm::vec2 LightOfTheMoon::windowSize(1200, 900);

LightOfTheMoon& LightOfTheMoon::getInstance() {
	static LightOfTheMoon instance;
	return instance;
}

LightOfTheMoon::LightOfTheMoon()
	:debugDraw(physicsScale)
{
	r.setWindowSize(windowSize);
	bool useVsync = true;
	r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
		.withSdlWindowFlags(SDL_WINDOW_OPENGL)
		.withVSync(useVsync);

	backgroundColor = { .13f,.13f,.13f,1.0f };

	/* // Sprite Atlas creation
	spriteAtlas = SpriteAtlas::create("FILE-NAME-HERE.json", Texture::create()
		.withFile("FILE-NAME-HERE.png")
		.withFilterSampling(false)
		.build());
	*/

	initLevel();

	// setup callback functions
	r.keyEvent = [&](SDL_Event& e) {
		onKey(e);
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

void LightOfTheMoon::initLevel() {
	initPhysics();

	auto camObj = createGameObject();
	camObj->name = "Camera";
	camera = camObj->addComponent<CameraComponent>();
	camObj->setPosition(windowSize * 0.5f);

	// Create all the things in the level
}

void LightOfTheMoon::update(float time) {
	updatePhysics();
	if (time > 0.03) // if framerate approx 30 fps then run two physics steps
	{
		updatePhysics();
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

		case SDLK_d:
			// Press 'D' for physics debug
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
	sceneObjects.push_back(obj);
	return obj;
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