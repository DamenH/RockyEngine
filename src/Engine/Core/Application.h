#include "Ogre.h"
#include "OgreApplicationContext.h"

class Application : public OgreBites::ApplicationContext
{
public:
	Application();
	void setup();
	Ogre::SceneNode *camNode;
};
