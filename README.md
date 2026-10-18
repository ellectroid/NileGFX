# NileGFX
Embedded 2D Graphics Engine (Painter's Algo, 2-4 Framebuffers)

2D graphics engine for Embedded systems. The example runs on STM32F746-Disco, but the engine logic is platform-agnostic.

Features:
- Painter's algorithm - queue operations to draw things on top of each other
- Primitives: circles, triangles, rectangles
- Textures with or without transparency/blending
- 2-4 Framebuffers (optional cache framebuffer and drawing framebuffer)
- Optional cache framebuffer with manual redraw requests
- Optional destructive operations (delete draw operation after executing)
- Interrupt signals for active and blanking stages
- Support for multiple screens
- 4 configurable draw operations queues per screen, individually configurable queue sizes for every screen
- Shadow registers for reloading configuration between draws

This STM32F746 example implementation has the following features: 
- The engine is running using STM32's DMA2D and LTDC, with hardware-specific code separated
- 3 Framebuffers in external SDRAM
- 2 Textures stored in QSPI Flash
- 1 Texture moving around on screen at runtime
