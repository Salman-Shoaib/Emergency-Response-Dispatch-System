#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct TaskNode {
    string task;
    TaskNode* next;
};

class TaskQueue {
private:
    TaskNode* front;
    TaskNode* rear;
public:
    TaskQueue() { front = rear = NULL; }
    ~TaskQueue() {
        while (front) { TaskNode* t = front; front = front->next; delete t; }
    }

    void Enqueue(string taskDesc) {
        TaskNode* newNode = new TaskNode;
        newNode->task = taskDesc;
        newNode->next = NULL;
        if (rear == NULL) front = rear = newNode;
        else { rear->next = newNode; rear = newNode; }
    }

    string Dequeue() {
        if (front == NULL) return "";
        TaskNode* temp = front;
        string val = temp->task;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
        return val;
    }

    void Display() {
        if (!front) { cout << "No pending maintenance tasks.\n"; return; }
        TaskNode* temp = front;
        cout << "--- Maintenance Tasks ---\n";
        while (temp) {
            cout << "- " << temp->task << endl;
            temp = temp->next;
        }
    }

    void SaveToFile(ofstream& out) {
        TaskNode* cur = front;
        while (cur) {
            out << cur->task << "\n";
            cur = cur->next;
        }
    }
};
