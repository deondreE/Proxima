# Proxima - Native Cross-Platform App Platform (MVP)

Proxima is a **groundbreaking experiment** in native cross-platform application development. Our core philosophy is to build a framework from the ground up, entirely in C++, by directly interfacing with low-level system graphics and input APIs. We explicitly **avoid all third-party UI toolkits, rendering engines like Skia, and higher-level graphics abstraction libraries (e.g., SDL, SFML, wgpu)**.

Our goal is to redefine how native applications are built, offering unparalleled control, maximum performance, and a truly bespoke native experience across multiple operating systems, leveraging the raw power of each platform's hardware.

## Mission

To forge a lean, C++ native foundation for building graphical user interfaces and accessing essential system functionalities on iOS, Android, macOS, and Windows. We aim to achieve this by directly interacting with system-level graphics APIs (Metal, DirectX, Vulkan, OpenGL ES) and implementing our entire UI and event system from first principles.

## Vision (Long-term)

Proxima envisions a future where developers can craft desktop and mobile applications with complete ownership over every pixel and every event, achieving unparalleled performance, a unique aesthetic, and deep system integration, all from a single, C++-driven codebase that rethinks traditional application architecture.

## MVP Features

This initial version focuses on demonstrating fundamental, custom-built capabilities:

*   **Core C++ Foundation:** All application logic is written in modern C++ (C++17/20) and compiled directly to native machine code. We leverage the C++ Standard Library for fundamental data structures and algorithms, but avoid extensive third-party helper libraries.
*   **Direct System Graphics API Interaction:**
    *   Proxima directly utilizes the native, low-level graphics APIs on each platform:
        *   **iOS/macOS:** Metal
        *   **Windows:** DirectX (e.g., DX11 or DX12 for rendering)
        *   **Android:** OpenGL ES / Vulkan
        *   **(Potential future desktop Linux):** Vulkan / OpenGL
    *   Our rendering pipeline, from vertex buffers to shaders, is custom-built on top of these APIs.
*   **Custom UI Rendering Engine:** Proxima implements its own rendering engine for all UI elements. This includes:
    *   **Vector/Shape Primitives:** Drawing lines, rectangles, circles, and other basic shapes directly via the chosen graphics API.
    *   **Text Rendering:** Custom glyph rasterization and atlas management (or highly optimized OS-provided font APIs for glyph data) directly drawn using the graphics API.
*   **Declarative UI Syntax:** A C++ DSL (Domain Specific Language) for defining UI components in a declarative style (e.g., `Button().text("Click Me").onClick(...)`).
*   **Basic UI Components:**
    *   `View`: The fundamental drawable and layout-able element.
    *   `Text`: For rendering static text.
    *   `Button`: A clickable interactive element with visual feedback (hover, press states), drawn purely by Proxima.
    *   `StackLayout`: Basic vertical/horizontal layout container, implemented from scratch.
*   **Direct Input Handling:** Raw keyboard, mouse, and touch events are captured directly from the OS-level event loops and processed by Proxima's custom event system.
*   **Minimal Platform Abstraction Layer:** Custom, thin C++ wrappers for:
    *   Creating and managing OS-level windows/views/surfaces.
    *   Setting up graphics contexts/devices (e.g., Metal `MTLCommandQueue`, DXGI Swap Chain).
    *   Receiving raw input events (keyboard, mouse, touch) directly.
    *   Accessing essential system APIs (e.g., getting screen resolution, basic timing, memory allocation hooks if needed).
*   **Native Build Integration:**
    *   **iOS:** Integrates with Xcode projects, compiling to a native iOS app.
    *   **Android:** Integrates with Gradle/CMake, compiling to a native Android app via NDK.
    *   **macOS:** Standard CMake/Xcode project, compiling to a native macOS app bundle.
    *   **Windows:** Standard CMake/Visual Studio project, compiling to a native Windows executable.

## Non-Goals for MVP

To maintain a focused scope for this initial, ambitious effort:

*   **Complex Widget Library:** No sophisticated widgets like lists, grids, rich text editors, date pickers, or complex animations.
*   **Advanced Layout:** Only basic linear layouts. No Flexbox or AutoLayout equivalents yet.
*   **Network Layer:** No integrated HTTP client or network abstraction.
*   **Filesystem Abstraction:** Only raw platform-specific file I/O for MVP.
*   **Native Widget Integration:** Proxima renders its *entire* UI from scratch. It does *not* wrap or use platform-native UI widgets (e.g., `UIButton`, `NSButton`, `android.widget.Button`, Win32 controls). This is a deliberate design choice for maximum control and consistent rendering.
*   **Hot Reload/Live Preview:** Compilation will be traditional and require a full rebuild.
*   **Extensive Tooling:** Basic CLI for project setup, but no sophisticated IDE integration beyond standard C++ development features.

## Architecture Highlights (MVP)

*   **Proxima Core (C++):** Contains the UI DSL, custom layout engine, custom event system, and cross-platform platform abstraction interfaces. This layer is entirely platform-agnostic.
*   **Proxima Graphics (C++):** Implements the custom rendering pipeline, translating UI elements into low-level draw commands for the platform-specific graphics APIs.
*   **Platform Implementation (C++ / Objective-C / Java / Win32):** Concrete implementations that bridge `Proxima Core` and `Proxima Graphics` to the specific OS and its raw APIs (e.g., creating a Metal `MTKView` on iOS, initializing a DirectX device on Windows, setting up an EGL context on Android).

```mermaid
graph TD
    A[Proxima App (C++)] --> B[Proxima Core Library (C++)]
    B --> C[Proxima Graphics Layer (C++ / Raw Metal/DX/Vulkan/GLES)]
    B --> D[Proxima Platform Interface (C++ abstract)]
    D -- implemented by --> E{Platform Implementation<br/>(C++ / Obj-C / Java / Win32)}
    E --> F{OS APIs<br/>(UIKit, Android NDK/SDK, Cocoa, Win32)}
    C --> G{OS Graphics APIs<br/>(Metal, DirectX, Vulkan, OpenGL ES)}
```

## How to Build (Conceptual)

*(Note: Specific, highly detailed instructions will be provided once the project is structured and initial platform integrations are working)*

1.  **Prerequisites:**
    *   CMake 3.15+ (for cross-platform build system generation)
    *   C++17/20 capable compiler (Clang/GCC for Linux/macOS/Android/iOS, MSVC for Windows)
    *   Platform-specific SDKs/Toolchains:
        *   Xcode for iOS/macOS development
        *   Android NDK & SDK for Android development
        *   Visual Studio (with C++ workloads) for Windows development
2.  **Clone Repository:**
    ```bash
    git clone https://github.com/deondreenglish/proxima.git
    cd proxima
    ```
3.  **Build Steps (Example - macOS Desktop):**
    ```bash
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DPROXIMA_BUILD_TARGET=macOS_Desktop
    cmake --build .
    ```
4.  **Run Sample App:**
    ```bash
    ./path/to/proxima-example-app # Output executable location will vary by platform
    ```

## Contributing

This is a true pioneer project. We are seeking passionate and skilled C++ developers eager to dive deep into system-level programming, graphics pipelines, and cross-platform architecture. If you're interested in contributing to the MVP (e.g., defining the UI DSL, implementing basic widgets, setting up platform integrations, writing a Metal renderer, etc.), please open an issue or reach out.

## License

[MIT License](LICENSE) (or your chosen license)
