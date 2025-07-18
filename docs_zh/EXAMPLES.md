# Dear ImGui: 示例

_(您可以在 https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md 浏览此文件，或使用任何 Markdown 查看器查看此文件)_

**`examples/` 文件夹为各种平台和图形 API 提供了示例应用程序（独立、可直接构建）。** 它们都使用了 `backends/` 文件夹中的标准后端（参见 `docs/BACKENDS.md`）。

示例的目的是展示如何与后端集成，让您试用 Dear ImGui，并指导您将 Dear ImGui 集成到您自己的应用程序/游戏/引擎中。
**一旦 Dear ImGui 设置并运行，请运行并参考 `imgui_demo.cpp` 中的 `ImGui::ShowDemoWindow()` 以了解最终用户 API 的用法。**

您可以在以下地址找到其中一些示例应用程序的 Windows 二进制文件：
  https://www.dearimgui.com/binaries

### 入门

在典型的现有应用程序中，使用标准后端进行集成，应该只需要不到20行代码。

```cpp
// 初始化时：
ImGui::CreateContext();
// 为每个后端调用 ImGui_ImplXXXX_Init()

// 在你的帧开始时：
// 为每个后端调用 ImGui_ImplXXXX_NewFrame()
ImGui::NewFrame();

// 在你的帧结束时：
ImGui::Render();
// 为你的渲染器后端调用 ImGui_ImplXXXX_RenderDrawData()

// 关闭时：
// 为每个后端调用 ImGui_ImplXXXX_Shutdown()
ImGui::DestroyContext();
```

**主要资源：**
- 阅读 **[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started) Wiki**，获取关于如何将 Dear ImGui 集成到现有应用程序中的详细示例。

**其他资源：**
- 阅读 FAQ：https://www.dearimgui.com/faq
- 阅读 `imgui.cpp` 中的“程序员指南”部分。
- 阅读每个文件顶部的注释和说明。

如果您正在使用此处提供的任何后端，您可以将 `backends/imgui_impl_xxxx(.cpp,.h)` 文件添加到您的项目中并直接使用。每个 `imgui_impl_xxxx.cpp` 文件都有其独立的变更日志，因此如果您以后想更新它们，会更容易了解发生了什么变化。

### 示例应用程序

[example_allegro5/](https://github.com/ocornut/imgui/blob/master/examples/example_allegro5/) <BR>
Allegro 5 示例。 <BR>
= `main.cpp` + `imgui_impl_allegro5.cpp`

[example_android_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_android_opengl3/) <BR>
Android + OpenGL3 (ES) 示例。 <BR>
= `main.cpp` + `imgui_impl_android.cpp` + `imgui_impl_opengl3.cpp`

[example_apple_metal/](https://github.com/ocornut/imgui/tree/master/examples/example_apple_metal/) <BR>
OSX & iOS + Metal 示例。 <BR>
= `main.m` + `imgui_impl_osx.mm` + `imgui_impl_metal.mm` <BR>
它基于 Xcode 9 提供的“跨平台”游戏模板。
(注意：`imgui_impl_osx.mm` 目前的功能不如其他平台后端完整。您可能更愿意使用 GLFW 或 SDL 后端，它们也支持 Windows 和 Linux。)

[example_apple_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_apple_opengl2/) <BR>
OSX + OpenGL2 示例。 <BR>
= `main.mm` + `imgui_impl_osx.mm` + `imgui_impl_opengl2.cpp` <BR>
(注意：`imgui_impl_osx.mm` 目前的功能不如其他平台后端完整。您可能更愿意使用 GLFW 或 SDL 后端，它们也支持 Windows 和 Linux。)

[example_glfw_wgpu/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_wgpu/) <BR>
GLFW + WebGPU 示例。支持 Emscripten (web) 或 Dawn (desktop) <BR>
= `main.cpp` + `imgui_impl_glfw.cpp` + `imgui_impl_wgpu.cpp`
注意，`example_glfw_opengl3` 和 `example_sdl2_opengl3` 示例也支持 Emscripten！

[example_glfw_metal/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_metal/) <BR>
GLFW (Mac) + Metal 示例。 <BR>
= `main.mm` + `imgui_impl_glfw.cpp` + `imgui_impl_metal.mm`

[example_glfw_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl2/) <BR>
GLFW + OpenGL2 示例 (旧版，固定管线)。 <BR>
= `main.cpp` + `imgui_impl_glfw.cpp` + `imgui_impl_opengl2.cpp` <BR>
**如果您的代码/引擎正在使用现代 GL 或 WEBGL (着色器, VBO, VAO 等)，请不要使用此示例。** <BR>
提供此代码主要是作为学习 Dear ImGui 集成的参考，因为它更短。如果您的代码使用 GL3+ 上下文或任何半现代的 GL 调用，使用此渲染器可能会使事情变得更复杂，需要您的代码将许多 GL 属性重置为其初始状态，并可能混淆您的 GPU 驱动程序。一星，不推荐。

[example_glfw_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/) <BR>
GLFW (Win32, Mac, Linux) + OpenGL3+/ES2/ES3 示例 (现代，可编程管线)。 <BR>
= `main.cpp` + `imgui_impl_glfw.cpp` + `imgui_impl_opengl3.cpp` <BR>
这使用了更现代的 GL 调用和自定义着色器。<BR>
它支持使用 Emscripten 构建并以 WebGL 为目标。<BR>
如果您在应用程序中使用现代 GL 或 WebGL，请优先使用此示例。

[example_glfw_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_vulkan/) <BR>
GLFW (Win32, Mac, Linux) + Vulkan 示例。 <BR>
= `main.cpp` + `imgui_impl_glfw.cpp` + `imgui_impl_vulkan.cpp` <BR>
这个示例相当长且繁琐，因为：Vulkan。
对于此示例，`main.cpp` 文件特别使用了来自 `imgui_impl_vulkan.h/cpp` 的辅助函数。

[example_glut_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_glut_opengl2/) <BR>
GLUT (例如，Linux/Windows 上的 FreeGLUT，OSX 上的 GLUT 框架) + OpenGL2 示例。 <BR>
= `main.cpp` + `imgui_impl_glut.cpp` + `imgui_impl_opengl2.cpp` <BR>
请注意，GLUT/FreeGLUT 在很大程度上是过时的软件，建议使用 GLFW 或 SDL。

[example_null/](https://github.com/ocornut/imgui/blob/master/examples/example_null/) <BR>
空示例，编译并链接 imgui，创建上下文，以无头模式运行，没有输入和图形输出。 <BR>
= `main.cpp` <BR>
这用于在尽可能多的设置中快速测试核心 imgui 文件的编译。
因为此应用程序不创建窗口或图形上下文，所以没有图形输出。

[example_sdl2_directx11/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_directx11/) <BR>
SDL2 + DirectX11 示例，仅限 Windows。 <BR>
= `main.cpp` + `imgui_impl_sdl2.cpp` + `imgui_impl_dx11.cpp` <BR>
这用于演示如何在 SDL2 中使用 DirectX。

[example_sdl2_metal/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_metal/) <BR>
SDL2 + Metal 示例，仅限 Mac。 <BR>
= `main.mm` + `imgui_impl_sdl2.cpp` + `imgui_impl_metal.mm`

[example_sdl2_opengl2/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl2/) <BR>
SDL2 (Win32, Mac, Linux 等) + OpenGL 示例 (旧版，固定管线)。 <BR>
= `main.cpp` + `imgui_impl_sdl2.cpp` + `imgui_impl_opengl2.cpp` <BR>
**如果您的代码/引擎正在使用 GL 或 WEBGL (着色器, VBO, VAO 等)，请不要使用 OPENGL2 代码。** <BR>
提供此代码主要是作为学习 Dear ImGui 集成的参考，因为它更短。如果您的代码使用 GL3+ 上下文或任何半现代的 GL 调用，使用此渲染器可能会使事情变得更复杂，需要您的代码将许多 GL 属性重置为其初始状态，并可能混淆您的 GPU 驱动程序。一星，不推荐。

[example_sdl2_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl3/) <BR>
SDL2 (Win32, Mac, Linux 等) + OpenGL3+/ES2/ES3 示例。 <BR>
= `main.cpp` + `imgui_impl_sdl2.cpp` + `imgui_impl_opengl3.cpp` <BR>
这使用了更现代的 GL 调用和自定义着色器。 <BR>
它支持使用 Emscripten 构建并以 WebGL 为目标。<BR>
如果您在应用程序中使用现代 GL 或 WebGL，请优先使用此示例。

[example_sdl2_sdlrenderer2/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_sdlrenderer2/) <BR>
SDL2 (Win32, Mac, Linux 等) + SDL_Renderer for SDL2 示例。<BR>
= `main.cpp` + `imgui_impl_sdl2.cpp` + `imgui_impl_sdlrenderer2.cpp` <BR>
这需要 SDL 2.0.18+ (2021年11月发布) <BR>

[example_sdl2_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_vulkan/) <BR>
SDL2 (Win32, Mac, Linux 等) + Vulkan 示例。 <BR>
= `main.cpp` + `imgui_impl_sdl2.cpp` + `imgui_impl_vulkan.cpp` <BR>
这个示例相当长且繁琐，因为：Vulkan。 <BR>
对于此示例，`main.cpp` 文件特别使用了来自 `imgui_impl_vulkan.h/cpp` 的辅助函数。

[example_sdl3_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_opengl3/) <BR>
SDL3 (Win32, Mac, Linux 等) + OpenGL3+/ES2/ES3 示例。 <BR>
= `main.cpp` + `imgui_impl_sdl3.cpp` + `imgui_impl_opengl3.cpp` <BR>
这使用了更现代的 GL 调用和自定义着色器。 <BR>
它支持使用 Emscripten 构建并以 WebGL 为目标。<BR>

[example_sdl3_sdlgpu3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_sdlgpu3/) <BR>
SDL3 (Win32, Mac, Linux 等) + SDL_GPU for SDL3 示例。<BR>
= `main.cpp` + `imgui_impl_sdl3.cpp` + `imgui_impl_sdlrenderer3.cpp` <BR>

[example_sdl3_sdlrenderer3/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_sdlrenderer3/) <BR>
SDL3 (Win32, Mac, Linux 等) + SDL_Renderer for SDL3 示例。<BR>
= `main.cpp` + `imgui_impl_sdl3.cpp` + `imgui_impl_sdlrenderer3.cpp` <BR>

[example_sdl3_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_vulkan/) <BR>
SDL3 (Win32, Mac, Linux 等) + Vulkan 示例。 <BR>
= `main.cpp` + `imgui_impl_sdl3.cpp` + `imgui_impl_vulkan.cpp` <BR>
这个示例相当长且繁琐，因为：Vulkan。 <BR>
对于此示例，`main.cpp` 文件特别使用了来自 `imgui_impl_vulkan.h/cpp` 的辅助函数。

[example_win32_directx9/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx9/) <BR>
DirectX9 示例，仅限 Windows。 <BR>
= `main.cpp` + `imgui_impl_win32.cpp` + `imgui_impl_dx9.cpp`

[example_win32_directx10/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx10/) <BR>
DirectX10 示例，仅限 Windows。 <BR>
= `main.cpp` + `imgui_impl_win32.cpp` + `imgui_impl_dx10.cpp`

[example_win32_directx11/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx11/) <BR>
DirectX11 示例，仅限 Windows。 <BR>
= `main.cpp` + `imgui_impl_win32.cpp` + `imgui_impl_dx11.cpp`

[example_win32_directx12/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx12/) <BR>
DirectX12 示例，仅限 Windows。 <BR>
= `main.cpp` + `imgui_impl_win32.cpp` + `imgui_impl_dx12.cpp` <BR>
这个示例相当长且繁琐，因为：DirectX12。

[example_win32_opengl3/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_opengl3/) <BR>
原生 Windows + OpenGL3 示例 (现代，可编程管线) <BR>
= `main.cpp` + `imgui_impl_win32.cpp` + `imgui_impl_opengl3.cpp` <BR>

[example_win32_vulkan/](https://github.com/ocornut/imgui/blob/master/examples/example_win32_vulkan/) <BR>
原生 Windows + Vulkan 示例 <BR>
= `main.cpp` + `imgui_impl_win32.cpp` + `imgui_impl_vulkan.cpp` <BR>

### 其他

**构建**

不幸的是，如今在不依赖第三方软件和构建系统的情况下，创建和维护使用外部库（我们在这里用来创建窗口和渲染3D三角形的库）的可移植构建文件仍然很繁琐。对于这里的大多数示例，我们选择提供：
 - 适用于 Linux/OSX 的 Makefile
 - 适用于 Visual Studio 2008+ 的批处理文件
 - 适用于 Visual Studio 2012+ 的 .sln 项目文件
 - 适用于 Apple 示例的 Xcode 项目文件
如果它们在您的设置中不起作用，请告诉我们！
您可能只需将 `imgui_impl_xxx.cpp/.h` 文件导入到您自己的代码库中，或直接使用命令行编译器编译它们。

如果您有兴趣使用 Cmake 来构建和链接示例，请参阅：
  https://github.com/ocornut/imgui/pull/1713 和 https://github.com/ocornut/imgui/pull/3027

**关于鼠标光标延迟**

对于大多数行为，Dear ImGui 不会引入明显的额外延迟，例如，在 `NewFrame()` 之前传递给 `io.AddMousePosEvent()` 的最后一个值将导致窗口在 `EndFrame()/Render()` 时移动到正确的位置。在 60 FPS 下，您的体验应该是愉快的。

但是，请考虑操作系统鼠标光标通常通过非常特定的硬件加速路径进行渲染，这使得它们比通过常规图形 API 渲染的大多数内容（包括但不限于 Dear ImGui 窗口）感觉更平滑。因为 UI 渲染和交互与鼠标在同一平面上发生，所以这种脱节可能会让特别敏感的用户感到不适。
您可以尝试启用 `io.MouseDrawCursor` 标志，以请求 Dear ImGui 使用常规图形 API 绘制鼠标光标，以帮助您可视化“硬件”光标和常规渲染的软件光标之间的差异。
然而，以 60 FPS 渲染鼠标光标会感觉迟钝，因此您可能不希望一直启用它。*仅*在进行交互式拖动时切换到软件渲染的光标可能对用户体验有益。

请注意，某些设置配置或 GPU 驱动程序可能会根据其设置引入额外的显示延迟。
如果您注意到拖动窗口有延迟，并且不确定原因是什么：请尝试在鼠标光标正下方直接绘制一个简单的 2D 形状，以帮助识别问题！
