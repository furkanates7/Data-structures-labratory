#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = {
        "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
        "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
        "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
        "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya",
        "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
        "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
        "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
        "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"
};
// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
        : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    map.loadDistanceData(distance_data); // Load map data
    map.resetVisited();                 // Mark all provinces as unvisited
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    numPriorityProvinces = 0;

    while (std::getline(file, line) && numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
        for (int i = 0; i < 81; ++i) {
            if (cities[i] == line) {
                priorityProvinces[numPriorityProvinces++] = i;
                break;
            }
        }
    }
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    numWeatherRestrictedProvinces = 0;

    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
        for (int i = 0; i < 81; ++i) {
            if (cities[i] == line) {
                weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = i;
                break;
            }
        }
    }
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    map.markAsVisited(startingCity);
    stack.push(startingCity);
    route.push_back(startingCity);

    exploreFromProvince(startingCity);
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    while (!stack.isEmpty() || !queue.isEmpty()) {
        enqueueNeighbors(province);

        if (!queue.isEmpty()) {
            int nextProvince = queue.dequeue();
            map.markAsVisited(nextProvince);
            stack.push(nextProvince);
            route.push_back(nextProvince);
            totalDistanceCovered += map.getDistance(province, nextProvince);
            province = nextProvince;
        } else {
            backtrack();
            if (!stack.isEmpty()) {
                province = stack.peek();
            }
        }

        if (isExplorationComplete()) {
            break;
        }
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    for (int neighbor = 0; neighbor < 81; ++neighbor) {
        if (!map.isVisited(neighbor) && !isWeatherRestricted(neighbor) &&
            map.getDistance(province, neighbor) <= maxDistance) {
            if (isPriorityProvince(neighbor)) {
                queue.enqueuePriority(neighbor);
            } else {
                queue.enqueue(neighbor);
            }
        }
    }
}

void RoutePlanner::backtrack() {
    if (!stack.isEmpty()) {
        stack.pop();
    }
}

bool RoutePlanner::isExplorationComplete() const {
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (!map.isVisited(priorityProvinces[i])) {
            return false;
        }
    }
    return true;
}

void RoutePlanner::displayResults() const {
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "Total Provinces Visited: " << route.size() << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route: ";
    for (int province : route) {
        std::cout << cities[province] << " ";
    }
    std::cout << std::endl;

    std::cout << "Priority Province Status:" << std::endl;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        std::cout << cities[priorityProvinces[i]] << ": "
                  << (map.isVisited(priorityProvinces[i]) ? "Visited" : "Not Visited") << std::endl;
    }
}