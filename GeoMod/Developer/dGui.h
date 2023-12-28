#ifndef DGUI_H
#define DGUI_H

#include "../../imgui/imgui.h"

enum EditMode {
    SELECT_MODE,
    LINE_MODE,
    CIRCLE_MODE
};

    
// Our state
EditMode editMode = EditMode::SELECT_MODE;

void dGui(ImGuiIO& io) {
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
                ImGui::SameLine();
                 ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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

        ImGui::SetNextWindowPos(ImVec2(0, 150)); // Set position to the top-left corner
        ImGui::SetNextWindowSize(ImVec2(250, viewportSize.y - 150)); // Set width to half of the screen width and height to full screen height

        ImGui::Begin("LeftBarWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        if (ImGui::BeginTabBar("LeftTabBar")) {
            if (ImGui::BeginTabItem("Heirarchy")) {
                               
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
}

#endif // DGUI_H