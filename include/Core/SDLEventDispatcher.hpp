#pragma once
#include "Core/IEventDispatcher.hpp"
#include <SDL3/SDL.h>
#include <queue>
#include <array>

namespace Core {

	class SDLEventDispatcher : public IEventDispatcher {
	private:
			std::queue<ProximaEvent> _q; 
			std::array<Handler, TEXT_INPUT + 1> _handlers{};
	public:
            void onQuit(Handler h) override { _handlers[QUIT] = std::move(h); }

			void pollAndTranslate() override { SDL_Event evt;
			while (SDL_PollEvent(&evt)) {

			  }
			}

			void dispatch() override {
				while (!_q.empty()) {
                auto e = _q.front();
                  _q.pop();
                if (_handlers[e.type])
                  _handlers[e.type](e);
			  }
			}
	};

} // Namespace Core