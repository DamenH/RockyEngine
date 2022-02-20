#include <entt/entt.hpp>

struct SystemBase 
{
	virtual void OnStartup(entt::registry &registry) = 0;
	virtual void OnUpdate(entt::registry &registry) = 0;
};