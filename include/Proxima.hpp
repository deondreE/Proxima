#pragma once

#if defined (_WIN32)
	#include <format>
	#include <iostream>
	#include <memory>
	#include "UI/W_Window.hpp"
	#include "UI/Text.hpp"
#elif defined(__linux__)
	#include <SDL3/SDL.h>
	#include <SDL3_image/SDL_image.h>
	#include <SDL3_ttf/SDL_ttf.h>
	#include <format>
	#include <iostream>
	#include <memory>

	#include "Core/EventDispatcher.hpp" 
	#include "Core/ProximaEvent.hpp"

	#include "UI/Button.hpp"
	#include "UI/ConfigManager.hpp"
	#include "UI/Image.hpp"
	#include "UI/Line.hpp"
	#include "UI/Rect.hpp"  
	#include "UI/Slider.hpp"  
	#include "UI/View.hpp"
	#include "UI/TextInput.hpp"

	#include "UI/StackLayout.hpp"
	#include "UI/Window.hpp"
#endif

