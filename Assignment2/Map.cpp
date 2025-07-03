#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>


Map::Map() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            distanceMatrix[i][j] = std::numeric_limits<int>::max();
        }
        visited[i] = false;
    }
}

void Map::loadDistanceData(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int row = 0;

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    while (std::getline(file, line) && row < MAX_SIZE) {
        std::stringstream ss(line);
        int col = 0;
        std::string value;

        while (std::getline(ss, value, ',') && col < MAX_SIZE) {
            distanceMatrix[row][col] = std::stoi(value);
            ++col;
        }
        ++row;
    }

    file.close();
}

bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    return distanceMatrix[provinceA][provinceB] <= maxDistance;
}

void Map::markAsVisited(int province) {
    visited[province] = true;
}

bool Map::isVisited(int province) const {
    return visited[province];
}

void Map::resetVisited() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

int Map::countVisitedProvinces() const {
    int count = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (visited[i]) {
            ++count;
        }
    }
    return count;
}

int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}
