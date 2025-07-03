#include "CelestialObject.h"
#include <iostream>
using namespace std;
// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {

    // Initial setup can be done here if needed
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // Additional copying logic can be added here if needed
}
// Print shape


// Rotate right (clockwise)
void CelestialObject::rotate_right() {
    int rows = shape.size();       // Number of rows
    int cols = shape[0].size();    // Number of columns
    vector<vector<bool>> new_shape(cols, vector<bool>(rows));  // Create a new grid for the rotated shape

    // Rotate the grid 90 degrees clockwise
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // The element in position (i, j) in the original grid
            // moves to position (j, rows - 1 - i) in the new rotated grid
            new_shape[j][rows - 1 - i] = shape[i][j];
        }
    }

    shape = new_shape;  // Update the original shape with the rotated shape
}



// Rotate left (counterclockwise)
void CelestialObject::rotate_left() {
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<bool>> new_shape(cols, vector<bool>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            new_shape[cols - 1 - j][i] = shape[i][j];
        }
    }

    shape = new_shape;
}

// Helper function to check if two shapes are the same
bool CelestialObject::is_shape_equal(const vector<vector<bool>> &shape1, const vector<vector<bool>> &shape2) const {
    if (shape1.size() != shape2.size()) return false; // Different number of rows
    for (size_t i = 0; i < shape1.size(); ++i) {
        if (shape1[i] != shape2[i]) return false; // Rows are different
    }
    return true;
}

void CelestialObject::rotate_objects() {
    CelestialObject *current_rotation = this;
    CelestialObject *first_rotation = this; // Start from the original object

    for (int i = 1; i < 4; ++i) {  // Attempt up to 3 additional rotations (90, 180, 270 degrees)
        // Create a temporary rotation for checking
        CelestialObject temp_rotation = *current_rotation;
        temp_rotation.rotate_right();

        // Check if the new rotation matches any of the existing rotations
        CelestialObject *check_rotation = this;
        bool is_unique = true;

        do {
            if (is_shape_equal(temp_rotation.shape, check_rotation->shape)) {
                is_unique = false;
                break;
            }
            check_rotation = check_rotation->right_rotation;
        } while (check_rotation != this && check_rotation != nullptr);

        // Only create and link the new rotation if it's unique
        if (is_unique) {
            CelestialObject *rotation = new CelestialObject(*current_rotation);
            rotation->rotate_right();

            // Copy the next_celestial_object from the original object
            rotation->next_celestial_object = this->next_celestial_object;

            // Link the rotations
            current_rotation->right_rotation = rotation;
            rotation->left_rotation = current_rotation;

            current_rotation = rotation;
        }
    }

    // Link the last rotation back to the first to form a circular linked list, if new rotations were created

    current_rotation->right_rotation = first_rotation;
    first_rotation->left_rotation = current_rotation;
}




// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // Check if the target has rotations
    if (target->right_rotation) {
        // Break the circular links first before deleting
        target->right_rotation->left_rotation = target->left_rotation; // The left rotation of right_rotation should point to the left rotation of target
        target->left_rotation->right_rotation = target->right_rotation; // The right rotation of left_rotation should point to the right rotation of target

        // Now we can safely delete the right rotation
        delete target->right_rotation;
        target->right_rotation = nullptr; // Set to nullptr after deletion to avoid dangling pointer
    }

    if (target->left_rotation) {
        // Break the circular links first before deleting
        target->right_rotation->left_rotation = target->left_rotation; // The left rotation of right_rotation should point to the left rotation of target
        target->left_rotation->right_rotation = target->right_rotation; // The right rotation of left_rotation should point to the right rotation of target

        // Now we can safely delete the left rotation
        delete target->left_rotation;
        target->left_rotation = nullptr; // Set to nullptr after deletion to avoid dangling pointer
    }
}