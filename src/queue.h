#pragma once
#ifndef QUEUE_H_
#define QUEUE_H_

class Queue {
private:
    static const int MAX_SIZE = 64; 
    uint8_t data[MAX_SIZE]; 
    int front; 
    int rear; 

public:
    Queue() : front(-1), rear(-1) {}

    bool isEmpty() {
        return front == -1 && rear == -1;
    }

    bool isFull() {
        return (rear + 1) % MAX_SIZE == front;
    }
    void push(uint8_t value) {
        if (isFull()) {
            return;
        }

        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear = (rear + 1) % MAX_SIZE;
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
        rear = (rear - 1 + MAX_SIZE) % MAX_SIZE;
        return poppedValue;
    }
};

#endif