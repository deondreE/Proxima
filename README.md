# Proxima - Native Cross-Platform App Platform (MVP)

Proxima is an experimental, non-web-based, native cross-platform application development framework built with C++. Our goal is to enable developers to create high-performance, truly native-feeling applications across multiple operating systems from a single codebase, without relying on web technologies or managed runtimes like Electron or Flutter/React Native's JavaScript/Dart engines.

## Mission

To provide a lean, performant, and C++ native foundation for building graphical user interfaces and accessing system functionalities on iOS, Android, macOS, and Windows.

## Vision (Long-term)

Proxima aims to be the go-to choice for developers seeking ultimate control, near-native performance, and a consistent native look-and-feel across their target platforms, while maintaining the productivity benefits of a single-codebase approach.

## MVP Features

This initial version focuses on demonstrating core capabilities:

*   **Core C++ Foundation:** All application logic is written in modern C++.
*   **Declarative UI Syntax:** A C++ DSL (Domain Specific Language) for defining UI components in a declarative style (similar to SwiftUI/Jetpack Compose, but native C++).
*   **Cross-Platform Graphics Rendering:**
    *   **Desktop (macOS/Windows):** Uses **Skia** as the rendering engine for custom, consistent UI elements.
    *   **Mobile (iOS/Android):** Uses **Skia** for consistent custom UI, or potentially direct low-level drawing APIs (Metal/OpenGL ES) if Skia proves too heavy for specific targets. (Initial focus is Skia for consistency).
*   **Basic UI Components:**
    *   `View`: A fundamental drawable and layout-able element.
    *   `Text`: For rendering static text.
    *   `Button`: A clickable interactive element with visual feedback.
    *   `StackLayout`: Basic vertical/horizontal layout container.
*   **Simple Event Handling:**
    *   Tap/Click events for `Button` and `View`.
*   **Platform Abstraction Layer:** Minimal abstraction for:
    *   Window/View management on desktop/mobile.
    *   Input event dispatching.
    *   Basic filesystem access (e.g., read a text file).
*   **Native Build Integration:**
    *   **iOS:** Integrates with Xcode projects, compiling to a native iOS app.
    *   **Android:** Integrates with Gradle/CMake, compiling to a native Android app via NDK.
    *   **macOS:** Standard CMake/Xcode project, compiling to a native macOS app bundle.
    *   **Windows:** Standard CMake/Visual Studio project, compiling to a native Windows executable.

## Non-Goals for MVP

To keep the scope manageable, the following are explicitly *not* part of the MVP:

*   **Complex Widget Library:** No sophisticated widgets like lists, grids, rich text editors, date pickers, etc.
*   **Advanced Layout:** Only basic linear layouts. No Flexbox or AutoLayout equivalents yet.
*   **Animation Engine:** No built-in animation system.
*   **Network Layer:** No integrated HTTP client or network abstraction.
*   **Native Widget Integration:** Proxima renders its own UI. It does *not* wrap or use platform-native UI widgets (e.g., `UIButton`, `NSButton`, `android.widget.Button`). This is a deliberate choice for consistent look and performance.
*   **Hot Reload/Live Preview:** Compilation will be traditional.
*   **Extensive Tooling:** Basic CLI for project setup, but no sophisticated IDE integration.

## Architecture Highlights (MVP)

*   **Core Layer (C++):** Contains the UI DSL, layout engine, event system, and cross-platform platform abstraction interfaces.
*   **Graphics Layer (C++ with Skia):** Implements the rendering of UI components using Skia's drawing primitives.
*   **Platform Layer (C++/Objective-C/Java/Win32):** Concrete implementations of the platform abstraction interfaces, responsible for setting up windows/views, handling native events, and calling into Skia.

```mermaid
graph TD
    A[Proxima App (C++)] --> B[Proxima Core Library (C++)]
    B --> C[Proxima Graphics Layer (C++ / Skia)]
    B --> D[Proxima Platform Layer (C++ / Obj-C / Java / Win32)]
    D --> E{Platform SDKs<br/>(UIKit, Android NDK/SDK, Cocoa, Win32)}
    C --> F{Skia Graphics Library}
```

## How to Build (Conceptual)

*(Note: Specific instructions will be detailed once the project is structured)*

1.  **Prerequisites:**
    *   CMake 3.15+
    *   C++17 capable compiler (Clang/GCC for Linux/macOS/Android/iOS, MSVC for Windows)
    *   Platform-specific SDKs (Xcode for iOS/macOS, Android NDK/SDK, Visual Studio for Windows)
    *   Skia Library (pre-built or built from source)
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
    ./path/to/proxima-example-app
    ```

## Contributing

This is a ground-up initiative. We welcome collaborators passionate about native performance and low-level cross-platform development. If you're interested in contributing to the MVP (e.g., defining the UI DSL, implementing basic widgets, setting up platform integrations), please open an issue or reach out.

## License

[MIT License](LICENSE) (or your chosen license)
