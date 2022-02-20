#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

#include "SystemBase.h"

class Application : public OgreBites::ApplicationContext
{
public:
	Application();
	void Setup();
	void Run();
	void RegisterSystem(SystemBase *system);

	Ogre::SceneNode *camNode;
	Ogre::SceneNode *modelNode;

	entt::registry *registry;

private:
	Ogre::Root *root;
	Ogre::SceneManager *scnMgr;
	Ogre::RTShader::ShaderGenerator *shadergen;

	SystemBase *systems[128];
};
