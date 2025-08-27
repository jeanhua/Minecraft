//
// Created by jeanhua on 2025/8/19.
//

#include "user_interface.h"
#include "../global_status.h"

UserInterface::~UserInterface() {
    if (mVAO != 0)
        glDeleteVertexArrays(1, &mVAO);
    if (mVBO != 0)
        glDeleteBuffers(1, &mVBO);
    delete mShader;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

std::pair<float, float> getTextureUV(const uint32_t id) {
    int x_id = static_cast<int>(id % 5);
    int y_id = static_cast<int>(id / 5);
    return std::make_pair(static_cast<float>(x_id) * 0.2f, static_cast<float>(y_id) * 0.2f);
}

UserInterface::UserInterface(Texture2D *worldTexture) {
    if (worldTexture == nullptr)std::cout << "worldTexture is null" << std::endl;
    std::cout << "Start to compile user interface shader program..." << std::endl;
    this->mShader = new Shader("assets/shader/vertex_ui.glsl", "assets/shader/fragment_ui.glsl");
    if (mShader->getShaderProgram() != 0)std::cout << "Compile shader program success." << std::endl;
    this->worldTexture = worldTexture;
    genBuffers();

    // blocks
    global_status::blocks.insert(global_status::blocks.end(), {
                      "grass stone",
                      "leaves",
                      "board 1",
                      "board 2",
                      "brick",
                      "dirt",
                      "end stone",
                      "chest",
                      "stone 1",
                      "stone 2",
                      "log ock",
                      "moss block",
                      "netherrack",
                      "resin brick",
                      "wool_green",
                      "wool_purple",
                      "wool_pink",
                      "wool_purple1",
                      "wool_orange",
                      "wool_red",
                      "wool_yellow",
                  });
}

void UserInterface::render() {
    mShader->begin();
    glBindVertexArray(mVAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 4);
    Shader::end();

    // imgui
    if (global_status::isUIShow) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        buildUIFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void UserInterface::genBuffers() {
    constexpr float size = 0.01f;
    std::vector<float> vertices = {
        -size, 0.0f, 0.0f,
        size, 0.0f, 0.0f,
        0.0f, -size, 0.0f,
        0.0f, size, 0.0f
    };

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

    const uint32_t aPosition = mShader->getAttribPos("aPosition");
    glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(aPosition);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UserInterface::buildUIFrame() {
    ImGui::Begin("Minecraft");
    ImGui::Text("Copyright by jeanhua");
    ImGui::Separator();
    ImGui::Text("Click F1 to show/hide Menu");
    ImGui::SetNextItemWidth(200.0f);
    ImGui::TextWrapped("Click mouse button left to destroy block and the right to put a block\n"
                       "Click WASD to move camera\n"
                       "Click Space to fly high and Shift to fly down.");
    ImGui::Separator();

    if (ImGui::BeginCombo("select block", global_status::blocks[global_status::currentBlock].c_str())) {
        for (int i = 0; i < global_status::blocks.size(); i++) {
            bool is_selected = (global_status::currentBlock == i);
            if (ImGui::Selectable(global_status::blocks[i].c_str(), is_selected)) {
                global_status::currentBlock = i;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("show fog", &global_status::showFog);
    ImGui::Checkbox("show sunshine", &global_status::showSunshine);
    ImGui::ColorEdit3("light color",reinterpret_cast<float *>(&global_status::lightColor));
    ImGui::SliderFloat("ambient strength",&global_status::ambientStrength,0.1f,1.0f);
    ImGui::SliderFloat("specular strength",&global_status::specularStrength,0.1f,1.0f);
    ImGui::SliderInt("shininess",&global_status::shininess,1,50);
    ImGui::Checkbox("show skybox", &global_status::showSkybox);
    ImGui::Separator();
    ImGui::Checkbox("line mode",&global_status::drawLine);
    ImGui::Checkbox("ray test",&global_status::rayTest);

    ImGui::End();
}
