#pragma once 

#if defined(_WIN32) || defined(_WIN64)
#if defined(EXPORT)
	#define PEXPORT __declspec(dllexport)
#else
	#define PEXPORT __declspec(dllimport)
#endif
#elif defined(__APPLE__)
	#define PEXPORT __attribute__((visibility("default")))
#elif defined(__linux__) || defined(__unix__)
	#define PEXPORT __attribute__((visibility("default")))
#else
	#define PEXPORT
#endif