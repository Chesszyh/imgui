# Dear ImGui 常见问题解答 (FAQ)

### Q: 我应该从哪里开始？

1.  **花点时间阅读 `docs/README.md` 文件。**
2.  **仔细研究 `imgui_demo.cpp` 中的 `ImGui::ShowDemoWindow()` 函数。**
    这个函数包含了 Dear ImGui 中几乎所有功能的使用示例。你可以随时调用 `ImGui::ShowDemoWindow()` 并浏览其内容。这个演示是学习和发现功能的最佳工具。
3.  **查看 `examples/` 目录下的示例应用程序。**
    它们展示了如何将平台和渲染后端的胶水代码（`imgui_impl_*.cpp` 文件）集成起来。你可以选择一个与你的技术栈最接近的示例作为起点。
4.  **查阅 [Wiki 页面](https://github.com/ocornut/imgui/wiki) 以获取更多链接和资源。**

### Q: 我遇到了编译/链接/启动或字体加载问题。我该怎么办？

- **对于初次使用者的问题，请使用 [GitHub Discussions](https://github.com/ocornut/imgui/discussions)。**
- **对于其他问题、bug报告、功能请求，请使用 [GitHub Issues](https://github.com/ocornut/imgui/issues)。**

1.  **确保你已经阅读了[入门指南](https://github.com/ocornut/imgui/wiki/Getting-Started)。**
2.  **确保你的 `imconfig.h` 文件被正确包含并位于你的构建系统可访问的路径中。**
    `imconfig.h` 是唯一一个 Dear ImGui 核心文件会去包含的、位于其外部的文件。如果你在编译时遇到 `imconfig.h` 相关的错误，很可能是你的包含路径设置不正确。
3.  **确保你所有的 `.cpp` 文件都是用相同的编译器设置和标志来编译的。**
    如果你在一个项目中混合使用不同设置编译的 `.obj` 文件，可能会导致难以诊断的链接或运行时错误。

### Q: 如何集成 Dear ImGui？

- **将 `imgui` 目录下的所有 `.h` 和 `.cpp` 文件复制到你的项目中。**
- **在你项目的构建系统中添加这些 `.cpp` 文件。**
- **提供一个 `imconfig.h` 文件** (你可以从 `misc/` 目录下的示例开始)。
- **选择并添加适合你平台的后端文件** (例如 `backends/imgui_impl_win32.cpp` 和 `backends/imgui_impl_dx11.cpp`)。

**简而言之：**
1.  调用 `ImGui::CreateContext()` 进行初始化。
2.  调用平台后端的 `ImGui_ImplXXX_Init()` 函数。
3.  调用渲染后端的 `ImGui_ImplXXX_Init()` 函数。
4.  在你的主循环中：
    1.  调用 `ImGui_ImplXXX_NewFrame()` (渲染后端)。
    2.  调用 `ImGui_ImplXXX_NewFrame()` (平台后端)。
    3.  调用 `ImGui::NewFrame()`。
    4.  **编写你的 ImGui 界面代码 (例如 `ImGui::Begin()`, `ImGui::Text()`, 等等)。**
    5.  调用 `ImGui::Render()`。
    6.  调用 `ImGui_ImplXXX_RenderDrawData(ImGui::GetDrawData())` (渲染后端)。
5.  调用 `ImGui::DestroyContext()` 和 `ImGui_ImplXXX_Shutdown()` 进行清理。

### Q: 我应该使用哪个版本？

**建议直接使用 `master` 分支的最新代码。**

我们偶尔会标记[发布版本](https://github.com/ocornut/imgui/releases)，但库本身相当稳定，报告的回归问题会很快被修复。高级用户可能希望使用 `docking` 分支来获得多视口和停靠功能。

### Q: 为什么这个库叫 Dear ImGui？

“ImGui” 是 “Immediate Mode GUI”（立即模式图形用户界面）的缩写。Omar Cornut 在开发这个库时，觉得加上 “Dear” 听起来更亲切、更友好。这只是一个个人选择，并没有什么深层含义。

### Q: 如何处理高DPI显示器/视网膜屏？

1.  **加载字体时，将其大小乘以 DPI 缩放因子。**
    例如，如果你的缩放因子是 2.0，你想要 13 像素的字体，那么加载时应该请求 26 像素的字体：`io.Fonts->AddFontFromFileTTF(..., 26.0f)`。
2.  **在加载字体后，相应地缩放所有样式。**
    `ImGui::GetStyle().ScaleAllSizes(dpi_scale_factor);`

你需要在应用程序启动时或 DPI 发生变化时执行这些操作。

### Q: 如何加载图标字体 (例如 FontAwesome)？

1.  **获取字体文件** (例如 `fontawesome-webfont.ttf`)。
2.  **使用 `ImFontConfig` 合并字体。**
    你需要将图标字体合并到你的主字体中。关键是设置正确的合并模式和像素对齐。

```cpp
// 首先加载你的主字体
io.Fonts->AddFontDefault();

// 配置用于合并的字体
ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 使用主字体的某个值
static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

// 加载图标字体并合并
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config, icon_ranges);
```

请参考 `docs/FONTS.md` 和 `misc/fonts/` 目录下的示例代码。

### Q: 如何让我的UI看起来更漂亮？

- **加载一个更好的字体！** 默认的 ProggyClean 字体虽然清晰，但像素感很强。加载一个 TrueType/OpenType 字体 (如 `Roboto` 或 `Segoe UI`) 会极大地改善观感。
- **调整样式颜色。** `ImGui::GetStyle()` 允许你完全自定义每个UI元素的颜色。`ImGui::ShowStyleEditor()` 是一个很好的工具，可以让你实时调整样式并生成代码。
- **从其他项目中寻找灵感。** 查看[作品展示帖子](https://github.com/ocornut/imgui/issues?q=label%3Agallery)和[使用Dear ImGui的软件列表](https://github.com/ocornut/imgui/wiki/Software-using-dear-imgui)。

### Q: 我可以不使用 `Begin()`/`End()` 来创建控件吗？

**可以，但这些控件不会被剪裁或正确地进行鼠标交互。**

所有窗口内的控件都必须在 `ImGui::Begin()` 和 `ImGui::End()` 调用之间。你可以将控件绘制到背景或前景的绘制列表中 (`ImGui::GetBackgroundDrawList()`)，但这通常用于绘制自定义图形，而不是标准的交互控件。

### Q: 如何实现一个控制台/日志窗口？

- **使用 `ImGui::TextUnformatted`** 来避免格式化字符串带来的性能开销。
- **使用 `ImGuiListClipper`** 来只提交可见行的数据进行渲染。这对于有成千上万行日志的窗口至关重要。
- **考虑使用 `ImGui::BeginChild`** 创建一个滚动的子区域。
- **当有新日志时，可以设置滚动条到最底部：** `ImGui::SetScrollHereY(1.0f);`

`imgui_demo.cpp` 中有一个很好的示例 (“Log” 部分)。

### Q: 如何在多个文件或函数中使用 ImGui？

**你不需要做任何特殊操作。**

只要 `ImGui::NewFrame()` 已经被调用，你就可以在代码的任何地方调用 ImGui 函数。ImGui 使用一个全局上下文来存储其状态，所以你不需要传递任何对象指针。只需确保你的 ImGui 调用都在 `NewFrame()` 和 `Render()` 之间。

### Q: ImGui 是线程安全的吗？

**不是。**

你应该只在单个线程中访问 ImGui 上下文。所有 ImGui 调用都应该在同一个线程中进行，通常是你的主线程或渲染线程。如果你需要在其他线程中与UI交互，你需要实现自己的消息队列或同步机制，将数据安全地传递到运行 ImGui 的线程。

### Q: 如何实现自定义控件？

- **阅读 `imgui_internal.h` 中的文档和代码。**
- **参考 `imgui_widgets.cpp` 中现有控件的实现。**
- **使用 `ImGui::GetWindowDrawList()`** 在当前窗口中绘制自定义形状。
- **使用 `ImGui::ItemAdd` 和 `ImGui::ItemSize`** 来告诉 ImGui 你的控件的边界框，以便进行布局和交互测试。
- **使用 `ImGui::ButtonBehavior`** 来处理基本的点击、拖拽等交互逻辑。

创建复杂的自定义控件可能需要对 ImGui 的内部工作原理有更深入的了解。
