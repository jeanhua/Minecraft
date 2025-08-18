//
// Created by jeanhua on 2025/8/18.
//

#include "utils.h"
#include <random>

int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distrib(min, max - 1);
    return distrib(generator);
}
