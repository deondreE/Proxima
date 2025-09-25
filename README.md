Proxima - Native Cross-Platform App Platform (MVP)


Proxima is a groundbreaking experiment in native cross-platform application development. Our core philosophy is to build a framework from the ground up, entirely in C++, by directly interfacing with low-level system graphics and input APIs. 

Our goal is to redefine how native applications are built, offering unparalleled control, maximum performance, and a truly bespoke native experience across multiple operating systems, leveraging the raw power of each platform's hardware.

## Mission

C++ powers most of the world wether we like it or not. We MUST make the expirence of Application development in the language better and more intuitive.

---

## Building

> **All platforms** 

```bash
git clone <repo_link> --recursive
```

> **Macos**

> Note: there is no code resulting for the Macos Platform this will just not compile.

```bash
# REQUIRES METAL
xcode --select install # something like that

cmake -S . -B build #only first run
cmake --build build
```

> **Windows**

> Note: there is no code resulting for the Windows Platform this will just not compile.


```powershell
# REQUIRED -> DX11 and MSVC 19.40 at least
cmake -S . -B build
cmake --build build
```

> **Arch Linux**

1. Install Dependencies
```bash
sudo pacman -Syu
sudo pacman -S --needed base-devel git cmake clang sdl3 sdl3_ttf
```

2. Build the project
```bash
cmake -S . -B build # for first build only
cmake --build build
./build/example_app
```

---

## Vision (Long-term)

Proxima will have language bindings for all popular languages

- Rust
- Python
- Lua
- Java

This will allow for the platform to reach as many runtimes as possible and there to be no excuse such as "I don't know C++" to ever exist. 

## Usage

```cpp
void on_btn_clicked() {
  std::cout << "Button Clicked!\n";
}

ConfigManager configManager;

int main(int argc, char* argv[]) {

  if (!configManager.load("./examples/config/app_config.yaml")) {
    std::cerr << "Failed to load config\n";
    SDL_Quit();
    TTF_Quit();
    return 1;
  }
  const AppConfig& appConfig = configManager.getAppConfig();

  try {
    WindowConfig windowConfig;
    windowConfig.title = appConfig.window_title;
    windowConfig.initialHeight = appConfig.initial_height;
    windowConfig.initialWidth = appConfig.initial_width;
    windowConfig.resizable = true;
    windowConfig.vsync = true;

    Window mainWindow(windowConfig);

    View& rootView = mainWindow.getRootView();
    auto helloText = std::make_unique<Text>("Welcome to Proxima UI!");
    helloText->size(400, 30)
        .pos(20, 20)
        .setFont("./examples/config/fonts/Delius-Regular.ttf", 28)
        .setColor({50, 50, 50, 255});
    auto backgroundImage = std::make_unique<Image>();
    backgroundImage->pos(0, 0).size(appConfig.initial_width,
                                    appConfig.initial_height);
    backgroundImage->z_index(-100);
    backgroundImage->setImagePath("./examples/assets/test-app.png");

    auto btn = std::make_unique<Button>();
    btn->size(120, 40)
        .pos(10, 60)
        .setFont("./examples/config/fonts/Delius-Regular.ttf", 36)
        .text("Press this button!")
        .onClick(on_btn_clicked);
    btn->setColor({255, 255, 255, 255});

    auto volumeSlider =
        std::make_unique<Slider>(50, 400, 300, 0.0f, 100.0f, 75.0f);
    volumeSlider->setStep(5.0f);
    volumeSlider->setColors({100, 100, 200, 255}, {255, 150, 0, 255});
    volumeSlider->setDimentions(30, 20, 30);
    volumeSlider->onValueChanged([](float newValue) {
      std::cout << "Volume changed to: " << newValue << std::endl;
    });
    volumeSlider->z_index(10);

    rootView.add(std::move(helloText));
    rootView.add(std::move(backgroundImage));
    rootView.add(std::move(btn));
    rootView.add(std::move(volumeSlider));

    mainWindow.run();
  } catch (const std::runtime_error& e) {
    std::cerr << std::format("Application encountered an error: {}\n",
                             e.what());
    return 1;
  }

  std::cout << "Applicatio exited succesfully.";

  return 0;
}
```

## MVP Features

This initial version focuses on demonstrating fundamental, custom-built capabilities:


- Core Applicaiton logic for defining DSL.

- Direct System Graphics API Interaction:
	- Proxima directly utilizes the native, low-level graphics APIs on each platform:
		- iOS/macOS: Metal

		- Windows: DirectX
		- Android: Vulkan

		- Linux: SDL3


- Custom UI Rendering Engine: Proxima implements its own rendering engine for all UI elements. This includes:
	- Vector/Shape Primitives: Drawing lines, rectangles, circles.
	- Text Inputs
	- Layouts
	- Text Rendering

- Procedural/Struct-Based UI Definition

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

- Support for svg, based rendering.

- Complex Widget Library: No sophisticated widgets like lists, grids, rich text editors, date pickers, or complex animations.

- Network Layer: No integrated HTTP client or network abstraction.

- Filesystem Abstraction: Only raw platform-specific file I/O for MVP.

- Native Widget Integration: Proxima renders its entire UI from scratch. It does not wrap or use platform-native UI widgets (e.g., UIButton, NSButton, android.widget.Button, Win32 controls). This is a deliberate design choice for maximum control and consistent rendering.

- Hot Reload/Live Preview: Compilation will be traditional and require a full rebuild.

- Extensive Tooling: Basic CLI for project setup, but no sophisticated IDE integration.

## Contributing

Check out the issues, comment, let us know your progress. Tag this `[AI]` with a comment if generated by AI please.

## License

[BSD License](LICENSE)
