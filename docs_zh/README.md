# Dear ImGui
<center><b><i>“给一个人状态，他总有一天会遇到bug；但教他如何在两个必须保持同步的地方表示状态，他将一生与bug为伴。”</i></b></center> <a href="https://twitter.com/rygorous/status/1507178315886444544">-ryg</a>

---

[![构建状态](https://github.com/ocornut/imgui/workflows/build/badge.svg)](https://github.com/ocornut/imgui/actions?workflow=build) [![静态分析状态](https://github.com/ocornut/imgui/workflows/static-analysis/badge.svg)](https://github.com/ocornut/imgui/actions?workflow=static-analysis) [![测试状态](https://github.com/ocornut/imgui_test_engine/workflows/tests/badge.svg)](https://github.com/ocornut/imgui_test_engine/actions?workflow=tests)

<sub>（本库在一个免费且宽松的许可下提供，但需要资金支持以维持其持续改进。除了维护和稳定性，还有许多理想的功能有待添加。如果您的公司正在使用 Dear ImGui，请考虑与我们联系。）</sub>

**商业用户：** 通过商业赞助/支持合同支持持续的开发和维护：
<br>&nbsp;&nbsp;*电子邮件: contact @ dearimgui dot com*
<br>**个人用户：** 在[这里](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=WGHNC6MBFLZ2S)支持持续的开发和维护。另请参阅[资金赞助页面](https://github.com/ocornut/imgui/wiki/Funding)。

| [简介](#简介) - [使用](#使用) - [工作原理](#工作原理) - [发布与更新日志](#发布与更新日志) - [演示](#演示) - [入门与集成](#入门与集成) |
:----------------------------------------------------------: |
| [作品展示](#作品展示) - [支持与常见问题](#支持与常见问题-faq) - [如何帮助](#如何帮助) - **[资金与赞助商](https://github.com/ocornut/imgui/wiki/Funding)** - [致谢](#致谢) - [许可证](#许可证) |
| [维基](https://github.com/ocornut/imgui/wiki) - [扩展](https://github.com/ocornut/imgui/wiki/Useful-Extensions) - [语言绑定与框架后端](https://github.com/ocornut/imgui/wiki/Bindings) - [使用Dear ImGui的软件](https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui) - [用户评价](https://github.com/ocornut/imgui/wiki/Quotes) |

### 简介

Dear ImGui 是一个用于 C++ 的**无膨胀的图形用户界面库**。它输出优化的顶点缓冲区，您可以在启用3D管线的应用程序中随时渲染。它快速、可移植、与渲染器无关，并且自包含（无外部依赖）。

Dear ImGui 旨在**实现快速迭代**并**赋能程序员**创建**内容创建工具和可视化/调试工具**（而不是为普通最终用户设计的UI）。为此，它崇尚简单性和生产力，并缺少一些在更高级别库中常见的功能。例如，它不支持完整的国际化（如从右到左文本、双向文本、文本塑形等）和无障碍功能。

Dear ImGui 特别适合集成到游戏引擎（用于工具）、实时3D应用程序、全屏应用程序、嵌入式应用程序，或任何操作系统功能非标准的控制台平台上的应用程序。

- 最小化状态同步。
- 最小化用户端的UI相关状态存储。
- 最小化设置和维护工作。
- 易于创建反映动态数据集的动态UI。
- 易于创建代码驱动和数据驱动的工具。
- 易于创建临时的短期工具和更复杂的长期工具。
- 易于修改和改进。
- 可移植，依赖最小，可在目标设备（控制台、手机等）上运行。
- 高效的运行时和内存消耗。
- 经过实战检验，被[游戏行业的许多主要参与者](https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui)使用。

### 使用

**Dear ImGui 的核心是自包含的，仅由几个与平台无关的文件组成**，您可以轻松地在您的应用程序/引擎中编译它们。这些文件是仓库根目录下的所有文件（`imgui*.cpp`, `imgui*.h`）。**不需要特定的构建过程**。您可以将 `.cpp` 文件添加到您现有的项目中。

在 [backends/](https://github.com/ocornut/imgui/tree/master/backends) 文件夹中提供了**适用于各种图形API和渲染平台的后端**，同时在 [examples/](https://github.com/ocornut/imgui/tree/master/examples) 文件夹中提供了示例应用程序。您也可以创建自己的后端。任何可以渲染带纹理三角形的地方，都可以渲染 Dear ImGui。

有关更多详细信息，请参阅本文档的[入门与集成](#入门与集成)部分。

在您的应用程序中设置好 Dear ImGui 后，您可以在程序循环的_任何地方_使用它：
```cpp
ImGui::Text("Hello, world %d", 123);
if (ImGui::Button("Save"))
    MySaveFunction();
ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
```
![示例代码输出（深色，Segoe UI字体，FreeType）](https://user-images.githubusercontent.com/8225057/191050833-b7ecf528-bfae-4a9f-ac1b-f3d83437a2f4.png)
![示例代码输出（浅色，Segoe UI字体，FreeType）](https://user-images.githubusercontent.com/8225057/191050838-8742efd4-504d-4334-a9a2-e756d15bc2ab.png)

```cpp
// 创建一个名为“My First Tool”的窗口，带有一个菜单栏。
ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
if (ImGui::BeginMenuBar())
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* 执行操作 */ }
        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* 执行操作 */ }
        if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}

// 编辑一个以4个浮点数存储的颜色
ImGui::ColorEdit4("Color", my_color);

// 生成样本并绘制它们
float samples[100];
for (int n = 0; n < 100; n++)
    samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
ImGui::PlotLines("Samples", samples, 100);

// 在一个滚动区域内显示内容
ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
ImGui::BeginChild("Scrolling");
for (int n = 0; n < 50; n++)
    ImGui::Text("%04d: Some text", n);
ImGui::EndChild();
ImGui::End();
```
![my_first_tool_v188](https://user-images.githubusercontent.com/8225057/191055698-690a5651-458f-4856-b5a9-e8cc95c543e2.gif)

Dear ImGui 允许您**创建精细的工具**以及非常临时的工具。在临时性的极端情况下：使用现代编译器的“编辑并继续”（热代码重载）功能，您可以在应用程序运行时添加几个小部件来调整变量，然后在几分钟后删除这些代码！Dear ImGui 不仅仅用于调整数值。您可以用它来通过发出文本命令来跟踪正在运行的算法。您可以将它与您自己的反射数据一起使用，以实时浏览您的数据集。您可以用它来暴露引擎中子系统的内部结构，创建日志记录器、检查工具、性能分析器、调试器，甚至一个完整的游戏制作编辑器/框架等。

### 工作原理

IMGUI 范式通过其 API 试图从用户的角度最小化多余的状态复制、状态同步和状态保留。它比传统的保留模式界面更不容易出错（更少的代码和更少的bug），并且非常适合创建动态用户界面。有关更多详细信息，请查看维基的[关于IMGUI范式](https://github.com/ocornut/imgui/wiki#about-the-imgui-paradigm)部分。

Dear ImGui 输出顶点缓冲区和命令列表，您可以轻松地在您的应用程序中渲染它们。渲染它们所需的绘制调用和状态更改的数量相当少。因为 Dear ImGui 不直接了解或接触图形状态，您可以在代码的任何地方调用它的函数（例如，在正在运行的算法中间，或在您自己的渲染过程中）。有关如何将 Dear ImGui 与您现有代码库集成的说明，请参阅 `examples/` 文件夹中的示例应用程序。

_一个常见的误解是，将立即模式GUI误认为是立即模式渲染，后者通常意味着在调用GUI函数时，用大量低效的绘制调用和状态更改来冲击您的驱动程序/GPU。这**不是** Dear ImGui 的工作方式。Dear ImGui 输出顶点缓冲区和一小批绘制调用列表。它从不直接接触您的GPU。这些绘制调用批次是相当优化的，您可以在稍后，在您的应用程序中甚至远程渲染它们。_

### 发布与更新日志

请参阅[发布页面](https://github.com/ocornut/imgui/releases)以获取带有详细说明的更新日志。
阅读更新日志是了解 Dear ImGui 最新功能的好方法，也许会给您一些您一直忽略的功能的灵感！

### 演示

调用 `ImGui::ShowDemoWindow()` 函数将创建一个演示窗口，展示各种功能和示例。代码始终可在 `imgui_demo.cpp` 中供参考。[这里是演示的样子](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v167/v167-misc.png)。

您应该能够从源代码构建示例。如果不能，请告诉我们！如果您想快速查看一些 Dear ImGui 的功能，可以在这里下载演示应用程序的 Windows 二进制文件：
- [imgui-demo-binaries-20250625.zip](https://www.dearimgui.com/binaries/imgui-demo-binaries-20250625.zip) (Windows, 1.92.0, 构建于 2025/06/25, master 分支) 或[更早的二进制文件](https://www.dearimgui.com/binaries)。

演示应用程序不支持 DPI 感知，因此在 4K 屏幕上可能会有些模糊。要在您的应用程序中实现 DPI 感知，您可以在不同的缩放比例下加载/重新加载字体，并使用 `style.ScaleAllSizes()` 缩放您的样式（请参阅[常见问题](https://www.dearimgui.com/faq)）。

### 入门与集成

请参阅[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started)以获取详细信息。

在大多数平台上，当使用 C++ 时，**您应该能够直接使用 [imgui_impl_xxxx](https://github.com/ocornut/imgui/tree/master/backends) 后端的组合而无需修改**（例如 `imgui_impl_win32.cpp` + `imgui_impl_dx11.cpp`）。如果您的引擎支持多个平台，请考虑使用更多的 `imgui_impl_xxxx` 文件而不是重写它们：这对您来说工作量更少，并且您可以立即让 Dear ImGui 运行起来。如果您愿意，可以*稍后*决定使用您的自定义引擎函数重写一个自定义后端。

将 Dear ImGui 集成到您的自定义引擎中，需要：1) 连接鼠标/键盘/游戏手柄输入；2) 上传一个纹理到您的 GPU/渲染引擎；3) 提供一个可以绑定纹理并渲染带纹理三角形的渲染函数，这基本上就是后端所做的事情。`examples/` 文件夹中充满了正是这样做的应用程序：设置一个窗口并使用后端。如果您遵循[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started)，理论上集成 Dear ImGui 不会超过一个小时。**请务必花时间阅读[常见问题](https://www.dearimgui.com/faq)、注释和示例应用程序！**

官方维护的后端/绑定（在仓库中）：
- **渲染器**: DirectX9, DirectX10, DirectX11, DirectX12, Metal, OpenGL/ES/ES2, SDL_GPU, SDL_Renderer2/3, Vulkan, WebGPU。
- **平台**: GLFW, SDL2/SDL3, Win32, Glut, OSX, Android。
- **框架**: Allegro5, Emscripten。

[第三方后端/绑定维基页面](https://github.com/ocornut/imgui/wiki/Bindings)：
- **语言**: C, C# 以及：Beef, ChaiScript, CovScript, Crystal, D, Go, Haskell, Haxe/hxcpp, Java, JavaScript, Julia, Kotlin, Lobster, Lua, Nim, Odin, Pascal, PureBasic, Python, ReaScript, Ruby, Rust, Swift, Zig...
- **框架**: AGS/Adventure Game Studio, Amethyst, Blender, bsf, Cinder, Cocos2d-x, Defold, Diligent Engine, Ebiten, Flexium, GML/Game Maker Studio, GLEQ, Godot, GTK3, Irrlicht Engine, JUCE, LÖVE+LUA, Mach Engine, Magnum, Marmalade, Monogame, NanoRT, nCine, Nim Game Lib, Nintendo 3DS/Switch/WiiU (自制软件), Ogre, openFrameworks, OSG/OpenSceneGraph, Orx, Photoshop, px_render, Qt/QtDirect3D, raylib, SFML, Sokol, Unity, Unreal Engine 4/5, UWP, vtk, VulkanHpp, VulkanSceneGraph, Win32 GDI, WxWidgets。
- 许多绑定是自动生成的（通过经典的 [cimgui](https://github.com/cimgui/cimgui) 或更新的/实验性的 [dear_bindings](https://github.com/dearimgui/dear_bindings)），您可以使用它们的元数据输出来为其他语言生成绑定。

[有用的扩展/小部件维基页面](https://github.com/ocornut/imgui/wiki/Useful-Extensions)：
- 自动化/测试、文本编辑器、节点编辑器、时间轴编辑器、绘图、软件渲染器、远程网络访问、内存编辑器、Gizmo等。著名且支持良好的扩展包括 [ImPlot](https://github.com/epezent/implot) 和 [Dear ImGui Test Engine](https://github.com/ocornut/imgui_test_engine)。

另请参阅[维基](https://github.com/ocornut/imgui/wiki)以获取更多链接和想法。

### 作品展示

使用 Dear ImGui 的示例项目：[Tracy](https://github.com/wolfpld/tracy) (性能分析器), [ImHex](https://github.com/WerWolv/ImHex) (十六进制编辑器/数据分析), [RemedyBG](https://remedybg.itch.io/remedybg) (调试器) 以及[数百个其他项目](https://github.com/ocornut/imgui/wiki/Software-using-Dear-ImGui)。

更多用户提交的使用 Dear ImGui 的项目截图，请查看[作品展示帖子](https://github.com/ocornut/imgui/issues?q=label%3Agallery)！

有关第三方小部件和扩展的列表，请查看[有用的扩展/小部件维基页面](https://github.com/ocornut/imgui/wiki/Useful-Extensions)。

|  |  |
|--|--|
| 自定义引擎 [erhe](https://github.com/tksuoran/erhe) (docking 分支)<BR>[![erhe](https://user-images.githubusercontent.com/8225057/190203358-6988b846-0686-480e-8663-1311fbd18abd.jpg)](https://user-images.githubusercontent.com/994606/147875067-a848991e-2ad2-4fd3-bf71-4aeb8a547bcf.png) | 用于 [Wonder Boy: The Dragon's Trap](http://www.TheDragonsTrap.com) (2017) 的自定义引擎<BR>[![the dragon's trap](https://user-images.githubusercontent.com/8225057/190203379-57fcb80e-4aec-4fec-959e-17ddd3cd71e5.jpg)](https://cloud.githubusercontent.com/assets/8225057/20628927/33e14cac-b329-11e6-80f6-9524e93b048a.png) |
| 自定义引擎 (无标题)<BR>[![editor white](https://user-images.githubusercontent.com/8225057/190203393-c5ac9f22-b900-4d1e-bfeb-6027c63e3d92.jpg)](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/editor_white.png) | Tracy 性能分析器 ([github](https://github.com/wolfpld/tracy))<BR>[![tracy profiler](https://user-images.githubusercontent.com/8225057/190203401-7b595f6e-607c-44d3-97ea-4c2673244dfb.jpg)](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v176/tracy_profiler.png) |

### 支持与常见问题 (FAQ)

请参阅：[常见问题 (FAQ)](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md)，其中回答了常见问题。

请参阅：[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started)和[维基](https://github.com/ocornut/imgui/wiki)以获取许多链接、参考和文章。

请参阅：[关于IMGUI范式的文章](https://github.com/ocornut/imgui/wiki#about-the-imgui-paradigm)以阅读/学习立即模式GUI范式。

请参阅：[即将到来的变更](https://github.com/ocornut/imgui/wiki/Upcoming-Changes)。

请参阅：[Dear ImGui Test Engine + 测试套件](https://github.com/ocornut/imgui_test_engine)用于自动化和测试。

为了让搜索引擎能够抓取维基，这里有一个[可抓取的维基链接](https://github-wiki-see.page/m/ocornut/imgui/wiki)（不适合人类阅读，[原因在此](https://github-wiki-see.page/)）。

刚开始？对于初次使用的用户，如果遇到编译/链接/运行问题或加载字体问题，请使用[GitHub Discussions](https://github.com/ocornut/imgui/discussions)。对于任何其他问题、错误报告、请求、反馈，请发布在[GitHub Issues](https://github.com/ocornut/imgui/issues)上。请仔细阅读并填写新问题模板。

付费商业客户可获得私人支持（电子邮件：_contact @ dearimgui dot com_）。

**我应该获取哪个版本？**

我们偶尔会标记[发布版本](https://github.com/ocornut/imgui/releases)（带有漂亮的发布说明），但通常同步到最新的 `master` 或 `docking` 分支是安全且推荐的。该库相当稳定，报告的回归问题往往会很快得到修复。高级用户可能希望使用带有[多视口](https://github.com/ocornut/imgui/wiki/Multi-Viewports)和[停靠](https://github.com/ocornut/imgui/wiki/Docking)功能的 `docking` 分支。该分支会定期与 master 保持同步。

**谁在使用 Dear ImGui？**

请参阅[评价](https://github.com/ocornut/imgui/wiki/Quotes)、[资金与赞助商](https://github.com/ocornut/imgui/wiki/Funding)和[使用Dear ImGui的软件](https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui)维基页面，了解谁在使用 Dear ImGui。如果可以，请添加您的游戏/软件！另外，请查看[作品展示帖子](https://github.com/ocornut/imgui/issues?q=label%3Agallery)！

### 如何帮助

**我能如何提供帮助？**

- 查看 [GitHub 论坛/问题](https://github.com/ocornut/imgui/issues)。
- 您可以帮助开发并提交拉取请求！请理解，通过提交PR，您也同时请求维护者审查您的代码，然后永远接管其维护。PR的制作应既符合最终用户的利益，也便于维护者理解和接受。
- 查看[维基](https://github.com/ocornut/imgui/wiki/)上的[寻求帮助](https://github.com/ocornut/imgui/wiki/Help-Wanted)页面以获取更多想法。
- 成为[资金支持者](https://github.com/ocornut/imgui/wiki/Funding)！让您的公司通过商业赞助/维护合同或购买[Dear ImGui Test Engine](https://github.com/ocornut/imgui_test_engine)的许可证来资助该项目（请联系：omar AT dearimgui DOT com）。

### 赞助商

Dear ImGui 的持续开发一直得到用户和私人赞助商的资助。
<BR>有关当前和过去 Dear ImGui 资金支持者和赞助商的**详细列表**，请参阅[资金赞助页面](https://github.com/ocornut/imgui/wiki/Funding)。
<BR>从2014年11月到2019年12月，持续的开发也得到了其在 Patreon 上的用户和个人捐赠的资助。

**感谢所有过去和现在的支持者，帮助这个项目保持活力和繁荣！**

Dear ImGui 使用了为开源项目免费提供的软件和服务：
- [PVS-Studio](https://pvs-studio.com/en/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) 用于静态分析（支持 C/C++/C#/Java）。
- [GitHub actions](https://github.com/features/actions) 用于持续集成系统。
- [OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage) 用于代码覆盖率分析。

### 致谢

由 [Omar Cornut](https://www.miracleworld.net) 以及 GitHub 上所有直接或间接的[贡献者](https://github.com/ocornut/imgui/graphs/contributors)开发。该库的早期版本是在 [Media Molecule](https://www.mediamolecule.com) 的支持下开发的，并首次在游戏 [Tearaway](https://tearaway.mediamolecule.com) (PS Vita) 内部使用。

经常性的贡献者包括 Rokas Kupstys [@rokups](https://github.com/rokups) (2020-2022)：他在自动化系统和回归测试方面的大部分工作现在可在 [Dear ImGui Test Engine](https://github.com/ocornut/imgui_test_engine) 中找到。

维护/支持合同、赞助发票和其他B2B交易由 [Disco Hello](https://www.discohello.com) 托管和处理。

Omar：“我最初是在 [Q-Games](https://www.q-games.com) 发现 IMGUI 范式的，当时 Atman Binstock 在代码库中留下了他自己的简单实现，我花了很多时间来改进和思考它。后来发现，Atman 是直接通过与 Casey 合作而接触到这个概念的。当我搬到 Media Molecule 时，我重写了一个新的库，试图克服我使用过的第一个库的缺陷和局限性。它就成了这个库，从那时起，我花了不合理的时间来迭代和改进它。”

内嵌了 Tristan Grimmer 的 [ProggyClean.ttf](https://www.proggyfonts.net) 字体（MIT 许可证）。
<br>内嵌了 Sean Barrett 的 [stb_textedit.h, stb_truetype.h, stb_rect_pack.h](https://github.com/nothings/stb/)（公共领域）。

早期版本的灵感、反馈和测试来自：Casey Muratori, Atman Binstock, Mikko Mononen, Emmanuel Briney, Stefan Kamoda, Anton Mikhailov, Matt Willis。还要感谢在 GitHub 上发布反馈、问题和补丁的每一个人。

### 许可证

Dear ImGui 在 MIT 许可下授权，更多信息请参阅 [LICENSE.txt](https://github.com/ocornut/imgui/blob/master/LICENSE.txt)。
