#include <fstream>
#include <sstream>
#include "AsteroidDash.h"


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error opening space grid file: " << input_file << std::endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> row;
        int cell;

        // Read each integer in the row and add it to the row vector
        while (ss >> cell) {
            row.push_back(cell);
        }

        // If the row is of the correct size (24), add it to the space grid
        if (row.size() == 24) {
            space_grid.push_back(row);
        } else {
            cerr << "Error: Invalid row size in grid file." << endl;
            file.close();
            return;
        }
    }

    // Check if the grid is of the correct size (10x24)
    if (space_grid.size() != 10) {
        cerr << "Error: Invalid number of rows in grid file." << endl;
    }

    file.close();  // Close the file
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    std::ifstream file(player_file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening player file: " << player_file_name << std::endl;
        return;
    }

    string line;
    int row = 0, col = 0;
    vector<vector<bool>> shape;

    // Read the player's position (row, col)
    if (getline(file, line)) {
        stringstream ss(line);
        ss >> row >> col;
    } else {
        cerr << "Error: Player position not found in file: " << player_file_name << endl;
        file.close();
        return;
    }

    // Read the spacecraft's shape
    while (getline(file, line)) {
        if (line.empty()) continue;  // Skip empty lines

        vector<bool> spacecraft_row;
        for (char ch : line) {
            if (ch == '1') {
                spacecraft_row.push_back(true);
            } else if (ch == '0') {
                spacecraft_row.push_back(false);
            }
        }
        shape.push_back(spacecraft_row);
    }

    if (shape.empty()) {
        cerr << "Error: Spacecraft shape not found in file: " << player_file_name << endl;
        file.close();
        return;
    }

    // Create the Player object using the parsed data
    player = new Player(shape, row, col, player_name);  // Default values for max_ammo and lives

    file.close();  // Close the file
}



// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error opening celestial objects file: " << input_file << std::endl;
        return;
    }

    string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int type_int;  // Önce type bilgisini int olarak okuyalım
        int start_row, time_of_appearance;

        // Göksel nesne türü, başlangıç satırı ve ortaya çıkma zamanını okuyalım
        if (iss >> type_int >> start_row >> time_of_appearance) {
            ObjectType type = static_cast<ObjectType>(type_int);

            vector<vector<bool>> shape;

            // Nesnenin şekil bilgisini dosyadan okuma
            string shape_line;
            while (std::getline(file, shape_line) && !shape_line.empty()) {
                std::vector<bool> row;
                for (char ch : shape_line) {
                    if (ch == '1') {
                        row.push_back(true);
                    } else if (ch == '0') {
                        row.push_back(false);
                    }
                }
                shape.push_back(row);
            }

            // Yeni bir CelestialObject oluştur ve bağlı listeye ekle
            CelestialObject *object = new CelestialObject(shape, type, start_row, time_of_appearance);
            if (!celestial_objects_list_head) {
                celestial_objects_list_head = object;
            } else {
                CelestialObject *current = celestial_objects_list_head;
                while (current->next_celestial_object) {
                    current = current->next_celestial_object;
                }
                current->next_celestial_object = object;
            }
        }
    }

    file.close();
}



// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    for (const auto &row : space_grid) {
        for (char ch : row) {
            std::cout << ch;
        }
        std::cout << std::endl;
    }
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {

}
void AsteroidDash::shoot() {}
    // TODO: Check if player has ammo, create projectile and reduce ammo


// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash(){
    delete player;
    // TODO: Delete dynamically allocated celestial objects
    CelestialObject *current = celestial_objects_list_head;
    while (current) {
        CelestialObject *next = current->next_celestial_object;
        delete current;
        current = next;
    }
    celestial_objects_list_head = nullptr;
}