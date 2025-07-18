// 冒泡排序可视化程序
// 使用 Dear ImGui 展示排序过程

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <algorithm>
#include <chrono>
#include <d3d11.h>
#include <random>
#include <tchar.h>
#include <thread>
#include <vector>


// DirectX 11 数据
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool g_SwapChainOccluded = false;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// 排序状态
struct SortState {
    std::vector<int> array;
    int current_i = -1; // 当前外层循环位置
    int current_j = -1; // 当前内层循环位置
    int compare_pos1 = -1; // 正在比较的两个位置
    int compare_pos2 = -1;
    bool is_sorting = false;
    bool is_completed = false;
    int step_count = 0;
    int swap_count = 0;
    float sort_speed = 100.0f; // 排序速度 (ms)

    void reset(int size = 20)
    {
        array.clear();
        array.resize(size);

        // 生成随机数组
        std::random_device rd;
        std::mt19937 gen(rd());
        for (int i = 0; i < size; i++) {
            array[i] = i + 1;
        }
        std::shuffle(array.begin(), array.end(), gen);

        current_i = -1;
        current_j = -1;
        compare_pos1 = -1;
        compare_pos2 = -1;
        is_sorting = false;
        is_completed = false;
        step_count = 0;
        swap_count = 0;
    }

    // 执行一步冒泡排序
    bool step()
    {
        if (is_completed)
            return false;

        int n = array.size();

        // 初始化外层循环
        if (current_i == -1) {
            current_i = 0;
            current_j = 0;
        }

        // 检查是否完成
        if (current_i >= n - 1) {
            is_completed = true;
            compare_pos1 = compare_pos2 = -1;
            return false;
        }

        // 执行内层循环的一步
        if (current_j < n - 1 - current_i) {
            compare_pos1 = current_j;
            compare_pos2 = current_j + 1;

            if (array[current_j] > array[current_j + 1]) {
                std::swap(array[current_j], array[current_j + 1]);
                swap_count++;
            }

            current_j++;
            step_count++;
        } else {
            // 内层循环完成，开始下一轮
            current_i++;
            current_j = 0;
            compare_pos1 = compare_pos2 = -1;
        }

        return true;
    }
};

static SortState g_sortState;

// DirectX 11 辅助函数声明
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// UI 渲染函数
void RenderSortVisualization()
{
    ImGui::Begin("冒泡排序可视化", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // 控制面板
    ImGui::Text("控制面板");
    ImGui::Separator();

    static int array_size = 20;
    if (ImGui::SliderInt("数组大小", &array_size, 5, 50)) {
        if (!g_sortState.is_sorting) {
            g_sortState.reset(array_size);
        }
    }

    ImGui::SliderFloat("排序速度 (ms)", &g_sortState.sort_speed, 10.0f, 1000.0f);

    if (ImGui::Button("重置数组")) {
        g_sortState.reset(array_size);
    }
    ImGui::SameLine();

    if (!g_sortState.is_sorting && !g_sortState.is_completed) {
        if (ImGui::Button("开始排序")) {
            g_sortState.is_sorting = true;
        }
    } else if (g_sortState.is_sorting) {
        if (ImGui::Button("暂停")) {
            g_sortState.is_sorting = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("单步执行")) {
            g_sortState.step();
        }
    } else {
        if (ImGui::Button("继续")) {
            g_sortState.is_sorting = true;
        }
    }

    // 统计信息
    ImGui::Separator();
    ImGui::Text("统计信息");
    ImGui::Text("步骤数: %d", g_sortState.step_count);
    ImGui::Text("交换次数: %d", g_sortState.swap_count);
    ImGui::Text("当前状态: %s",
        g_sortState.is_completed ? "已完成" : g_sortState.is_sorting ? "排序中"
                                                                     : "暂停");

    if (g_sortState.current_i >= 0) {
        ImGui::Text("外层循环: %d/%d", g_sortState.current_i + 1, (int)g_sortState.array.size());
    }
    if (g_sortState.current_j >= 0 && !g_sortState.is_completed) {
        ImGui::Text("内层循环: %d/%d", g_sortState.current_j + 1,
            (int)g_sortState.array.size() - g_sortState.current_i);
    }

    ImGui::End();

    // 可视化窗口
    ImGui::Begin("排序可视化", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImVec2(800, 400);
    ImVec2 canvas_max = ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y);

    // 绘制背景
    draw_list->AddRectFilled(canvas_pos, canvas_max, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_pos, canvas_max, IM_COL32(255, 255, 255, 255));

    if (!g_sortState.array.empty()) {
        int n = g_sortState.array.size();
        float bar_width = canvas_size.x / n;
        int max_val = *std::max_element(g_sortState.array.begin(), g_sortState.array.end());

        for (int i = 0; i < n; i++) {
            float bar_height = (float)g_sortState.array[i] / max_val * (canvas_size.y - 20);
            ImVec2 bar_min = ImVec2(canvas_pos.x + i * bar_width + 2,
                canvas_max.y - bar_height - 10);
            ImVec2 bar_max = ImVec2(canvas_pos.x + (i + 1) * bar_width - 2,
                canvas_max.y - 10);

            // 选择颜色
            ImU32 color;
            if (i == g_sortState.compare_pos1 || i == g_sortState.compare_pos2) {
                // 正在比较的元素用红色
                color = IM_COL32(255, 100, 100, 255);
            } else if (g_sortState.current_i >= 0 && i >= n - 1 - g_sortState.current_i) {
                // 已排序的元素用绿色
                color = IM_COL32(100, 255, 100, 255);
            } else {
                // 未排序的元素用蓝色
                color = IM_COL32(100, 150, 255, 255);
            }

            draw_list->AddRectFilled(bar_min, bar_max, color);
            draw_list->AddRect(bar_min, bar_max, IM_COL32(255, 255, 255, 255));

            // 绘制数值
            char text[16];
            sprintf_s(text, "%d", g_sortState.array[i]);
            ImVec2 text_size = ImGui::CalcTextSize(text);
            ImVec2 text_pos = ImVec2(bar_min.x + (bar_width - text_size.x) * 0.5f,
                bar_max.y - text_size.y - 2);
            draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), text);
        }
    }

    // 预留空间
    ImGui::Dummy(canvas_size);

    // 图例
    ImGui::Separator();
    ImGui::Text("图例:");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "■ 正在比较");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "■ 已排序");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "■ 未排序");

    ImGui::End();
}

int main(int, char**)
{
    // 创建应用程序窗口
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"冒泡排序可视化", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"冒泡排序可视化程序", WS_OVERLAPPEDWINDOW, 100, 100, 1200, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // 初始化 Direct3D
    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // 显示窗口
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // 设置 Dear ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // 设置 Dear ImGui 样式
    ImGui::StyleColorsDark();

    // 设置平台/渲染器后端
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // 初始化排序状态
    g_sortState.reset(20);

    // 主循环
    bool done = false;
    auto last_time = std::chrono::high_resolution_clock::now();

    while (!done) {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // 处理窗口最小化
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // 处理窗口大小调整
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // 排序逻辑
        if (g_sortState.is_sorting) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time);

            if (elapsed.count() >= g_sortState.sort_speed) {
                if (!g_sortState.step()) {
                    g_sortState.is_sorting = false;
                }
                last_time = current_time;
            }
        }

        // 开始 Dear ImGui 帧
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 渲染我们的UI
        RenderSortVisualization();

        // 渲染
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    // 清理
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// DirectX 11 辅助函数实现
bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
