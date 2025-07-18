# Dear ImGui: 使用字体

_(您可以在 https://github.com/ocornut/imgui/blob/master/docs/FONTS.md 浏览此文件，或使用任何 Markdown 查看器查看此文件)_

`imgui.cpp` 的代码中内嵌了一份 [ProggyClean.ttf](http://proggyfonts.net) (由 Tristan Grimmer 创作)，这是一个默认使用的 13 像素高的像素完美字体。我们将其嵌入源代码中，这样您就可以在没有任何文件系统访问的情况下使用 Dear ImGui。ProggyClean 不能平滑缩放，因此，如果您的应用程序希望看起来美观并支持多种分辨率，建议您加载自己的字体文件。

您也可以加载外部的 .TTF/.OTF 文件。
在 `misc/fonts/` 文件夹中，您可以找到一些推荐的字体，为方便起见提供。

**另请阅读常见问题解答：** https://www.dearimgui.com/faq (其中有字体部分！)

## 目录
- [问题排查](#问题排查)
- [新功能！1.92 版的动态字体系统 (2025年6月)](#新功能192-版的动态字体系统-2025年6月)
- [我应该如何在我的应用程序中处理 DPI？](#我应该如何在我的应用程序中处理-dpi)
- [字体加载说明](#字体加载说明)
- [从内存加载字体数据](#从内存加载字体数据)
- [加载嵌入在源代码中的字体数据](#加载嵌入在源代码中的字体数据)
- [使用图标字体](#使用图标字体)
  - [排除重叠范围](#排除重叠范围)
- [使用 FreeType 光栅化器 (imgui_freetype)](#使用-freetype-光栅化器-imgui_freetype)
- [使用彩色字形/表情符号](#使用彩色字形表情符号)
- [使用自定义字形范围](#使用自定义字形范围)
- [使用自定义彩色图标](#使用自定义彩色图标)
- [关于文件名](#关于文件名)
- [关于 UTF-8 编码](#关于-utf-8-编码)
- [调试工具](#调试工具)
- [仓库中包含的字体的致谢/许可证](#仓库中包含的字体的致谢许可证)
- [字体链接](#字体链接)

---

## 问题排查

**绝大多数遇到的字体和文本相关问题来自以下 4 个方面：**

### (1) 由于使用 `\` 或非预期的工作目录导致的文件名无效。

请参阅[关于文件名](#关于文件名)。如果文件名不正确，`AddFontXXX()` 函数应该会断言。

### (2) 您的非 ASCII 字符串的 UTF-8 编码无效。

请参阅[关于 UTF-8 编码](#关于-utf-8-编码)。使用编码查看器确认源代码中字符串字面量的编码是否正确。

### (3) 缺少字形范围。

🆕 **从 1.92 版开始，使用最新的后端：指定字形范围是不必要的。**

⏪ **1.92 版之前：** 如果要使用非 ASCII 字符，则需要加载具有明确字形范围的字体。请参阅[字体加载说明](#字体加载说明)。使用[调试工具](#调试工具)确认加载的字体和加载的字形范围。

这是 Dear ImGui 以前的一个限制（在 1.92 版中解除）：加载字体时，需要指定要加载哪些字符的字形。
所有加载的字体字形都会提前渲染到一个单一的纹理图集中。调用 `io.Fonts->GetTexDataAsAlpha8()`、`io.Fonts->GetTexDataAsRGBA32()` 或 `io.Fonts->Build()` 中的任何一个都会构建图集。这通常由渲染器后端调用，例如 `ImGui_ImplDX11_NewFrame()` 会调用它。**如果使用自定义字形范围，请确保该数组是持久的**，并在调用 `GetTexDataAsAlpha8()/GetTexDataAsRGBA32()/Build()` 期间可用。

### (4) 字体图集纹理上传到 GPU 失败。

🆕 **从 1.92 版开始，使用最新的后端：图集是增量构建并动态调整大小的，这种情况不太可能发生。**

⏪ 这通常是第 3 点的副产品。如果您有大量的字形或多种字体，纹理可能会变得太大，超出您的图形 API 的承受能力。**上传纹理失败的典型结果是每个字形或所有东西都显示为空白矩形。** 请注意，某些图形驱动程序有纹理大小限制。如果您正在构建 PC 应用程序，请注意您的用户可能使用的硬件限制比您的低。

![空方块](https://github.com/user-attachments/assets/68b50fb5-8b9d-4c38-baec-6ac384f06d26)

一些解决方案：
- 您可以减少过采样，例如 `font_config.OversampleH = 1`，这会将您的纹理大小减半，但会损失一些质量。
  请注意，虽然在大多数情况下 OversampleH = 2 在视觉上非常接近 3，但当 OversampleH = 1 时，质量下降会很明显。在此处阅读有关过采样的信息：[https://github.com/nothings/stb/blob/master/tests/oversample](https://github.com/nothings/stb/blob/master/tests/oversample)。
- 通过从源本地化数据计算字形范围来减少字形范围。
  您可以使用 `ImFontGlyphRangesBuilder` 来实现此目的，并在需要新字符时在帧之间重建图集。这将是最大的胜利！
- 设置 `io.Fonts.Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;` 以禁用将纹理高度向上取整到下一个二次幂。

##### [返回目录](#目录)

---

## 新功能！1.92 版的动态字体系统 (2025年6月)

v1.92 引入了一个更新的动态字体系统。它要求后端支持 `ImGuiBackendFlags_HasTextures` 功能：
- 图标、亚洲和非英语语言的用户不再需要提前预构建所有字形。节省了加载时间、内存，并减少了缺少字形的问题。不再需要指定字形范围。
- 可以随时使用 `PushFont(NULL, new_size)` 来更改字体大小。
- 打包自定义矩形更加方便，因为可以立即写入像素。
- 以前对字体的任何更新都需要后端特定的调用来重新上传纹理，并且这些调用在后端之间不可移植。现在可以以不需要您进行后端特定调用的方式缩放字体等。
- 可以为任何字体源插入自定义加载器/后端。

有关更多详细信息，请参阅 [#8465](https://github.com/ocornut/imgui/issues/8465)。

##### [返回目录](#目录)

---

## 我应该如何在我的应用程序中处理 DPI？

请参阅 [FAQ 条目](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-how-should-i-handle-dpi-in-my-application)。

##### [返回目录](#目录)

---

## 字体加载说明

**加载默认字体：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();
```

**使用以下命令加载 .TTF/.OTF 文件：**

🆕 **从 1.92 版开始，使用最新的后端：传递大小是不必要的。**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf");
```
⏪ **1.92 版之前，或没有最新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
```
如果您收到断言“无法加载字体文件！”，则您的字体文件名很可能不正确。请仔细阅读[关于文件名](#关于文件名)。

**加载多种字体：**
```cpp
// 初始化
ImGuiIO& io = ImGui::GetIO();
ImFont* font1 = io.Fonts->AddFontFromFileTTF("font.ttf");
ImFont* font2 = io.Fonts->AddFontFromFileTTF("anotherfont.otf");
```

在您的应用程序循环中，选择要使用的字体：
```cpp
ImGui::Text("Hello"); // 使用默认字体（即加载的第一个字体）
ImGui::PushFont(font2, 0.0f); // 更改字体，保持当前大小
ImGui::Text("Hello with another font");
ImGui::PopFont();
```

**对于高级选项，创建一个 `ImFontConfig` 结构并将其传递给 `AddFont()` 函数（它将在内部被复制）：**
```cpp
ImFontConfig config;
config.OversampleH = 1.0f;
ImFont* font = io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, &config);
```

**将多种字体合并为一种：**

🆕 **从 1.92 版开始，使用最新的后端：指定字形范围是不必要的。**
```cpp
// 加载第一个字体
ImFont* font = io.Fonts->AddFontDefault();
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 0.0f, &config);           // 合并到第一个字体中以添加例如亚洲字符
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 0.0f, &config); // 合并到第一个字体中以添加图标
io.Fonts->Build();
```
⏪ **1.92 版之前，或没有最新的后端：**
```cpp
// 加载第一个字体
ImFont* font = io.Fonts->AddFontDefault();

// 添加字符范围并合并到上一个字体中
// 范围数组不会被 AddFont* 函数复制，并且是延迟使用的
// 因此请确保在构建或调用 GetTexDataAsRGBA32() 时它可用。
static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // 不会被 AddFont* 复制，因此请保持在作用域内。
ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese()); // 合并到第一个字体中
io.Fonts->AddFontFromFileTTF("fontawesome-webfont.ttf", 18.0f, &config, icons_ranges);             // 合并到第一个字体中
io.Fonts->Build();
```

**添加第四个参数以仅烘焙特定的字体范围：**

🆕 **从 1.92 版开始，使用最新的后端：指定字形范围是不必要的。所有 `GetGlyphRangesXXX()` 函数都已标记为过时。**
```cpp
// 基本拉丁语，扩展拉丁语
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesDefault());

// 默认 + 简体中文使用的 2500 个表意文字的选择
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

// 默认 + 平假名，片假名，半角，1946 个表意文字的选择
io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels, nullptr, io.Fonts->GetGlyphRangesJapanese());
```
请参阅[使用自定义字形范围](#使用自定义字形范围)部分以创建您自己的范围。

**加载和使用日文字体的示例：**

🆕 **从 1.92 版开始，使用最新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf");
```

⏪ **1.92 版之前，或没有最新的后端：**
```cpp
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
```

```cpp
ImGui::Text(u8"こんにちは！テスト %d", 123);
if (ImGui::Button(u8"ロード"))
{
    // 做某事
}
ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
```

![示例代码输出](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/code_sample_02_jp.png)
<br>_(设置：深色样式（左），浅色样式（右）/ 字体：NotoSansCJKjp-Medium, 20px / 圆角：5)_

##### [返回目录](#目录)

---

## 从内存加载字体数据

```cpp
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, ...);
```

**重要提示：** `AddFontFromMemoryTTF()` 默认情况下会将数据缓冲区的所有权转移给字体图集，字体图集在销毁时会尝试释放它。
这是为了避免不必要的复制，并且可能不是一个好的 API（未来的版本将重新设计它）。
如果您想保留数据的所有权并自行释放它，您需要清除 `FontDataOwnedByAtlas` 字段：

```cpp
ImFontConfig font_cfg;
font_cfg.FontDataOwnedByAtlas = false;
ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, data_size, size_pixels, &font_cfg);
```

##### [返回目录](#目录)

---

## 加载嵌入在源代码中的字体数据

- 编译并使用 [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) 来创建一个压缩的 C 样式数组，您可以将其嵌入源代码中。
- 有关如何使用该工具的说明，请参阅 [binary_to_compressed_c.cpp](https://github.com/ocornut/imgui/blob/master/misc/fonts/binary_to_compressed_c.cpp) 中的文档。
- 您可以在演示二进制包中找到适用于 Windows 的预编译版本 `binary_to_compressed_c.exe`（请参阅 [README](https://github.com/ocornut/imgui/blob/master/docs/README.md)）。
- 该工具可以选择输出 Base85 编码以减小*源代码*的大小，但实际二进制文件中的只读数组将大约大 20%。

然后使用以下命令加载字体：
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(compressed_data, compressed_data_size, size_pixels, ...);
```
或
```cpp
ImFont* font = io.Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_data_base85, size_pixels, ...);
```

##### [返回目录](#目录)

---

## 使用图标字体

在您的 Dear ImGui 应用程序中使用图标字体（例如 [FontAwesome](http://fontawesome.io) 或 [OpenFontIcons](https://github.com/traverseda/OpenFontIcons)）是一种简单实用的方法。
一种常见的模式是将图标字体合并到您的主字体中，这样您就可以直接从字符串中嵌入图标，而无需来回切换字体。

要从您的 C++ 代码中引用图标的 UTF-8 码点，您可以使用由 Juliette Foucaut 创建的这些头文件：https://github.com/juliettef/IconFontCppHeaders。

因此，您可以使用 `ICON_FA_SEARCH` 作为将呈现为“搜索”图标的字符串。

🆕 **从 1.92 版开始，使用最新的后端：指定字形范围是不必要的。您可以省略此参数。**
示例设置：
```cpp
// 将图标合并到默认工具字体中
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();
ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 如果您想使图标等宽，请使用此选项
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config);
```
⏪ **1.92 版之前：**
```cpp
// 将图标合并到默认工具字体中
#include "IconsFontAwesome.h"
ImGuiIO& io = ImGui::GetIO();
io.Fonts->AddFontDefault();

ImFontConfig config;
config.MergeMode = true;
config.GlyphMinAdvanceX = 13.0f; // 如果您想使图标等宽，请使用此选项
static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &config, icon_ranges);
```
示例用法：
```cpp
// 用法，例如
ImGui::Text("%s among %d items", ICON_FA_SEARCH, count);
ImGui::Button(ICON_FA_SEARCH " Search");
// C 字符串*字面量*可以在编译时连接，例如 "hello" " world"
// ICON_FA_SEARCH 被定义为字符串字面量，因此这与 "A" "B" 变为 "AB" 相同
```
有关其他图标字体和相关工具，请参阅下面的链接。

**等宽图标？**

为了使您的图标看起来更等宽并便于对齐，您可能需要在加载图标字体时设置 `ImFontConfig::GlyphMinAdvanceX` 值。
如果您设置了 `GlyphMinAdvanceX`，则需要向 `AddFontXXX()` 调用传递一个 `font_size`，因为 MinAdvanceX 值将针对给定大小指定，否则将进行缩放。

**截图**

这是一个使用图标的应用程序（“Avoyd”，https://www.avoyd.com）：
![avoyd](https://user-images.githubusercontent.com/8225057/81696852-c15d9e80-9464-11ea-9cab-2a4d4fc84396.jpg)

##### [返回目录](#目录)

---

### 排除重叠范围

🆕 **从 1.92 版开始，使用最新的后端：字形范围被忽略**：加载字形时，会按顺序查询合并列表中的输入字体。第一个拥有该字形的字体会加载它。
<BR>‼️ **如果您要合并多个字体，可能会出现不希望的重叠范围。** 您可以使用 `ImFontConfig::GlyphExcludeRanges[]` 来指定在给定输入中要忽略的范围。

```cpp
// 添加字体源 1，但忽略 ICON_MIN_FA..ICON_MAX_FA 范围
static ImWchar exclude_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// 添加字体源 2，它期望使用上述范围
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("FontAwesome4.ttf", 0.0f, &cfg2);
```
另一个（愚蠢的）例子：
```cpp
// 从第一个字体中删除 'A'-'Z'
static ImWchar exclude_ranges[] = { 'A', 'Z', 0 };
ImFontConfig cfg1;
cfg1.GlyphExcludeRanges = exclude_ranges;
io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &cfg1);

// 加载另一个字体以填补空白
ImFontConfig cfg2;
cfg2.MergeMode = true;
io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 0.0f, &cfg2);
```
![image](https://github.com/user-attachments/assets/f3d751d3-1aee-4698-bd9b-f511902f56bb)

您可以使用 `Metrics/Debugger->Fonts->Font->Input Glyphs Overlap Detection Tool` 来查看多个字体源中可用的字形列表。这可以帮助您了解哪个字体输入提供了哪个字形。

##### [返回目录](#目录)

---

## 使用 FreeType 光栅化器 (imgui_freetype)

- Dear ImGui 使用 [stb_truetype.h](https://github.com/nothings/stb/) 来光栅化字体（可选过采样）。这种技术及其实现在小尺寸字体渲染时并不理想，可能会显得有些模糊或难以阅读。
- 但是，您可以使用 `misc/freetype/` 文件夹中的 `imgui_freetype.cpp`。使用此文件进行编译，并在您的 `imconfig.h` 文件或构建系统中添加 `#define IMGUI_ENABLE_FREETYPE` 以自动激活它。
- FreeType 支持自动微调，这往往能提高小字体的可读性。尤其是在较小的分辨率下，它会产生很大的不同。
- 请阅读 `misc/freetype/` 文件夹中的文档。
- 正确的 sRGB 空间混合将对您的字体渲染质量产生重要影响。

##### [返回目录](#目录)

---

## 使用彩色字形/表情符号

- `imgui_freetype` 和 FreeType 2.10+ 支持渲染彩色表情符号。
- 您需要使用 `ImGuiFreeTypeBuilderFlags_LoadColor` 标志加载字体。
- 表情符号经常编码在较高的 Unicode 层（字符代码 >0x10000），因此需要使用 `IMGUI_USE_WCHAR32` 编译 dear imgui。
- FreeType 目前并非支持所有类型的彩色字体。
- 文本渲染器不支持有状态的 Unicode 功能，例如肤色修饰符。

![彩色字形](https://user-images.githubusercontent.com/8225057/106171241-9dc4ba80-6191-11eb-8a69-ca1467b206d1.png)

```cpp
io.Fonts->AddFontFromFileTTF("../../../imgui_dev/data/fonts/NotoSans-Regular.ttf", 16.0f);
static ImWchar ranges[] = { 0x1, 0x1FFFF, 0 };
static ImFontConfig cfg;
cfg.MergeMode = true;
cfg.FontLoaderFlags |= ImGuiFreeTypeLoaderFlags_LoadColor;
io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\seguiemj.ttf", 16.0f, &cfg);
```

##### [返回目录](#目录)

---

## 使用自定义字形范围

🆕 **从 1.92 版开始，使用最新的后端：指定字形范围是不必要的。因此，这不再真正有用。**

⏪ 您可以使用 `ImFontGlyphRangesBuilder` 帮助程序根据文本输入创建字形范围。例如：对于一个脚本已知的游戏，如果您可以将整个脚本提供给它，并只构建游戏需要的字符。
```cpp
ImVector<ImWchar> ranges;
ImFontGlyphRangesBuilder builder;
builder.AddText("Hello world");                        // 添加一个字符串（这里“Hello world”包含 7 个唯一字符）
builder.AddChar(0x7262);                               // 添加一个特定字符
builder.AddRanges(io.Fonts->GetGlyphRangesJapanese()); // 添加一个默认范围
builder.BuildRanges(&ranges);                          // 构建最终结果（包含所有提交的唯一字符的有序范围）

io.Fonts->AddFontFromFileTTF("myfontfile.ttf", size_in_pixels, nullptr, ranges.Data);
io.Fonts->Build();                                     // 在 'ranges' 仍在作用域内且未被删除时构建图集。
```

##### [返回目录](#目录)

---

## 使用自定义彩色图标

🆕 **从 1.92 版开始，使用最新的后端：此系统已进行了改进。**

**长话短说：** 对于新系统，建议您创建一个自定义的 `ImFontLoader` 并向其注册您的字体。
`AddCustomRectFontGlyph()` 已被废弃，因为它的 API 对于可调整大小的字体没有多大意义。

您可以在 [#8466](https://github.com/ocornut/imgui/issues/8466) 中提问。

⏪ **1.92 版之前：**

作为使用 `imgui_freetype` 和 `ImGuiFreeTypeBuilderFlags_LoadColor` 渲染彩色字形的替代方案，您可以在纹理图集中分配自己的空间并自行写入。**（这是一个 BETA API，如果您熟悉 dear imgui 和您的渲染后端，请使用）**

- 您可以使用 `ImFontAtlas::AddCustomRect()` 和 `ImFontAtlas::AddCustomRectFontGlyph()` API 来注册将打包到字体图集纹理中的矩形。在构建图集之前注册它们，然后调用 `Build()`。
- 然后，您可以使用 `ImFontAtlas::GetCustomRect(int)` 来查询矩形在纹理中的位置/大小，并将您选择的任何图形数据blit/复制到这些矩形中。
- 此 API 是 beta 版，因为它很可能会更改以支持多 dpi（在具有不同 DPI 比例的多个显示器上的多个视口）。

#### 伪代码：
```cpp
// 添加字体，然后注册两个映射到此字体的字形 'a' 和 'b' 的自定义 13x13 矩形
ImFont* font = io.Fonts->AddFontDefault();
int rect_ids[2];
rect_ids[0] = io.Fonts->AddCustomRectFontGlyph(font, 'a', 13, 13, 13+1);
rect_ids[1] = io.Fonts->AddCustomRectFontGlyph(font, 'b', 13, 13, 13+1);

// 构建图集
io.Fonts->Build();

// 以 RGBA 格式检索纹理
unsigned char* tex_pixels = nullptr;
int tex_width, tex_height;
io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

for (int rect_n = 0; rect_n < IM_ARRAYSIZE(rect_ids); rect_n++)
    if (const ImTextureRect* rect = io.Fonts->GetCustomRect(rect_ids[rect_n]))
    {
        // 用红色像素填充自定义矩形（实际上您会在这里绘制/复制您的位图数据！）
        for (int y = 0; y < rect->Height; y++)
        {
            ImU32* p = (ImU32*)tex_pixels + (rect->Y + y) * tex_width + (rect->X);
            for (int x = rect->Width; x > 0; x--)
                *p++ = IM_COL32(255, 0, 0, 255);
        }
    }
```

##### [返回目录](#目录)

---

## 关于文件名

**请注意，许多新的 C/C++ 用户在加载文件时遇到问题，*因为他们提供的文件名是错误的*，这是由于对当前目录的错误假设造成的。**

需要注意两点：

(1) 在 C/C++ 和大多数编程语言中，如果要在字符串字面量中使用反斜杠 `\`，则需要将其写为双反斜杠 `\\`。碰巧的是，Windows 使用反斜杠作为路径分隔符，所以要小心。
```cpp
io.Fonts->AddFontFromFileTTF("MyFiles\MyImage01.jpg", ...);   // 这是不正确的！！
io.Fonts->AddFontFromFileTTF("MyFiles\\MyImage01.jpg", ...);  // 这是正确的
```
在某些情况下，您也可以在 Windows 下使用 `/` 路径分隔符。

(2) 确保您的 IDE/调试器设置从正确的工作（当前）目录启动您的可执行文件。在 Visual Studio 中，您可以在项目 `属性 > 常规 > 调试 > 工作目录` 中更改您的工作目录。人们假设他们的执行将从项目的根文件夹开始，而默认情况下，它通常从存储对象或可执行文件的文件夹开始。
```cpp
io.Fonts->AddFontFromFileTTF("MyImage01.jpg", ...);       // 相对文件名取决于您运行程序时的工作目录！
io.Fonts->AddFontFromFileTTF("../MyImage01.jpg", ...);    // 从您的工作目录的父文件夹加载
```
##### [返回目录](#目录)

---

## 关于 UTF-8 编码

**对于非 ASCII 字符的显示，一个常见的用户问题是未正确传递 UTF-8 编码的字符串。**

(1) 我们提供了一个函数 `ImGui::DebugTextEncoding(const char* text)`，您可以调用它来验证您的 UTF-8 字符串的内容。
这是确认您的编码是否正确的便捷方法。

```cpp
ImGui::SeparatorText("CORRECT");
ImGui::DebugTextEncoding(u8"こんにちは");

ImGui::SeparatorText("INCORRECT");
ImGui::DebugTextEncoding("こんにちは");
```
![UTF-8 编码查看器](https://github.com/ocornut/imgui/assets/8225057/61c1696a-9a94-46c5-9627-cf91211111f0)

您也可以在 `Metrics/Debuggers->Tools->UTF-8 Encoding viewer` 下找到此工具，如果您想从剪贴板粘贴，但这不会验证您的编译器完成的 UTF-8 编码。

(2) 要以 UTF-8 编码：

还有一些特定于编译器的方法可以默认强制执行 UTF-8 编码：

- Visual Studio 编译器：`/utf-8` 命令行标志。
- Visual Studio 编译器：代码中的 `#pragma execution_character_set("utf-8")`。
- 自 2023 年 5 月以来，我们已将所有示例的 Visual Studio 项目更改为使用 `/utf-8`（[参见提交](https://github.com/ocornut/imgui/commit/513af1efc9080857bbd10000d98f98f2a0c96803)）。

或者，从 C++11 开始，您可以使用 `u8"my text"` 语法将字面量字符串编码为 UTF-8。例如：
```cpp
ImGui::Text(u8"hello");
ImGui::Text(u8"こんにちは");   // 这将始终编码为 UTF-8
ImGui::Text("こんにちは");     // 此编码取决于编译器设置/标志，可能不正确。
```

从 C++20 开始，因为 C++ 委员会讨厌它的用户，他们决定将 `u8""` 语法更改为不返回 `const char*`，而是返回一个新类型 `const char8_t*`，它不能转换为 `const char*`。
由于 `u8""` 在 C++20 中的类型用法有点繁琐：
```cpp
ImGui::Text((const char*)u8"こんにちは");
```
但是，您可以使用 MSVC 的编译器选项 [`/Zc:char8_t-`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-char8-t?view=msvc-170) 和 Clang 和 GCC 的 [`-fno-char8_t`](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r3.html) 完全禁用此行为。
##### [返回目录](#目录)

---

## 调试工具

#### Metrics/Debugger->Fonts
您可以使用 `Metrics/Debugger` 窗口（在 `Demo>Tools` 中可用）来浏览您的字体并了解如果您遇到问题，发生了什么。您也可以在 `Demo->Tools->Style Editor->Fonts` 中找到它。相同的信息在样式编辑器的字体下也可用。

![字体调试](https://user-images.githubusercontent.com/8225057/135429892-0e41ef8d-33c5-4991-bcf6-f997a0bcfd6b.png)

#### UTF-8 编码查看器**
您可以使用 `Metrics/Debugger` 中的 `UTF-8 Encoding viewer` 来验证您的 UTF-8 字符串的内容。在 C/C++ 代码中，您可以调用 `ImGui::DebugTextEncoding("my string");` 函数来验证您的 UTF-8 编码是否正确。

![UTF-8 编码查看器](https://user-images.githubusercontent.com/8225057/166505963-8a0d7899-8ee8-4558-abb2-1ae523dc02f9.png)

##### [返回目录](#目录)

---

## 仓库中包含的字体的致谢/许可证

`misc/fonts/` 文件夹中提供了一些字体文件：

**Roboto-Medium.ttf**，由 Christian Robetson 设计
<br>Apache 许可证 2.0
<br>https://fonts.google.com/specimen/Roboto

**Cousine-Regular.ttf**，由 Steve Matteson 设计
<br>数字化数据版权 (c) 2010 Google Corporation。
<br>根据 SIL Open Font License, Version 1.1 授权
<br>https://fonts.google.com/specimen/Cousine

**DroidSans.ttf**，由 Steve Matteson 设计
<br>Apache 许可证 2.0
<br>https://www.fontsquirrel.com/fonts/droid-sans

**ProggyClean.ttf**，由 Tristan Grimmer 设计
<br>MIT 许可证
<br>（建议加载设置：Size = 13.0, GlyphOffset.y = +1）
<br>http://www.proggyfonts.net/

**ProggyTiny.ttf**，由 Tristan Grimmer 设计
<br>MIT 许可证
<br>（建议加载设置：Size = 10.0, GlyphOffset.y = +1）
<br>http://www.proggyfonts.net/

**Karla-Regular.ttf**，由 Jonathan Pinhorn 设计
<br>SIL OPEN FONT LICENSE Version 1.1

##### [返回目录](#目录)

## 字体链接

#### 图标字体

- 用于图标字体的 C/C++ 头文件（#define 带有码点，可在源代码字符串字面量中使用）https://github.com/juliettef/IconFontCppHeaders
- FontAwesome https://fortawesome.github.io/Font-Awesome
- OpenFontIcons https://github.com/traverseda/OpenFontIcons
- Google Icon Fonts https://design.google.com/icons/
- Kenney Icon Font (游戏控制器图标) https://github.com/nicodinh/kenney-icon-font
- IcoMoon - 自定义图标字体构建器 https://icomoon.io/app

#### 常规字体

- Google Noto Fonts (全球语言) https://www.google.com/get/noto/
- Open Sans Fonts https://fonts.google.com/specimen/Open+Sans
- (日语) M+ 字体，由 Coji Morishita 设计 http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html

#### 等宽字体

像素完美：
- Proggy Fonts，由 Tristan Grimmer 设计 http://www.proggyfonts.net 或 http://upperboundsinteractive.com/fonts.php
- Sweet16, Sweet16 Mono，由 Martin Sedlak 设计 (Latin + Supplemental + Extended A) https://github.com/kmar/Sweet16Font (还包括一个 .inl 文件，可直接在 dear imgui 中使用。)

常规：
- Google Noto Mono Fonts https://www.google.com/get/noto/
- 用于美化源代码的字体 https://github.com/chrissimpkins/codeface
- 编程字体 http://s9w.github.io/font_compare/
- Inconsolata http://www.levien.com/type/myfonts/inconsolata.html
- Adobe Source Code Pro: 用于 UI 和编码环境的等宽字体系列 https://github.com/adobe-fonts/source-code-pro
- 等宽/固定宽度程序员字体 http://www.lowing.org/fonts/

或者使用 Arial Unicode 或 Windows 提供的其他 Unicode 字体以获得完整的字符覆盖（不确定其许可）。

##### [返回目录](#目录)
