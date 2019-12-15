/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [11/12/19] - Made simple menu/game/gameover system
 * + Francesco Frassineti [06/12/19] - Added CharacterController as a friend class
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
#include "TileMap.hpp"
#include <queue>

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
	static constexpr float32 physicsScale = 0.01f;

	std::shared_ptr<GameObject> createGameObject();
	void addGameObject(std::shared_ptr<GameObject> gameObject);
	void destroyGameObject(GameObject* gameObject);

	void requestChangeState(GameState state);//Change GameState in a safe way

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	std::shared_ptr<sre::SpriteAtlas> getSpriteAtlas();

	// Delete copy equal 
	LightOfTheMoon(const LightOfTheMoon&) = delete;
	LightOfTheMoon& operator=(const LightOfTheMoon&) = delete;

private:
	static LightOfTheMoon* instance;

	sre::SDLRenderer r;

	void initMenu();
	void initLevel();
	void initGameOver();

	void initPlayer();
	void initBoss();

	void initPhysics();

	void update(float time);
	void render();
	void onKey(SDL_Event& event);
	void onMouse(SDL_Event& event);

	void handleContact(b2Contact* contact, bool begin);

	void changeState(GameState state);

	std::shared_ptr<CameraComponent> camera;
	std::shared_ptr<sre::SpriteAtlas> spriteAtlas;

	std::vector<std::shared_ptr<GameObject>> sceneObjects;

	void updatePhysics();

	std::queue<std::shared_ptr<GameObject>> toBeAdded;
	std::queue<GameObject*> toBeRemoved;

	sre::Color backgroundColor;
	b2World* world = nullptr;
	void registerPhysicsComponent(PhysicsComponent* r);
	void deregisterPhysicsComponent(PhysicsComponent* r);
	std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;
	Box2DDebugDraw debugDraw;
	bool doDebugDraw = false;

	TileMap currentTileMap;

	GameState currentState;
	GameState requestedState;

	friend class PhysicsComponent;
	friend class CharacterController;
};