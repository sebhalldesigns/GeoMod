// Dear ImGui: standalone example application for Win32 + OpenGL 3

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// This is provided for completeness, however it is strongly recommended you use OpenGL with SDL or GLFW.
#include "../GLEW/GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h>
#include <GL/GL.h>
#include <tchar.h>
#include "../GeoMod/Utility/Files/gFileReader.h"
#include "GeoMod/GeoMod.h"
#include "GeoMod/Developer/dGui.h"

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int currentMouseScreenPosX;
int currentMouseScreenPosY;
gPoint currentMousePos;

float originX = 0.0f;
float originY = 0.0f;

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui Win32+OpenGL3 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    glewExperimental = GL_TRUE;
    int init_res = glewInit();
    if(init_res != GLEW_OK) {
        printf("failed to init glew: %s\n", glewGetErrorString(glewInit()));
    }

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
      
     
    std::string gridVsSrc = gFileReader::ReadFile("../../gridVs.glsl");
    std::string gridGsSrc = gFileReader::ReadFile("../../gridGs.glsl");
    std::string gridFsSrc = gFileReader::ReadFile("../../gridFs.glsl");

    const char* gridVsRaw = gridVsSrc.c_str();
    const char* gridGsRaw = gridGsSrc.c_str();
    const char* gridFsRaw = gridFsSrc.c_str();

    unsigned int gridVs, gridGs, gridFs, gridProgram;

    gridVs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gridVs, 1, &gridVsRaw, NULL);
    glCompileShader(gridVs);
    // Check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(gridVs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(gridVs, 512, NULL, infoLog);
        printf("error: shader compilation failed: %s\n", &infoLog);
    }

    gridGs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gridGs, 1, &gridGsRaw, NULL);
    glCompileShader(gridGs);

    // Check for shader compile errors
    glGetShaderiv(gridGs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(gridGs, 512, NULL, infoLog);
        printf("error: shader compilation failed: %s\n", &infoLog);
    }

    gridFs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gridFs, 1, &gridFsRaw, NULL);
    glCompileShader(gridFs);
    
    // Check for shader compile errors
    glGetShaderiv(gridFs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(gridFs, 512, NULL, infoLog);
        printf("error: shader compilation failed: %s\n", &infoLog);
    }

    gridProgram = glCreateProgram();

    glAttachShader(gridProgram, gridVs);
    //glAttachShader(gridProgram, gridGs);
    glAttachShader(gridProgram, gridFs);

    // Link the program
    glLinkProgram(gridProgram);

    // Delete the shader objects once linked
    glDeleteShader(gridVs);
    //glDeleteShader(gridGs);
    glDeleteShader(gridFs);

    // Check for linking errors
    glGetProgramiv(gridProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(gridProgram, 512, NULL, infoLog);
        printf("error: shader linking failed: %s\n", &infoLog);
    }

    GLuint horizontalLineVao, horizontalLineVbo;
    glGenVertexArrays(1, &horizontalLineVao);
    glGenBuffers(1, &horizontalLineVbo);

    float horizontalLineVertices[] = { -1.0f, -1.0f, 1.0f, -1.0f }; // Two endpoints of the line (x1, y1, x2, y2)

    glBindVertexArray(horizontalLineVao);
    glBindBuffer(GL_ARRAY_BUFFER, horizontalLineVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(horizontalLineVertices), horizontalLineVertices, GL_STATIC_DRAW);

    // Vertex attribute for the line endpoints
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;


         // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();

        dGui(io);
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glViewport(250, 0, (int)io.DisplaySize.x - 250, (int)io.DisplaySize.y - 150);

        float viewportWidth = io.DisplaySize.x - 250;
        float viewportHeight = io.DisplaySize.y - 150;

        /*{

            float majorGridSize = 100.0f;
            float minorGridSize = 10.0f;
            float majorGridX = majorGridSize/(viewportWidth/2.0f);
            float majorGridY = majorGridSize/(viewportHeight/2.0f);
            int numberOfMajorX = (int)(viewportWidth/majorGridSize) + 1;
            int numberOfMajorY = (int)(viewportHeight/majorGridSize) + 1;

            float minorGridX = minorGridSize/(viewportWidth/2.0f);
            float minorGridY = minorGridSize/(viewportHeight/2.0f);
            int numberOfMinorX = (int)(viewportWidth/minorGridSize) + 1;
            int numberOfMinorY = (int)(viewportHeight/minorGridSize) + 1;

            float majorOffsetX = fmod(originX/(viewportWidth/2.0f), majorGridX);
            float majorOffsetY = -1*fmod(originY/(viewportHeight/2.0f), majorGridY);
            float minorOffsetX = fmod(originX/(viewportWidth/2.0f), minorGridX);
            float minorOffsetY = -1*fmod(originY/(viewportHeight/2.0f), minorGridY);

            // Ensure a VAO is bound
            glBindVertexArray(horizontalLineVao);


            glUseProgram(gridProgram);
            // Set uniforms.
            glUniform1f(glGetUniformLocation(gridProgram, "offset"), minorOffsetY);
            glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 1);
            glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), minorGridY);
            glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.05f, 0.05f, 0.05f, 1.0f);
            // Draw grid.
            glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMinorY);

            // Set uniforms.
            glUniform1f(glGetUniformLocation(gridProgram, "offset"),minorOffsetX);
            glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 0);
            glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), minorGridX);
            glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.05f, 0.05f, 0.05f, 1.0f);
            // Draw grid.
            glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMinorX);

            // Set uniforms.
            glUniform1f(glGetUniformLocation(gridProgram, "offset"), majorOffsetY);
            glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 1);
            glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), majorGridY);
            glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.1f, 0.1f, 0.1f, 1.0f);
            // Draw grid.
            glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMajorY);

            // Set uniforms.
            glUniform1f(glGetUniformLocation(gridProgram, "offset"), majorOffsetX);
            glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 0);
            glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), majorGridX);
            glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.1f, 0.1f, 0.1f, 1.0f);
            // Draw grid.
            glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMajorX);

            glBindVertexArray(0);
        }*/
        
            


        // Present
        ::SwapBuffers(g_MainWindow.hDC);

   
       
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, true);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_MOUSEWHEEL:
        printf("MOUSEWHEEL %d @ %d %d\n", GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
        //originY -= (float) GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;
    case WM_MOUSEHWHEEL:
        printf("MOUSE H WHEEL %d @ %d %d\n", GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
        //originX -= (float) GET_WHEEL_DELTA_WPARAM(wParam);
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
        case WM_MOUSEMOVE: {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            int dX = xPos - currentMouseScreenPosX;
            int dY = yPos - currentMouseScreenPosY;

            currentMouseScreenPosX = xPos;
            currentMouseScreenPosY = yPos;

            if (GetAsyncKeyState(VK_MBUTTON)) {
                
                originX += (float) dX;
                originY += (float) dY;
                
            }
            //printf("MOUSE POS %f %f \t ORIGIN %f %f \n", (float)currentMouseScreenPosX, (float)currentMouseScreenPosY, originX, originY);
            
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}