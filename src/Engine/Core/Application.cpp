#include "Ogre.h"
#include "OgreApplicationContext.h"

#include <entt/entt.hpp>

class Application : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
	Application();
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent &evt);

	Ogre::SceneNode *camNode;
	Ogre::SceneNode *modelNode;
};

Application::Application() : OgreBites::ApplicationContext("Bootstrap Ogre")
{
}

bool Application::keyPressed(const OgreBites::KeyboardEvent &evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	return true;
}

void Application::setup(void)
{
	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();

	// register for input events
	addInputListener(this);

	// get a pointer to the already created root
	Ogre::Root *root = getRoot();
	Ogre::SceneManager *scnMgr = root->createSceneManager();

	// register our scene with the RTSS
	Ogre::RTShader::ShaderGenerator *shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// without light we would just get a black screen
	Ogre::Light *light = scnMgr->createLight("MainLight");
	Ogre::SceneNode *lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 10, 15);
	lightNode->attachObject(light);

	// also need to tell where we are
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	camNode->setPosition(0, 0, 15);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

	// create the camera
	Ogre::Camera *cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(true);
	camNode->attachObject(cam);

	// and tell it to render into the main window
	getRenderWindow()->addViewport(cam);

	// finally something to render
	Ogre::Entity *ent = scnMgr->createEntity("DamagedHelmet.mesh");
	modelNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	modelNode->attachObject(ent);

	unsigned short src, dst;
	if (!ent->getMesh()->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dst))
	{
		// enforce that we have tangent vectors
		ent->getMesh()->buildTangentVectors(Ogre::VES_TANGENT, src, dst);
	}

	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("DamagedHelmet");
	//Ogre::GpuProgramParametersSharedPtr mParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	//mParams->setNamedConstant("u_ScaleIBLAmbient", Ogre::Vector4(Ogre::Real(1)));
}