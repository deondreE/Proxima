# Contributing

Currently we are open for contributions, but we ask that you take some time to read this document laying out the specification of the project.

## Build System

CMake -- Don't request to change it at all your request will be deined.

Why CMake -- Because I couldn't use make...

## Viewport

A viewport is NOT a window. Don't try to combine them. At the same right the viewport should not depend on Proxima, but on the end user.
We should not have to render, something like a depending viewport unless the developer doesn't provide information ie: Renderer up front.

Even then the capabilities will be limited because Proxima is not a Rendering lib.

Limited to:

- Text in the viewport.
- Image quad in viewport.
