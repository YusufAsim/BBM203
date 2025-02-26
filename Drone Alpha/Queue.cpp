#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue()
{
    // TODO: Your code here
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province)
{
    // TODO: Your code here
    // Implement circular structure
    // Add the province

    if (front == ((rear + 1) % MAX_QUEUE_SIZE))
    {
        return;
    }
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    if (front == -1)
    {
        front++;
    }
    data[rear] = province;
}

// Removes and returns the front province from the queue
int Queue::dequeue()
{
    // TODO: Your code here
    if (front == -1)
    {
        return -1;
    }
    if (front == rear)
    {
        int temp = data[front];
        front = rear = -1;
        return temp;
    }
    int temp = data[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    return temp;
}

// Returns the front province without removing it
int Queue::peek() const
{
    // TODO: Your code here
    if (front == -1 && rear == -1)
    {
        return -1;
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const
{
    // TODO: Your code here
    if (front == -1 && rear == -1)
    {
        return true;
    }
    return false;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province)
{
    // TODO: Your code here
    if (front == ((rear + 1) % MAX_QUEUE_SIZE))
    {
        return;
    }
    if (front == -1)
    {
        front = rear = 0;
    }
    else
    {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        for (int i = rear; i > front; i--)
        {
            data[i] = data[i - 1];
        }
        data[front] = province;
    }
}
