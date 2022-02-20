#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"

class FreeCameraSystem : SystemBase
{
	void OnStartup(entt::registry &registry) override
	{
		// Create the camera?
	}

	void OnUpdate(entt::registry &registry) override
	{
		auto inputView = registry.view<InputComponent>();

		auto &input = inputView.get<InputComponent>(inputView[0]);
		for (auto entity : inputView)
		{
			auto &input = inputView.get<InputComponent>(entity);

			input.ForwardPressed;
			input.BackPressed;
			input.LeftPressed;
			input.RightPressed;
			input.MouseDeltaX;
			input.MouseDeltaY;
		}
	}
};
