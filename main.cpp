// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#define NO_SDL_GLEXT
#include "../GLEW/GL/glew.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include <cstdio>
#include "../GeoMod/Utility/Files/gFileReader.h"


enum EditMode {
    SELECT_MODE,
    LINE_MODE,
    CIRCLE_MODE
};

// Main code
int main(int, char**) {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("GeoMod Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 750, window_flags);

    if (window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    glewExperimental = GL_TRUE;
    int init_res = glewInit();
    if(init_res != GLEW_OK) {
        printf("failed to init glew: %s\n", glewGetErrorString(glewInit()));
    }

    SDL_GL_SetSwapInterval(1); // Enable vsync

    printf("Got to here!\n");
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    
     
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

    
    // Our state
    EditMode editMode = EditMode::SELECT_MODE;


    // Main loop
    bool done = false;

    while (!done) {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();


        ImGui::SetNextWindowPos(ImVec2(0, 0)); // Set position to the top-left corner
        ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
        ImGui::SetNextWindowSize(ImVec2(viewportSize.x, 150)); // Set width to half of the screen width and height to full screen height

        // Create the window
        ImGui::Begin("TabBarWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("Sketch")) {
                
                ImGui::BeginChild("EditMode", ImVec2(150.0f, 110.0f), ImGuiChildFlags_Border | ImGuiChildFlags_FrameStyle);
                ImGui::Text("Edit Mode");
                if (editMode == EditMode::SELECT_MODE)
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Active color
                if (ImGui::Button("Select")) {
                    if (editMode == EditMode::SELECT_MODE)
                        ImGui::PopStyleColor(); // Active color
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Active color
                    editMode = EditMode::SELECT_MODE;
                }
                if (editMode == EditMode::SELECT_MODE)
                    ImGui::PopStyleColor(); // Active color

                if (editMode == EditMode::LINE_MODE)
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Active color
                if (ImGui::Button("Line")) {
                    if (editMode == EditMode::LINE_MODE)
                        ImGui::PopStyleColor(); // Active color
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Active color
                    editMode = EditMode::LINE_MODE;
                }
                if (editMode == EditMode::LINE_MODE)
                    ImGui::PopStyleColor(); // Active color


                if (editMode == EditMode::CIRCLE_MODE)
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Active color
                if (ImGui::Button("Circle")) {
                    if (editMode == EditMode::CIRCLE_MODE)
                        ImGui::PopStyleColor(); // Active color
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Active color
                    editMode = EditMode::CIRCLE_MODE;
                }
                if (editMode == EditMode::CIRCLE_MODE)
                    ImGui::PopStyleColor(); // Active color
                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Model")) {
                // Content for Tab 2
                ImGui::Text("This is Tab 2");
                ImGui::EndTabItem();
            }
            // Add more tabs as needed
            ImGui::EndTabBar();
        }


        ImGui::End();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y - 150);

        float majorGridSize = 100.0f;
        float minorGridSize = 10.0f;
        float majorGridX = majorGridSize/(io.DisplaySize.x/2.0f);
        float majorGridY = majorGridSize/(io.DisplaySize.y/2.0f);
        int numberOfMajorX = (int)(io.DisplaySize.x/majorGridSize) + 1;
        int numberOfMajorY = (int)(io.DisplaySize.y/majorGridSize) + 1;

        float minorGridX = minorGridSize/(io.DisplaySize.x/2.0f);
        float minorGridY = minorGridSize/(io.DisplaySize.y/2.0f);
        int numberOfMinorX = (int)(io.DisplaySize.x/minorGridSize) + 1;
        int numberOfMinorY = (int)(io.DisplaySize.y/minorGridSize) + 1;

        // Ensure a VAO is bound
        glBindVertexArray(horizontalLineVao);


        glUseProgram(gridProgram);
        // Set uniforms.
        glUniform1f(glGetUniformLocation(gridProgram, "offset"), 0.0f);
        glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 1);
        glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), minorGridY);
        glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.05f, 0.05f, 0.05f, 1.0f);
        // Draw grid.
        glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMinorY);

        // Set uniforms.
        glUniform1f(glGetUniformLocation(gridProgram, "offset"), 0.0f);
        glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 0);
        glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), minorGridX);
        glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.05f, 0.05f, 0.05f, 1.0f);
        // Draw grid.
        glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMinorX);

        // Set uniforms.
        glUniform1f(glGetUniformLocation(gridProgram, "offset"), 0.0f);
        glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 1);
        glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), majorGridY);
        glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.1f, 0.1f, 0.1f, 1.0f);
        // Draw grid.
        glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMajorY);

        // Set uniforms.
        glUniform1f(glGetUniformLocation(gridProgram, "offset"), 0.0f);
        glUniform1i(glGetUniformLocation(gridProgram, "horizontal"), 0);
        glUniform1f(glGetUniformLocation(gridProgram, "gridSize"), majorGridX);
        glUniform4f(glGetUniformLocation(gridProgram, "gridColor"), 0.1f, 0.1f, 0.1f, 1.0f);
        // Draw grid.
        glDrawArraysInstanced(GL_LINES, 0, 2, numberOfMajorX);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}