/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Fixed Singleton implementation to deal with gameloop
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "Box2DDebugDraw.hpp"
#include "GameObject.hpp"
#include "CameraComponent.hpp"
#include "Level.hpp"

class PhysicsComponent;

enum class GameState {
	Menu,
	Running,
	GameOver
};

class LightOfTheMoon : public b2ContactListener {
public:
	LightOfTheMoon();
	~LightOfTheMoon();
	static LightOfTheMoon* getInstance();

	static const glm::vec2 windowSize;
	static constexpr float32 timeStep = 1.0f / 60.0f;

	std::shared_ptr<GameObject> createGameObject();

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	// Delete copy equal 
	LightOfTheMoon(const LightOfTheMoon&) = delete;
	LightOfTheMoon& operator=(const LightOfTheMoon&) = delete;

private:
	static LightOfTheMoon* instance;

	sre::SDLRenderer r;

	void initLevel();
	void initPhysics();

	void update(float time);
	void render();
	void onKey(SDL_Event& event);

	void handleContact(b2Contact* contact, bool begin);

	std::shared_ptr<CameraComponent> camera;
	std::shared_ptr<sre::SpriteAtlas> spriteAtlas;

	std::vector<std::shared_ptr<GameObject>> sceneObjects;

	void updatePhysics();

	sre::Color backgroundColor;
	b2World* world = nullptr;
	const float physicsScale = 0.01f;
	void registerPhysicsComponent(PhysicsComponent* r);
	void deregisterPhysicsComponent(PhysicsComponent* r);
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;

	Level currentLevel;

	friend class PhysicsComponent;
};