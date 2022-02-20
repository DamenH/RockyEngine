#include <entt/entt.hpp>

struct SystemBase 
{
	virtual void OnStartup(entt::registry &registry);
	virtual void OnUpdate(entt::registry &registry);
};