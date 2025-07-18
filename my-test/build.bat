@REM 冒泡排序可视化程序编译脚本
@echo off

@REM 设置输出目录和文件名
@set OUT_DIR=Debug
@set OUT_EXE=BubbleSortVisualization

@REM 设置包含路径
@set INCLUDES=/I.. /I..\backends

@REM 设置源文件
@set SOURCES=main.cpp ..\backends\imgui_impl_dx11.cpp ..\backends\imgui_impl_win32.cpp ..\imgui*.cpp

@REM 设置链接库
@set LIBS=d3d11.lib d3dcompiler.lib

@REM 创建输出目录
if not exist %OUT_DIR% mkdir %OUT_DIR%

@REM 编译
cl /nologo /Zi /MD /utf-8 %INCLUDES% /D UNICODE /D _UNICODE %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%

if %ERRORLEVEL% == 0 (
    echo Success! Exe: %OUT_DIR%\%OUT_EXE%.exe
    echo.
    pause >nul
    %OUT_DIR%\%OUT_EXE%.exe
) else (
    echo Failed! Error code: %ERRORLEVEL%
    pause
)
