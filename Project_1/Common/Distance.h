#ifndef PROJECT_TASK1_DISTANCE_H
#define PROJECT_TASK1_DISTANCE_H

#include <vector>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>

#include "image.h"
#include "../Algorithms/ExactNN.h"

using namespace std;

int manhattanDistance(vector<unsigned char> * firstImagePixels, vector<unsigned char> * secondImagePixels);

double calcW(vector<Image *> * imgs, double samplePrcnt, int imgNum);

#endif //PROJECT_TASK1_DISTANCE_H
