#pragma once
#include <functional>
#include "Core/ProximaEvent.hpp"

class IEventDispatcher {
public:
 using Handler = std::function<bool(const ProximaEvent&)>;
 virtual ~IEventDispatcher() = default; 
	
 virtual void onQuit(Handler h) = 0;
 virtual void onKeyPress(Handler h) = 0;
 virtual void onKeyRelease(Handler h) = 0;
 virtual void onMousePress(Handler h) = 0;
 virtual void onMouseRelease(Handler h) = 0;
 virtual void onMouseMotion(Handler h) = 0;
 virtual void onWindowResize(Handler h) = 0;
 virtual void onTextInput(Handler h) = 0;

 virtual void pollAndTranslate() = 0;
 virtual void dispatch() = 0;
};