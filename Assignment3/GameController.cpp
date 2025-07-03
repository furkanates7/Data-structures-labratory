#include "GameController.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name
) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name, player_name);
    // TODO: Any extra initializations (currently none required)
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    std::ifstream file(commands_file);
    if (!file.is_open()) {
        std::cerr << "Error opening commands file: " << commands_file << std::endl;
        return;
    }

    string command;
    while (std::getline(file, command)) {
        if (command == "PRINT") {
            game->print_space_grid();
        } else if (command == "UPDATE") {
            game->update_space_grid();
        } else if (command == "SHOOT") {
            game->shoot();
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }

    file.close();
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;
    game = nullptr;
}
