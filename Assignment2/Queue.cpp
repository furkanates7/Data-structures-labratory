#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cout << "Queue overflow" << std::endl;
    } else {
        if (isEmpty()) {
            front = 0;
        }
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        data[rear] = province;
    }
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if (isEmpty()) {
        std::cout << "Queue underflow" << std::endl;
        return -1;
    } else {
        int province = data[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % MAX_QUEUE_SIZE;
        }
        return province;
    }
}


int Queue::peek() const {
    if (isEmpty()) {
        std::cout << "Error: Queue is empty. Cannot peek." << std::endl;
        return -1;
    } else {
        return data[front];
    }
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return front == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cout << "Queue overflow" << std::endl;
    } else {
        if (isEmpty()) {
            enqueue(province);
        } else {
            front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE; // Circular decrement
            data[front] = province;
        }
    }
}
