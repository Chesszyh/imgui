# Dear ImGui: 后端

_(您可以在 https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md 浏览此文件，或使用任何 Markdown 查看器查看此文件)_

## 目录

- [简介](#简介)
   - [入门](#入门)
   - [什么是后端？](#什么是后端)
- [使用标准后端](#使用标准后端)
- [使用第三方后端](#使用第三方后端)
- [编写自己的后端](#编写自己的后端)
  - [使用自定义引擎？](#使用自定义引擎)
  - [平台：实现您的平台后端](#平台实现您的平台后端)
  - [渲染：实现您的 RenderDrawData 函数](#渲染实现您的renderdrawdata函数)
  - [渲染：添加对 `ImGuiBackendFlags_RendererHasTextures` 的支持 (1.92+)](#渲染添加对-imguibackendflags_rendererhastextures-的支持-192)

## 简介

### 入门

💡 **[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started) Wiki** 提供了如何将 Dear ImGui 集成到现有应用程序中的示例。
<BR> [EXAMPLES.MD](https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md) 文档也值得一读。

### 什么是后端？

Dear ImGui 具有高度可移植性，只需要一些东西即可运行和渲染，通常包括：

- **必需**：提供鼠标/键盘输入（输入到 `ImGuiIO` 结构中）。
- **必需**：创建、更新和销毁纹理。
- **必需**：使用剪裁矩形渲染带索引的纹理三角形。

额外的功能是可选的，我们的后端会尽可能多地支持它们：

- 可选：自定义纹理绑定支持。
- 可选：剪贴板支持。
- 可选：游戏手柄支持。
- 可选：鼠标光标形状支持。
- 可选：输入法编辑器（IME）支持。
- 可选：多视口支持。
- 等等。

这基本上就是每个后端所做的事情，外加一些强制性的可移植性代码。使用标准后端可确保您获得所有这些功能，包括那些在您这边更难实现的功能（例如多视口支持）。

理解核心 Dear ImGui 库（根目录中的文件）和我们在此描述的后端（`backends/` 文件夹）之间的区别非常重要。

- 某些问题可能仅限于后端或平台。
- 您应该能够为几乎任何平台和任何 3D 图形 API 编写后端。
  例如，您可以发挥创意，使用软件渲染或在不同的机器上进行远程渲染。

## 使用标准后端

**`backends/` 文件夹包含了适用于流行平台/图形 API 的后端，您可以在您的应用程序或引擎中使用它们来轻松集成 Dear ImGui。** 每个后端通常自包含在一对文件中：`imgui_impl_XXXX.cpp` + `imgui_impl_XXXX.h`。

- **“平台”后端**负责：鼠标/键盘/游戏手柄输入、光标形状、计时和窗口管理。
  例如：Windows ([imgui_impl_win32.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_win32.cpp)), SDL3 ([imgui_impl_sdl3.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_sdl3.cpp)), GLFW ([imgui_impl_glfw.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_glfw.cpp)) 等。

- **“渲染器”后端**负责：创建图集纹理和渲染 ImGui 绘制数据。
  例如：DirectX11 ([imgui_impl_dx11.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_dx11.cpp)), OpenGL/WebGL ([imgui_impl_opengl3.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_opengl3.cpp)), Vulkan ([imgui_impl_vulkan.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_vulkan.cpp)) 等。

- 对于某些高级框架，单个后端通常同时处理“平台”和“渲染器”部分。
  例如：Allegro 5 ([imgui_impl_allegro5.cpp](https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_allegro5.cpp))。如果您最终为您的引擎创建自定义后端，您可能也想这样做。

一个应用程序通常结合一个平台后端 + 一个渲染器后端 + Dear ImGui 主要源代码。
例如，[example_win32_directx11](https://github.com/ocornut/imgui/tree/master/examples/example_win32_directx11) 应用程序结合了 `imgui_impl_win32.cpp` + `imgui_impl_dx11.cpp`。在 `examples/` 文件夹中有 20 多个示例。有关详细信息，请参阅 [EXAMPLES.MD](https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md)。

**一旦 Dear ImGui 设置并运行，请运行并参考 `imgui_demo.cpp` 中的 `ImGui::ShowDemoWindow()` 以了解最终用户 API 的用法。**

### 标准后端列表

在 `backends/` 文件夹中：

**平台后端列表：**

    imgui_impl_android.cpp      ; Android 原生应用 API
    imgui_impl_glfw.cpp         ; GLFW (Windows, macOS, Linux 等) http://www.glfw.org/
    imgui_impl_osx.mm           ; macOS 原生 API (功能不如 glfw/sdl 后端完整)
    imgui_impl_sdl2.cpp         ; SDL2 (Windows, macOS, Linux, iOS, Android) https://www.libsdl.org
    imgui_impl_sdl3.cpp         ; SDL3 (Windows, macOS, Linux, iOS, Android) https://www.libsdl.org
    imgui_impl_win32.cpp        ; Win32 原生 API (Windows)
    imgui_impl_glut.cpp         ; GLUT/FreeGLUT (这是史前软件，今天绝对不推荐！)

**渲染器后端列表：**

    imgui_impl_dx9.cpp          ; DirectX9
    imgui_impl_dx10.cpp         ; DirectX10
    imgui_impl_dx11.cpp         ; DirectX11
    imgui_impl_dx12.cpp         ; DirectX12
    imgui_impl_metal.mm         ; Metal (ObjC 或 C++)
    imgui_impl_opengl2.cpp      ; OpenGL 2 (旧版固定管线。不要与现代 OpenGL 代码一起使用！)
    imgui_impl_opengl3.cpp      ; OpenGL 3/4, OpenGL ES 2/3, WebGL
    imgui_impl_sdlgpu3.cpp      ; SDL_GPU (SDL3 的可移植 3D 图形 API)
    imgui_impl_sdlrenderer2.cpp ; SDL_Renderer (SDL2 的可选组件，从 SDL 2.0.18+ 开始提供)
    imgui_impl_sdlrenderer3.cpp ; SDL_Renderer (SDL3 的可选组件。建议使用 SDL_GPU！)
    imgui_impl_vulkan.cpp       ; Vulkan
    imgui_impl_wgpu.cpp         ; WebGPU (网页 + 桌面)

**高级框架后端列表（结合平台 + 渲染器）：**

    imgui_impl_allegro5.cpp

Emscripten 也受支持！
SDL2+GL、SDL3+GL、GLFW+GL 和 GLFW+WebGPU 示例都已准备好使用 Emscripten 构建和运行。

### 推荐的后端

如果您不确定使用哪个后端，以下是推荐用于可移植应用程序的平台/框架：

|库 |网站 |后端 |注意 |
|--------|--------|--------|-----|
| SDL3 | https://www.libsdl.org | imgui_impl_sdl3.cpp | 推荐 |
| SDL2 | https://www.libsdl.org | imgui_impl_sdl2.cpp | |
| GLFW | https://github.com/glfw/glfw | imgui_impl_glfw.cpp | |
| Sokol | https://github.com/floooh/sokol | [util/sokol_imgui.h](https://github.com/floooh/sokol/blob/master/util/sokol_imgui.h) | 比 GLFW/SDL 更底层 |

如果您的应用程序在 Windows 上运行，或者您正在使用多视口，win32 后端在处理某些细节方面比其他后端稍好。

## 使用第三方后端

请参阅 https://github.com/ocornut/imgui/wiki/Bindings 获取完整列表（例如 Adventure Game Studio, Cinder, Cocos2d-x, Game Maker Studio2, Godot, LÖVE+LUA, Magnum, Monogame, Ogre, openFrameworks, OpenSceneGraph, SFML, Sokol, Unity, Unreal Engine 等等）。

## 编写自己的后端

### 使用自定义引擎？

您很可能会想开始使用您自己的自定义/高级工具重写自己的后端……
**三思而后行！**

**长话短说：**
- 编写自己的渲染器后端很容易。
- 编写自己的平台后端更难，而且您更有可能引入错误。
- **通过创建自己的后端，您不太可能为您的项目增加价值。**

**考虑按原样使用现有的后端。**
您将节省大量集成库的时间。
标准后端经过实战检验，并处理了您很可能实现不正确的细微之处。
如果您真的需要，可以**稍后**决定自己重写一个自定义后端。
在大多数情况下，自定义后端的功​​能比我们提供的标准后端少，bug 也更多。
如果您想要可移植性，您可以使用多个后端，并在编译时或运行时在它们之间进行选择。

**示例 A**：您的引擎构建在 Windows + DirectX11 之上，但您在 DirectX11 之上拥有自己的高级渲染系统。
建议：首先尝试使用 `imgui_impl_win32.cpp` + `imgui_impl_dx11.cpp`。
一旦它工作正常，如果您真的需要，您可以用使用您自己的渲染函数的自定义渲染器替换 `imgui_impl_dx11.cpp` 代码，并继续使用标准的 Win32 代码等。

**示例 B**：您的引擎在 Windows、Mac、Linux 上运行，并分别使用 DirectX11、Metal 和 Vulkan。
建议：使用多个通用后端！
一旦它工作正常，如果您真的需要，您可以用您自己的抽象替换后端的部分内容。

**示例 C**：您的引擎在我们无法提供公共后端的平台上运行（例如 PS4/PS5、Switch），并且您到处都有高级系统。
建议：首先尝试使用非可移植后端（例如 win32 + 底层图形 API）来让您的桌面版本首先工作。这将使您更快地运行并熟悉 Dear ImGui 的工作和设置方式。然后，您可以使用您自己的引擎 API 重写一个自定义后端……

另外：
`docking` 分支的[多视口功能](https://github.com/ocornut/imgui/wiki/Multi-Viewports)允许 Dear ImGui 窗口从主应用程序窗口无缝分离。这是通过在平台和渲染器后端添加一个额外的层来实现的，该层允许 Dear ImGui 传达特定于平台的请求，例如：“创建一个额外的操作系统窗口”、“创建一个渲染上下文”、“获取此窗口的操作系统位置”，但有些事情更困难，“查找鼠标位置下的操作系统窗口，但某些窗口标记为穿透”。有关详细信息，请参阅 `ImGuiPlatformIO`。
使用 100% 您自己的抽象正确支持多视口功能比支持单视口更困难。
如果您决定使用未经修改的 `imgui_impl_XXXX.cpp` 文件，您可以自动受益于与视口和平台窗口相关的改进和修复，而无需在您这边进行额外的工作。

### 平台：实现您的平台后端

`impl_impl_XXX.cpp` 文件中的平台后端包含许多实现。

**在您的 `ImGui_ImplXXX_Init()` 函数中：**
- 您可以分配后端数据并使用 `io.BackendPlatformUserData` 在以后存储/检索它。
- 将 `io.BackendPlatformName` 设置为一个名称，例如 `"imgui_impl_xxxx"`，它将在“关于”框等中可用。
- 将 `io.BackendPlatformUserData` 设置为您的后端数据。
- 使用支持的可选功能设置 `io.BackendFlags`：
  - `ImGuiBackendFlags_HasGamepad`：支持游戏手柄并且当前已连接一个。
  - `ImGuiBackendFlags_HasMouseCursors`：支持遵循 `GetMouseCursor()` 值来更改操作系统光标形状。
  - `ImGuiBackendFlags_HasSetMousePos`：支持 `io.WantSetMousePos` 请求以重新定位操作系统鼠标位置（仅当设置了 `io.ConfigNavMoveSetMousePos` 时使用）。
  - `ImGuiBackendFlags_PlatformHasViewports`：支持多个视口。（仅限多视口）
  - `ImGuiBackendFlags_HasMouseHoveredViewport`：支持使用鼠标悬停的视口调用 `io.AddMouseViewportEvent()`。如果可能，请忽略带有 `ImGuiViewportFlags_NoInputs` 标志的视口。如果无法做到这一点，Dear ImGui 需要使用有缺陷的启发式方法来查找鼠标位置下的视口，因为它不知道外部窗口。（仅限多视口）

**在您的 `ImGui_ImplXXX_NewFrame()` 函数中：**
- 将 `io.DeltaTime` 设置为自上一帧以来经过的时间（以秒为单位）。
- 将 `io.DisplaySize` 设置为您的窗口大小。
- 将 `io.DisplayFrameBufferSize` 设置为您的窗口像素密度（仅限 macOS/iOS）。
- 如果支持，更新鼠标光标形状。

**在您的 `ImGui_ImplXXX_NewFrame()` 函数或事件处理程序中：**
- **鼠标支持**
  - 使用 `io.AddMousePosEvent()`、`io.AddMouseButtonEvent()`、`io.AddMouseWheelEvent()` 传递鼠标事件。
  - 如果您能够区分鼠标、触摸屏和笔输入，请使用 `io.AddMouseSourceEvent()`。触摸屏和笔输入需要 Dear ImGui 的某些功能使用不同的逻辑。
  - 使用 `io.AddMouseViewportEvent()` 指定鼠标悬停在哪个视口/操作系统窗口上。请仔细阅读说明，因为这并不像看起来那么简单！（仅限多视口）
- **键盘支持**
  - 使用 `io.AddKeyEvent()` 传递按键事件。
  - 使用 `io.AddInputCharacter()` 传递文本/字符事件。
- **游戏手柄支持**
  - 使用 `io.AddKeyEvent()` 和 `io.AddKeyAnalogEvent()` 传递游戏手柄事件，使用 `ImGuiKey_GamepadXXX` 值。
- **其他**
  - 剪贴板支持：在 `ImGuiPlatformIO` 中设置 `Platform_GetClipboardTextFn()`、`Platform_SetClipboardTextFn()` 处理程序。
  - 在 Shell 中打开支持：在 `ImGuiPlatformIO` 中设置 `Platform_OpenInShellFn()` 处理程序。
  - IME 支持：在 `ImGuiPlatformIO` 中设置 `Platform_SetImeDataFn()` 处理程序。
  - 使用 `io.AddFocusEvent()` 通知应用程序窗口获得/失去焦点。
- **多视口支持**
  - 如果支持，更新监视器列表。
  - 在 `ImGuiPlatformIO` 中设置所有必需的处理程序以创建/销毁/移动/调整大小/标题/聚焦/等窗口。

### 渲染：实现您的 RenderDrawData 函数

注意：设置 `ImGuiBackendFlags_RendererHasVtxOffset` 表示您的后端可以处理带顶点偏移的渲染（`ImDrawCmd::VtxOffset` 字段）。否则，渲染将限制为每个窗口 64K 个顶点，这对于高级绘图可能会有限制。作为替代方案，您也可以在 `imconfig.h` 文件中使用 `#define ImDrawIdx unsigned int` 来支持 32 位索引。

```cpp
void MyImGuiBackend_RenderDrawData(ImDrawData* draw_data)
{
    // TODO: 更新纹理。
    // - 大多数情况下，列表将有 1 个状态为 OK 的元素，即无事可做。
    // - 这几乎总是指向 ImGui::GetPlatformIO().Textures[]，但是是 ImDrawData 的一部分，以允许覆盖或禁用纹理更新。
    if (draw_data->Textures != nullptr)
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                MyImGuiBackend_UpdateTexture(tex);

    // TODO: 设置渲染状态：
    // - 启用 Alpha 混合
    // - 无背面剔除
    // - 无深度测试，无深度写入
    // - 启用剪刀测试
    MyEngineSetupenderState();

    // TODO: 设置纹理采样状态
    // - 使用双线性过滤进行采样（不是点/最近邻过滤）。
    // - 使用 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines;' 以允许点/最近邻过滤。

    // TODO: 设置覆盖 draw_data->DisplayPos 到 draw_data->DisplayPos + draw_data->DisplaySize 的视口

    // TODO: 设置覆盖 draw_data->DisplayPos 到 draw_data->DisplayPos + draw_data->DisplaySize 的正交投影矩阵

    // TODO: 设置着色器：顶点 { float2 pos, float2 uv, u32 color }，片段着色器从 1 个纹理中采样颜色，乘以顶点颜色。

    // 渲染命令列表
    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;  // Dear ImGui 生成的顶点缓冲区
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;   // Dear ImGui 生成的索引缓冲区
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    MyEngineSetupenderState();
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // 将剪刀/剪裁矩形投影到帧缓冲区空间
                // - 剪裁坐标在 imgui 坐标空间中提供：
                //   - 对于给定的视口，draw_data->DisplayPos == viewport->Pos 和 draw_data->DisplaySize == viewport->Size
                //   - 在单视口应用程序中，draw_data->DisplayPos == (0,0) 和 draw_data->DisplaySize == io.DisplaySize，但始终使用 GetMainViewport()->Pos/Size 而不是硬编码这些值。
                //   - 为了支持多视口应用程序（请参阅 github 上的 'docking' 分支），
                //     始终从剪裁边界中减去 draw_data->DisplayPos 以将其转换为您的视口空间。
                // - 请注意，pcmd->ClipRect 包含 Min+Max 边界。某些图形 API 可能使用 Min+Max，其他可能使用 Min+Size（size 为 Max-Min）
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                // 我们使用剪刀测试来剪裁某些对象。所有底层图形 API 都应该支持它。
                // - 如果您的引擎尚不支持剪刀测试，您可以先忽略它。您会得到一些小故障
                //   （某些元素在其边界之外可见），但您可以在其他一切正常工作后修复它！
                MyEngineSetScissor(clip_min.x, clip_min.y, clip_max.x, clip_max.y);

                // 绘制调用的纹理由 pcmd->GetTexID() 指定。
                // 绝大多数绘制调用将使用 Dear ImGui 纹理图集，其值是您在初始化期间自己设置的。
                MyEngineBindTexture((MyTexture*)pcmd->GetTexID());

                // 渲染 'pcmd->ElemCount/3' 个带索引的三角形。
                // 默认情况下，索引 ImDrawIdx 是 16 位的，如果您的引擎不支持 16 位索引，您可以在 imconfig.h 中将其更改为 32 位。
                MyEngineDrawIndexedTriangles(pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer + pcmd->IdxOffset, vtx_buffer, pcmd->VtxOffset);
            }
        }
    }
}
```

### 渲染：添加对 `ImGuiBackendFlags_RendererHasTextures` 的支持 (1.92+)

版本 [1.92.0](https://github.com/ocornut/imgui/releases/tag/v1.92.0)（2025 年 6 月）在渲染后端中添加了纹理支持，这是支持动态字体缩放等功能的基础。

**为了向前发展并利用所有新功能，很可能在 2026 年 6 月之前，所有后端都将需要支持 `ImGuiBackendFlags_RendererHasTextures`。**

`ImFontAtlas` 函数，如 `Build()`、`GetTexDataAsRGBA32()`、`GetTexDataAsAlpha8()`、`SetTexID()`、`IsBuilt()` 已被废弃，取而代之的是迭代 `Textures[]` 数组并在 Dear ImGui 请求时更新其状态。

**长话短说：在标准后端中添加对 `ImGuiBackendFlags_RendererHasTextures` 支持的提交列表：**

- Allegro5: [ee8941e](https://github.com/ocornut/imgui/commit/ee8941e) (+35 行)
- DirectX9: [75efba7](https://github.com/ocornut/imgui/commit/75efba7) (+48 行)
- DirectX10: [2d2b1bc](https://github.com/ocornut/imgui/commit/2d2b1bc) (+40 行)
- DirectX11: [372fd27](https://github.com/ocornut/imgui/commit/372fd27) (+40 行)
- DirectX12: [eefe5d5](https://github.com/ocornut/imgui/commit/eefe5d5) (+87 行)
- Metal: [26c017d](https://github.com/ocornut/imgui/commit/26c017d) (+55 行)
- OpenGL Legacy: [0430c55](https://github.com/ocornut/imgui/commit/0430c55) (+25 行)
- OpenGL3/WebGL/ES: [dbb91a5](https://github.com/ocornut/imgui/commit/dbb91a5) (+47 行)
- SDL_Renderer2: [9fa65cd](https://github.com/ocornut/imgui/commit/9fa65cd) (+20 行)
- SDL_Renderer3: [e538883](https://github.com/ocornut/imgui/commit/e538883) (+19 行)
- SDL_GPU: [16fe666](https://github.com/ocornut/imgui/commit/16fe666) (+41 行)
- Vulkan: [abe294b](https://github.com/ocornut/imgui/commit/abe294b) (+33 行)
- WGPU: [571dae9](https://github.com/ocornut/imgui/commit/571dae9) (+30 行)

**说明：**

- 设置 `ImGuiBackendFlags_RendererHasTextures` 以表示您的后端可以处理该功能。
- 在渲染期间，例如在您的 RenderDrawData 函数中，迭代 `ImDrawData->Textures` 数组并处理所有纹理。
- 在关闭期间，迭代 `ImGui::GetPlatformIO().Textures` 并销毁所有纹理。
- （两个数组都是 `ImVector<ImTextureData*>`。它们位于不同的位置只是因为：为了允许高级用户执行多线程渲染，我们在 ImDrawData 中存储一个指向纹理列表的指针，目的是让多线程渲染用户用自己的指针替换它。）

处理纹理的伪代码：
```cpp
if (draw_data->Textures != nullptr)
    for (ImTextureData* tex : *draw_data->Textures)
        if (tex->Status != ImTextureStatus_OK)
            MyImGuiBackend_UpdateTexture(tex);
```
```cpp
void MyImGuiBackend_UpdateTexture(ImTextureData* tex)
{
    if (tex->Status == ImTextureStatus_WantCreate)
    {
        // 基于 tex->Width, tex->Height 创建纹理。
        // - 大多数后端仅支持 tex->Format == ImTextureFormat_RGBA32。
        // - 用于内存特别受限的平台的后端可能支持 tex->Format == ImTextureFormat_Alpha8。

        // 上传所有纹理像素
        // - 从我们的 CPU 端纹理副本中读取并复制到您的图形 API。
        // - 根据需要使用 tex->Width, tex->Height, tex->GetPixels(), tex->GetPixelsAt(), tex->GetPitch()。

        // 存储您的数据，并确认创建。
        tex->SetTexID(xxxx); // 指定将存储在 ImDrawCmd 中的后端特定的 ImTextureID 标识符。
        tex->SetStatus(ImTextureStatus_OK);
        tex->BackendUserData = xxxx; // 如果需要，存储更多后端数据（大多数后端分配一个小纹理来存储数据）
    }
    if (tex->Status == ImTextureStatus_WantUpdates)
    {
        // 将像素矩形上传到现有纹理
        // - 我们只写入以前从未使用过的纹理区域！
        // - 使用 tex->TexID 或 tex->BackendUserData 检索您存储的数据。
        // - 使用 tex->UpdateRect.x/y, tex->UpdateRect.w/h 获取块位置和大小。
        //   - 使用 tex->Updates[] 获取 tex->UpdateRect 内的各个子区域。不推荐。
        // - 从我们的 CPU 端纹理副本中读取并复制到您的图形 API。
        // - 根据需要使用 tex->Width, tex->Height, tex->GetPixels(), tex->GetPixelsAt(), tex->GetPitch()。

        // 确认更新
        tex->SetStatus(ImTextureStatus_OK);
    }
    if (tex->Status == ImTextureStatus_WantDestroy && tex->UnusedFrames > 0)
    {
        // 如果您使用分阶段渲染并且有正在进行的渲染，请根据需要将 tex->UnusedFrames > 0 检查更改为更高的计数，例如 > 2

        // 销毁纹理
        // - 使用 tex->TexID 或 tex->BackendUserData 检索您存储的数据。
        // - 在您的图形 API 中销毁纹ry。

        // 确认销毁
        tex->SetTexID(ImTextureID_Invalid);
        tex->SetStatus(ImTextureStatus_Destroyed);
    }
}
```
有关此操作的具体示例，请参阅上面“在标准后端中添加对 `ImGuiBackendFlags_RendererHasTextures` 支持的提交列表”。
