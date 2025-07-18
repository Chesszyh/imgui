# 贡献指南

## 目录

- [入门与通用建议](#入门与通用建议)
- [问题 (Issues) vs 讨论 (Discussions)](#问题-issues-vs-讨论-discussions)
- [如何开启一个 Issue](#如何开启一个-issue)
- [如何开启一个 Pull Request](#如何开启一个-pull-request)
- [版权 / 贡献者许可协议](#版权--贡献者许可协议)

## 入门与通用建议

- 文章：[如何提出好问题](https://bit.ly/3nwRnx1)。
- 请浏览 [Wiki](https://github.com/ocornut/imgui/wiki) 以查找代码片段、链接和其他资源 (例如 [入门](https://github.com/ocornut/imgui/wiki/Getting-Started), [有用的扩展](https://github.com/ocornut/imgui/wiki/Useful-Extensions))。
- 如果您的问题与设置 Dear ImGui 有关，请阅读[入门](https://github.com/ocornut/imgui/wiki/Getting-Started)。
- 请阅读 [docs/FAQ.md](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md)。
- 如果您的问题与字体或文本有关，请阅读 [docs/FONTS.md](https://github.com/ocornut/imgui/blob/master/docs/FONTS.md)。
- 请运行 `ImGui::ShowDemoWindow()` 来探索演示及其源代码。
- 请使用您 IDE 的搜索功能来搜索与您情况相关的符号和注释。
- 请使用 GitHub 的搜索功能来查找类似的主题（在搜索中始终包括“已关闭”的 issues/pr）。
- 您可以[按标签浏览问题](https://github.com/ocornut/imgui/labels)。
- 请使用网络搜索引擎查找类似的问题。
- 如果您遇到崩溃或断言，请使用调试器定位触发它的行并阅读周围的注释。
- 请不要做一个[帮助吸血鬼](https://slash7.com/2006/12/22/vampires/)。

## '问题 (Issues)' vs '讨论 (Discussions)'

我们很乐意使用“问题”来处理许多类型的开放式问题。我们鼓励“问题”成为一个庞大、集中且交叉引用的 Dear ImGui 内容数据库。

**仅当您：**
- **无法构建或链接示例。**
- **无法在您的应用程序或自定义引擎中构建、链接或运行 Dear ImGui。**
- **无法加载字体。**

**那么请[使用讨论区](https://github.com/ocornut/imgui/discussions)而不是开启一个 issue。**

如果 Dear ImGui 已成功显示在您的应用中，并且您以前使用过 Dear ImGui，则可以开启一个 Issue。任何形式的讨论都欢迎作为新的 issue。

## 如何开启一个 Issue

您可以使用问题跟踪器提交错误报告、功能请求或建议。您也可以寻求帮助或建议。但是**请仔细阅读这篇长文。忽略这些指南的问题可能会被关闭。忽略这些指南的用户可能会被屏蔽。**

请尽力阐明您的请求。由于人们不遵循这些指南而导致的不完整或模棱两可的请求数量常常是压倒性的。没有提供所要求信息而创建的问题可能会被过早关闭。特别理所当然、不礼貌或懒惰的请求可能会导致封禁。

**请理解，开源软件的生死取决于维护者可以投入的精力。我们有很多事情要做。这是一个注意力经济，许多懒惰或次要的问题正在占用我们的注意力并消耗精力，使我们无法从事更重要的工作。**

**步骤：**

- 文章：[如何提出好问题](https://bit.ly/3nwRnx1)。
- **请务必填写所要求的新问题模板。** 包括 Dear ImGui 版本号、分支名称、平台/渲染器后端（`imgui_impl_XXX` 文件）、操作系统。
- **尽量明确您的目标、您的期望以及您尝试过的方法。** 注意 [XY 问题](http://xyproblem.info/)。您脑海中或代码中的内容对其他人来说并不明显。人们经常在没有首先阐明目标的情况下讨论问题并提出不正确的解决方案。在请求新功能时，请描述使用上下文（您打算如何使用它，为什么需要它等）。如果您尝试了某件事但失败了，请向我们展示您尝试过的内容。
- **请包含代码。提供一个最小、完整且可验证的示例 ([MCVE](https://stackoverflow.com/help/mcve)) 来演示您的问题。** 一个理想的提交包含一小段代码，任何人都可以将其粘贴到示例应用程序（`examples/../main.cpp`）或演示（`imgui_demo.cpp`）中以理解和重现它。**将您的问题缩小到其最短、最纯粹的形式是理解、解释和修复它的最简单方法。** 请测试您缩短的代码以确保它能表现出问题。**通常在创建 MCVE 的过程中您就会解决问题！** 许多缺少独立可验证示例的问题在其描述中都缺少问题的实际原因，这最终会浪费每个人的时间。
- **附上屏幕截图（或 GIF/视频）以阐明上下文。** 它们通常传达描述中省略的有用信息。您可以在消息编辑框中拖动图片/文件。避免使用第三方图像托管服务，更喜欢 GitHub 附件的长期持久性（您可以将图片拖到您的帖子中）。在 Windows 上，您可以使用 [ScreenToGif](https://www.screentogif.com/) 轻松捕获 .gif 文件。
- **如果您正在讨论断言或崩溃，请提供调试器调用堆栈。** 切勿在没有附加信息的情况下说“它崩溃了”。如果您不知道如何使用调试器并检索调用堆栈，学习它将很有用。
- **请确保您的项目已启用断言。** `IM_ASSERT()` 的调用分散在代码中，以帮助捕获常见问题。当触发断言时，请阅读其周围的注释。默认情况下，`IM_ASSERT()` 调用标准的 `assert()` 函数。要验证您的断言是否已启用，请在您的 `main()` 函数中添加 `IM_ASSERT(false);` 行。您的应用程序应显示错误消息并中止。如果您的应用程序没有报告错误，则您的断言已禁用。
- 请说明您是否对您的 Dear ImGui 副本或后端进行了重大修改。
- 如果您不是直接从 C++ 调用 Dear ImGui，请提供有关您的语言以及您正在使用的包装器/绑定的信息。
- 请注意，消息将发送到“正在关注”用户的邮箱。在发送消息之前，请尽量校对您的消息。除非他们浏览网站，否则这些用户看不到编辑。

**一些不幸的警告**
- 如果您参与了竞技性在线多人游戏的作弊方案（例如 DLL 注入），请不要在此处发帖。我们不会回答，您将被屏蔽。您的问题是否与所述项目有关并不重要。我们已经有太多这样的人，需要保护我们的时间和理智。
- 由于上述用户频繁滥用此服务，如果您的 GitHub 帐户是匿名的并且是五分钟前创建的，请理解您的帖子将受到更严格的审查，不完整的问题将被严厉驳回。

如果您已经使用 Dear ImGui 一段时间，或者已经使用 C/C++ 多年，或者在这里表现出良好的行为，那么不完全遵守每一项规定是可以的。这些是指导方针，经验丰富的用户或社区成员会知道在给定上下文中哪些信息是有用的。

## 如何开启一个 Pull Request

- **请理解，通过提交 PR，您也同时请求维护者审查您的代码，然后接管其维护。** PR 的制作应既符合最终用户的利益，也便于维护者理解和接受。
- 许多 PR 对于展示需求和可能的解决方案很有用，但不适合合并（导致其他问题，没有看到大局的其他方面等）。如有疑问，请不要犹豫推送 PR，因为这始终是指出问题并找到可合并解决方案的第一步！即使 PR 长时间未合并，它的存在也可能对其他用户有用，并有助于找到通用解决方案。
- **在添加功能时，** 请描述使用上下文（您打算如何使用它，为什么需要它等）。注意 [XY 问题](http://xyproblem.info/)。
- **在修复警告或编译问题时，** 请发布编译器日志并指定您正在使用的编译器版本和平台。
- **附上屏幕截图（或 GIF/视频）以阐明上下文并一目了然地演示该功能。** 您可以在消息编辑框中拖动图片/文件。更喜欢 GitHub 附件的长期持久性而不是第三方托管（您可以将图片拖到您的帖子中）。
- **确保您的代码遵循代码库中已使用的编码风格：** 4 个空格缩进（无制表符）、`local_variable`、`FunctionName()`、`MemberName`、`// Text Comment`、`//CodeComment();`、C 风格的转换等。我们不使用现代 C++ 习语，并且倾向于只使用最少的 C++11 功能。`examples/` 下的应用程序通常不太一致，因为它们有时会尝试模仿某个生态系统通常采用的编码风格（例如，与 DirectX 相关的代码倾向于使用其示例的风格）。
- **确保您为拉取请求创建了一个专用的分支。** 在 Git 中，1 个 PR 与 1 个分支相关联。如果您在提交 PR 后继续推送到同一分支，您的新提交将出现在 PR 中（我们仍然可以挑选单个提交）。

## 版权 / 贡献者许可协议

您提交的任何代码都将成为存储库的一部分，并根据 [Dear ImGui 许可证](https://github.com/ocornut/imgui/blob/master/LICENSE.txt)分发。通过向项目提交代码，您同意该代码是您的作品，并且您可以将其提供给项目。

您还通过提交您的代码同意，您将代码的所有可转让权利授予项目维护者，例如包括重新许可代码、修改代码以及以源代码或二进制形式分发它。具体来说，这包括您将版权转让给项目维护者的要求。因此，在任何 PR 中都不要修改文件中的任何版权声明。
