# IMGUI

## `imgui.cpp`

- Dear IMGUI的文档并不完善，FAQ里第一个QA就是: `This library is poorly documented at the moment and expects the user to be acquainted with C/C++.`
- 具体示例可参考`imgui_demo.cpp`(作者还强烈建议不要删除该文件)
- UI与代码绑定：每帧重绘图像，代码终止时UI也会终止
- 空闲帧(IDLE)减少内存分配/释放，避免内存碎片
- 更偏向C风格，较少使用成员函数，`ImGui::`只作为命名空间
- 可通过`#define IMGUI_USER_CONFIG "my_config_file.h"`自定义配置文件(compile time)，而不是直接覆盖`imgui_config.h`