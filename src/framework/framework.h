//
// Created by jeanhua on 2025/8/16.
//

#ifndef MINECRAFT_FRAMEWORK_H
#define MINECRAFT_FRAMEWORK_H

#include "../core.h"
#include <vector>

using Vertex = struct {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
};

using VertexSimple = struct {
    float x, y, z;
    float u, v;
};

#endif //MINECRAFT_FRAMEWORK_H