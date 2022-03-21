#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <stdio.h> 

namespace AprilFools{
    struct RandomSaber{
        std::pair<int, int> leftXRot;
        std::pair<int, int> leftYRot;
        std::pair<int, int> leftZRot;
        std::pair<int, int> rightXRot;
        std::pair<int, int> rightYRot;
        std::pair<int, int> rightZRot;
        int zPos;
    };

    void Init();
    void generateRandomSaberMovementsForSession();
    void updateValuesOnMiss(int hand);
}