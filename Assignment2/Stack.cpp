#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    if (top >= MAX_SIZE - 1) {
        std::cout << "Stack overflow" << std::endl;
    } else {
        data[++top] = province;
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if (isEmpty()) {
        std::cout << "Stack underflow" << std::endl;
        return -1;
    } else {
        return data[top--];
    }
}

// Returns the top province without removing it
int Stack::peek() const {
    if (isEmpty()) {
        std::cout << "Stack is empty" << std::endl;
        return -1;
    } else {
        return data[top];
    }
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    return top == -1;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    return top + 1;
}
