#include "AprilFools.hpp"
#include "main.hpp"

namespace AprilFools{
    void Init(){
        srand(time(NULL));
        randomSaber.leftXRot.first = 0;
        randomSaber.leftYRot.first = 0;
        randomSaber.leftZRot.first = 0;
        randomSaber.rightXRot.first = 0;
        randomSaber.rightYRot.first = 0;
        randomSaber.rightZRot.first = 0;
    }
    void generateValueSeedForRotation(std::pair<int, int> *rotPair){
        int randnum = rand() % 2;
        int seed = randnum == 0 ? randnum + 1 : randnum - (randnum * 2);
        rotPair->second = seed;
    }
    void generateRandomSaberMovementsForSession(){
        generateValueSeedForRotation(&randomSaber.leftXRot);
        generateValueSeedForRotation(&randomSaber.leftYRot);
        generateValueSeedForRotation(&randomSaber.leftZRot);
        generateValueSeedForRotation(&randomSaber.rightXRot);
        generateValueSeedForRotation(&randomSaber.rightYRot);
        generateValueSeedForRotation(&randomSaber.rightZRot);
    }
    void updateValuesOnMiss(int hand){
        if (hand == 0){
            randomSaber.leftXRot.first += randomSaber.leftXRot.second;
            randomSaber.leftYRot.first += randomSaber.leftYRot.second;
            randomSaber.leftZRot.first += randomSaber.leftZRot.second;
        }
        else if (hand == 1){
            randomSaber.rightXRot.first += randomSaber.rightXRot.second;
            randomSaber.rightYRot.first += randomSaber.rightYRot.second;
            randomSaber.rightZRot.first += randomSaber.rightZRot.second;
        }
    }
}