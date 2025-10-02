# Proxima - Native Cross-Platform App Platform (MVP)

Proxima is a simple UI library aiming to make the application layer a less dependent process.

---

## Vision (Long-term)

Proxima will have language bindings for all popular languages

- Rust
- Python
- Zig
- Odin
- Lua

This will allow for the platform to reach as many runtimes as possible and there to be no excuse such as "I don't know C++" to ever exist.

The Proxima way of rendering is super interesting:

We render everything for you at once in DEBUG only, then once you push you application to production the entire UI is chached in a .yaml format allowing for simple:

- Modding
- Editing

And this also allows for super fast runtime performance when outside of a debug environment..

This will eventually support every platform at the application layer... We are looking at support "Game Style" User interfaces in the future. This include but are not limited to:

- Viewports. **Both multiple, and single**
- Docking
- Quad rendering for inside games.

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

    - Windows: WIN32
    - Linux: SDL3

- Custom UI Rendering Engine: Proxima implements its own rendering engine for all UI elements. This includes:

  - Vector/Shape Primitives: Drawing lines, rectangles, circles.
  - Text Inputs
  - Layouts
  - Text Rendering

- Minimal Platform Abstraction Layer: Custom, thin C wrappers for:

  - Creating and managing OS-level windows/views/surfaces.

  - Setting up graphics contexts/devices (e.g., Metal MTLCommandQueue, DXGI Swap Chain).

  - Receiving raw input events (keyboard, mouse, touch) directly.

  - Accessing essential system APIs (e.g., getting screen resolution, basic timing, memory allocation hooks if needed).

- Native Build Integration:
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
[Contributing Layout](./docs/Contributing.md)

## License

[BSD License](LICENSE)
