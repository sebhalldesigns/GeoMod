
#include <cstdio>

#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include <optional>
#include <cstdint>

#include "Include/GeoMod.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HGLRC glRenderContextHandle;
float width;
float height;
int currentMouseScreenPosX;
int currentMouseScreenPosY;
gPoint currentMousePos;

float originX = 0.0f;
float originY = 0.0f;

float scale = 1.0f;

std::vector<gPoint> points;
std::vector<gLine> lines;
std::vector<gCircle> circles;

std::optional<gPoint> workingPoint;

enum {
    NONE,
    PNT,
    LINE,
    CIRCLE,
} editMode;

int main() {

    printf("hello world\n");

    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);
    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        800, 600, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        GetModuleHandle(nullptr),  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24, // bit depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16, // depth buffer
        0, // stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC hdc = GetDC(hwnd);
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    glRenderContextHandle = wglCreateContext(hdc);
    
    wglMakeCurrent(hdc, glRenderContextHandle);
    glViewport(0, 0, 800, 600);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glEnable(GL_TEXTURE_2D);   
    glEnable(GL_POINT_SMOOTH); 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction for text

    ReleaseDC(hwnd, hdc);

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE: {
        width = (float)LOWORD(lParam);
        height = (float)HIWORD(lParam);
        InvalidateRect(hwnd, NULL, NULL);
        return 0;
    }

    case WM_PAINT:
        {

            //printf("WM PAINT %f %f\n", width, height);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            wglMakeCurrent(hdc, glRenderContextHandle);
            glViewport(0, 0, (int)width, (int)height);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // rendering happens here
            
            glMatrixMode(GL_PROJECTION); // Switch to projection matrix mode
            glLoadIdentity(); // Reset the projection matrix
            
            // Set up a perspective projection matrix
            // Parameters: field of view, aspect ratio, near plane, far plane
            //gluPerspective(45.0, width / height, 0.1, 100.0);
            gluOrtho2D(0, width, height, 0); // Set these values to define your 2D plane

            glMatrixMode(GL_MODELVIEW); // Switch back to modelview matrix mode

            float gridScale = 100.0f * scale;

            glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
            // Horizontal lines
            for(float i = originY; i <= height; i += gridScale) {
                glBegin(GL_LINES);
                    glVertex2f(0.0f, i);
                    glVertex2f(width, i);
                glEnd();
            }

            // Vertical lines
            for(float i = originX; i <= width; i += gridScale) {
                glBegin(GL_LINES);
                    glVertex2f(i, 0.0f);
                    glVertex2f(i, height);
                glEnd();
            }
            // end of rendering

            
            glPointSize(7.5f);
            glLineWidth(2.5f);

            
            for (gPoint point : points) {
                glBegin(GL_POINTS);
                glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
                glVertex2d((point.x + originX) * scale, (point.y + originY) * scale);
                glEnd();
            }

            for (gLine line : lines) {
                glBegin(GL_LINES);
                glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
                glVertex2d((line.start.x + originX) * scale, (line.start.y + originY) * scale);
                glVertex2d((line.end.x + originX) * scale, (line.end.y + originY) * scale);
                glEnd();
            }

             for (gLine line : lines) {
                glBegin(GL_POINTS);
                glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
                glVertex2d((line.start.x + originX) * scale, (line.start.y + originY) * scale);
                glVertex2d((line.end.x + originX) * scale, (line.end.y + originY) * scale);
                glEnd();
            }


            if (workingPoint.has_value()) {
                glBegin(GL_POINTS);
                glColor4f(0.25f, 1.0f, 0.25f, 0.5f);
                glVertex2d((workingPoint.value().x + originX) * scale, (workingPoint.value().y + originY) * scale);
                glEnd();

                if (editMode == LINE) {
                    glBegin(GL_LINES);
                    glColor4f(0.25f, 1.0f, 0.25f, 0.5f);
                    glVertex2d((workingPoint.value().x + originX) * scale, (workingPoint.value().y + originY) * scale);
                    glVertex2d((currentMousePos.x + originX) * scale, (currentMousePos.y + originY) * scale);
                    glEnd();
                }
            }
                
            glColor4f(0.25f, 1.0f, 0.25f, 0.5f);
            glBegin(GL_POINTS);
            glVertex2d((currentMousePos.x + originX) * scale, (currentMousePos.y + originY) * scale);
            glEnd();

            for (int i = 0; i < 10; i++) {
                gCircle circle;
                circle.centre = {50.0+(150.0*i), 50.0};
                circle.radial = {50.0+(150.0*i), 100.0};
                glBegin(GL_POINTS);
                glVertex2d((circle.centre.x + originX) * scale, (circle.centre.y + originY) * scale);
                glVertex2d((circle.radial.x + originX) * scale, (circle.radial.y + originY) * scale);
                glEnd();

                glBegin(GL_LINES);
                std::vector<gLine> clines = circle.Approximate(5);
                for (gLine line : clines) {
                    glVertex2d((line.start.x + originX) * scale, (line.start.y + originY) * scale);
                    glVertex2d((line.end.x + originX) * scale, (line.end.y + originY) * scale);
                }
                glEnd();
            } 
            

            SwapBuffers(hdc);
            EndPaint(hwnd, &ps);
        }
        return 0;
     case WM_MOUSEMOVE: {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            int dX = xPos - currentMouseScreenPosX;
            int dY = yPos - currentMouseScreenPosY;

            currentMouseScreenPosX = xPos;
            currentMouseScreenPosY = yPos;

            
            
            gPoint newPoint = {((float) xPos / scale) - originX, ((float) yPos / scale) - originY};
            
            currentMousePos = newPoint;
            

            for (gPoint point : points) {
                if (point.IsNear(newPoint, 10.0)) {
                    currentMousePos = point;
                }
            }

            for (gLine line : lines) {
                if (line.start.IsNear(newPoint, 10.0)) {
                    currentMousePos = line.start;
                }

                if (line.end.IsNear(newPoint, 10.0)) {
                    currentMousePos = line.end;
                }
            }

            

            if (GetAsyncKeyState(VK_MBUTTON)) {
                
                originX += (float) dX;
                originY += (float) dY;
                
            }
            InvalidateRect(hwnd, NULL, NULL);
            //printf("MOUSE POS %f %f\n", (float)currentMouseXPos, (float)currentMouseYPos);
        }
        return 0;
    case WM_MOUSEWHEEL: {
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            
            scale *= (1.0f + (float)zDelta / 1000.0f);
            printf("MOUSE SCROLL %f\n", scale);
            InvalidateRect(hwnd, NULL, NULL);
        }
        return 0;
    case WM_LBUTTONDOWN:
            {
                
                if (editMode == PNT) {
                    points.push_back(currentMousePos);
                } else if (editMode == LINE || editMode == CIRCLE) {
                    if (!workingPoint.has_value()) {
                        workingPoint = currentMousePos;
                    } else {
                        if (editMode == LINE) {
                            lines.push_back({workingPoint.value(), currentMousePos});
                            workingPoint.reset();
                        } else {
                            circles.push_back({workingPoint.value(), currentMousePos});
                            workingPoint.reset();
                        }
                    }
                }
    	
                
            }
            return 0;
    
    case WM_CHAR:{
        uint64_t character = wParam;
        printf("WMCHAR %c %llu\n", character, character);
        switch (character) {
            case 49: {
                editMode = NONE;
                SetWindowText(hwnd, L"NONE");
                return 0;
            }   
            case 50: {
                editMode = PNT;
                SetWindowText(hwnd, L"PNT");
                return 0;
            }
            case 51: {
                editMode = LINE;
                SetWindowText(hwnd, L"LINE");
                return 0;
            }
            case 52: {
                editMode = CIRCLE;
                SetWindowText(hwnd, L"CIRCLE");
                return 0;
            }
        }
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}