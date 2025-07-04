#include "Leaderboard.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


void Leaderboard::read_from_file(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;  // No leaderboard file found, nothing to load
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        unsigned long score;
        time_t timestamp;
        string player_name;

        ss >> score >> timestamp >> player_name;

        LeaderboardEntry* new_entry = new LeaderboardEntry(score, timestamp, player_name);

        // Insert in order (this assumes the list is sorted in descending order by score)
        insert(new_entry);
    }

    file.close();
}

// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening leaderboard file for writing." << endl;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        file << current->score << " " << current->player_name << " " << current->last_played << endl;
        current = current->next;
    }

    file.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        cout << current->player_name << " " << current->score << " "
             << ctime(&current->last_played);  // ctime() converts time_t to string
        current = current->next;
    }
}

// Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
// is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
// top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (head_leaderboard_entry == nullptr || new_entry->score > head_leaderboard_entry->score) {
        // Insert at the beginning if the list is empty or the score is the highest
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next != nullptr && current->next->score >= new_entry->score) {
            current = current->next;
        }
        // Insert the new entry in the correct place
        new_entry->next = current->next;
        current->next = new_entry;
    }

    // Ensure the leaderboard size doesn't exceed MAX_LEADERBOARD_SIZE
    int count = 0;
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    // If the list is too large, remove the last entry
    if (count > MAX_LEADERBOARD_SIZE) {
        current = head_leaderboard_entry;
        for (int i = 1; i < count - 1; i++) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
    }
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry *to_delete = current;
        current = current->next;
        delete to_delete;
    }
}
