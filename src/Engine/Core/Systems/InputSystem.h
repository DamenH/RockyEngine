#include <SDL2/SDL.h>

#include "Engine/Core/SystemBase.h"
#include "Engine/Core/Components/InputComponent.h"

class InputSystem : SystemBase
{
	void OnStartup(entt::registry &registry) override
	{
		const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
		int32_t MouseDeltaX, MouseDeltaY;
		SDL_GetRelativeMouseState(&MouseDeltaX, &MouseDeltaY);

		auto entity = registry.create();
		registry.emplace<InputComponent>(
			entity,
			!!keyStates[SDL_SCANCODE_W],
			!!keyStates[SDL_SCANCODE_S],
			!!keyStates[SDL_SCANCODE_A],
			!!keyStates[SDL_SCANCODE_D],
			MouseDeltaX,
			MouseDeltaY);
	}

	void OnUpdate(entt::registry &registry) override
	{
		const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
		int32_t MouseDeltaX, MouseDeltaY;
		SDL_GetRelativeMouseState(&MouseDeltaX, &MouseDeltaY);

		auto inputView = registry.view<InputComponent>();

		auto &input = inputView.get<InputComponent>(inputView[0]);
		for (auto entity : inputView)
		{
			auto &input = inputView.get<InputComponent>(entity);

			input.ForwardPressed = !!keyStates[SDL_SCANCODE_W];
			input.BackPressed = !!keyStates[SDL_SCANCODE_S];
			input.LeftPressed = !!keyStates[SDL_SCANCODE_A];
			input.RightPressed = !!keyStates[SDL_SCANCODE_D];

			input.MouseDeltaX = MouseDeltaX;
			input.MouseDeltaY = MouseDeltaY;
		}
	}
};
