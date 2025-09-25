TODO: Proxima UI Core Development (MVP)

1. Bulletproof Core UI Components (C++)

- Rendering Pipeline:
	- Implement efficient and correct rendering for basic shapes (rectangles, circles, lines).


- Fundamental Widgets:
	- Develop robust Text Input (keyboard, cursor, selection, scrolling).

	- Create basic Layout Managers (StackLayout, FlowLayout, GridLayout).


- Event System:
	- Design and implement React-like event control with proper propagation.


- Resource Management:
	- Establish robust loading, caching, and cleanup for Fonts and Textures.


2. Clear & Stable C-Callable API (FFI Layer)

- API Header (.h):
	- Define a C-style API (using extern "C") exposing core UI functions and structs.

	- Avoid C++ features (classes, templates, exceptions, std::string) in the public API.


- Wrapper Functions:
	- Write thin C++ wrappers to convert between C-style arguments and C++ objects.


- Callbacks:
	- Implement a void* user_data mechanism for language-specific event callbacks.


3. Demonstrate with Python Bindings

- Python Bindings (ctypes/cffi):
	- Create a Python module to load the C library.

	- Develop Python classes wrapping C structs/pointers.

	- Map C functions to Python methods.

	- Implement the Python -> C callback mechanism.


- Simple Example App:
	- Build a Python script with a window, button, and click event.


4. Build System & Packaging

- CMake Setup:
	- Configure CMake to compile C++ into shared libraries.

	- Generate C-callable API header files.

	- (Optional) Support static library builds.


- Basic Packaging:
	- Provide clear build and linking instructions.



---

NOT for MVP:

- Multi-language support beyond C++ and Python.

- Complex custom widgets.

- Animation/transitions.

- Advanced theming/styling engine.

- Extensive cross-platform quirk handling (beyond basic SDL).