//
// Created by jeanhua on 2025/8/23.
//

#ifndef MINECRAFT_GLOBAL_STATUS_H
#define MINECRAFT_GLOBAL_STATUS_H

#include "../core.h"

class global_status {
public:
    inline static int mapSeed = 0;
    inline static int treeSeed = 0;

    inline static bool isUIShow = true;

    inline static bool showFog = true;
    inline static bool showSunshine = true;
    inline static ImVec4 lightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    inline static float ambientStrength = 0.8;
    inline static float specularStrength = 0.9;
    inline static int shininess = 32;
    inline static bool showSkybox = true;
    inline static std::vector<std::string>blocks;
    inline static int currentBlock = 0;

    inline static bool drawLine = false;
    inline static bool rayTest = false;

    inline static int renderRadius = 20;
};
#endif //MINECRAFT_GLOBAL_STATUS_H