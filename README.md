Proxima - Native Cross-Platform App Platform (MVP)


Proxima is a groundbreaking experiment in native cross-platform application development. Our core philosophy is to build a framework from the ground up, entirely in C, by directly interfacing with low-level system graphics and input APIs. We explicitly avoid all third-party UI toolkits, rendering engines like Skia, and higher-level graphics abstraction libraries (e.g., SDL, SFML, wgpu).

Our goal is to redefine how native applications are built, offering unparalleled control, maximum performance, and a truly bespoke native experience across multiple operating systems, leveraging the raw power of each platform's hardware.

## Mission

To forge a lean, C native foundation for building graphical user interfaces and accessing essential system functionalities on iOS, Android, macOS, and Windows. We aim to achieve this by directly interacting with system-level graphics APIs (Metal, DirectX, Vulkan, OpenGL ES) and implementing our entire UI and event system from first principles.

## Vision (Long-term)

Proxima envisions a future where developers can craft desktop and mobile applications with complete ownership over every pixel and every event, achieving unparalleled performance, a unique aesthetic, and deep system integration, all from a single, C-driven codebase that rethinks traditional application architecture.

## MVP Features

This initial version focuses on demonstrating fundamental, custom-built capabilities:


- Core C Foundation: All application logic is written in modern C (C99/C11/C17) and compiled directly to native machine code. We leverage the C Standard Library for fundamental data structures and algorithms, but avoid extensive third-party helper libraries.

- Direct System Graphics API Interaction:
	- Proxima directly utilizes the native, low-level graphics APIs on each platform:
		- iOS/macOS: Metal

		- Windows: DirectX (e.g., DX11 or DX12 for rendering)

		- Android: OpenGL ES / Vulkan

		- (Potential future desktop Linux): Vulkan / OpenGL


	- Our rendering pipeline, from vertex buffers to shaders, is custom-built on top of these APIs.


- Custom UI Rendering Engine: Proxima implements its own rendering engine for all UI elements. This includes:
	- Vector/Shape Primitives: Drawing lines, rectangles, circles, and other basic shapes directly via the chosen graphics API.

	- Text Rendering: Custom glyph rasterization and atlas management (or highly optimized OS-provided font APIs for glyph data) directly drawn using the graphics API.


- Procedural/Struct-Based UI Definition: UI components will be defined using C structs and function pointers for event handling, rather than a declarative C++ DSL. For example, proxima_button_create(char* text, void (*on_click_handler)(ProximaButton*)).

- Basic UI Components:
	- ProximaView: The fundamental drawable and layout-able element (represented as a struct).

	- ProximaText: For rendering static text.

	- ProximaButton: A clickable interactive element with visual feedback (hover, press states), drawn purely by Proxima, using function pointers for callbacks.

	- ProximaStackLayout: Basic vertical/horizontal layout container, implemented from scratch using structs and functions.


- Direct Input Handling: Raw keyboard, mouse, and touch events are captured directly from the OS-level event loops and processed by Proxima's custom event system, primarily using callbacks and event structs.

- Minimal Platform Abstraction Layer: Custom, thin C wrappers for:
	- Creating and managing OS-level windows/views/surfaces.

	- Setting up graphics contexts/devices (e.g., Metal MTLCommandQueue, DXGI Swap Chain).

	- Receiving raw input events (keyboard, mouse, touch) directly.

	- Accessing essential system APIs (e.g., getting screen resolution, basic timing, memory allocation hooks if needed).


- Native Build Integration:
	- iOS: Integrates with Xcode projects (potentially using Objective-C for bridging), compiling to a native iOS app.

	- Android: Integrates with Gradle/CMake, compiling to a native Android app via NDK (using JNI for Java bridging).

	- macOS: Standard CMake/Xcode project (potentially using Objective-C for bridging), compiling to a native macOS app bundle.

	- Windows: Standard CMake/Visual Studio project, compiling to a native Windows executable (using Win32 API directly).

## Non-Goals for MVP

To maintain a focused scope for this initial, ambitious effort:


- Complex Widget Library: No sophisticated widgets like lists, grids, rich text editors, date pickers, or complex animations.

- Advanced Layout: Only basic linear layouts. No Flexbox or AutoLayout equivalents yet.

- Network Layer: No integrated HTTP client or network abstraction.

- Filesystem Abstraction: Only raw platform-specific file I/O for MVP.

- Native Widget Integration: Proxima renders its entire UI from scratch. It does not wrap or use platform-native UI widgets (e.g., UIButton, NSButton, android.widget.Button, Win32 controls). This is a deliberate design choice for maximum control and consistent rendering.

- Hot Reload/Live Preview: Compilation will be traditional and require a full rebuild.

- Extensive Tooling: Basic CLI for project setup, but no sophisticated IDE integration beyond standard C development features.

## Architecture Highlights (MVP)

- Proxima Core (C): Contains the UI definition (structs, function pointers), custom layout engine, custom event system, and cross-platform platform abstraction interfaces. This layer is entirely platform-agnostic.

- Proxima Graphics (C): Implements the custom rendering pipeline, translating UI elements into low-level draw commands for the platform-specific graphics APIs.

- Platform Implementation (C / Objective-C / Java / Win32): Concrete implementations that bridge Proxima Core and Proxima Graphics to the specific OS and its raw APIs (e.g., creating a Metal MTKView on iOS, initializing a DirectX device on Windows, setting up an EGL context on Android).

```mermaid
graph TD
    A[Proxima App (C)] --> B[Proxima Core Library (C)]
    B --> C[Proxima Graphics Layer (C / Raw Metal/DX/Vulkan/GLES)]
    B --> D[Proxima Platform Interface (C abstract via function pointers)]
    D -- implemented by --> E{Platform Implementation<br/>(C / Obj-C / Java / Win32)}
    E --> F{OS APIs<br/>(UIKit, Android NDK/SDK, Cocoa, Win32)}
    C --> G{OS Graphics APIs<br/>(Metal, DirectX, Vulkan, OpenGL ES)}
```

## Contributing

This is a true pioneer project. We are seeking passionate and skilled C++ developers eager to dive deep into system-level programming, graphics pipelines, and cross-platform architecture. If you're interested in contributing to the MVP (e.g., defining the UI DSL, implementing basic widgets, setting up platform integrations, writing a Metal renderer, etc.), please open an issue or reach out.

## License

[BSD License](LICENSE)
