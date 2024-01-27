#pragma once
#ifndef QUEUE_H_
#define QUEUE_H_

#include <config/settings.h>

class Queue {
private:
    uint8_t data[MAX_SIZE_QUEUE]; 
    int front; 
    int rear; 

public:
    Queue() : front(-1), rear(-1) {
        
    }

    bool isEmpty() {
        return front == -1 && rear == -1;
    }

    bool isFull() {
        return (rear + 1) % MAX_SIZE_QUEUE == front;
    }
    void push(uint8_t value) {
        if (isFull()) {
            return;
        }

        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear = (rear + 1) % MAX_SIZE_QUEUE;
        }

        data[rear] = value;
    }
    uint8_t pop() {
        if (isEmpty()) {
            return 0;
        }
        uint8_t poppedValue = data[rear];
        if (front == rear) {
            front = rear = -1;
            return poppedValue;
        } 
        rear = (rear - 1 + MAX_SIZE_QUEUE) % MAX_SIZE_QUEUE;
        return poppedValue;
    }
};

#endif