#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct StackNode {
    string command;
    StackNode* next;
};

class CommandStack {
private:
    StackNode* top;
public:
    CommandStack() { top = NULL; }
    ~CommandStack() {
        while (top) { StackNode* t = top; top = top->next; delete t; }
    }

    void Push(string cmd) {
        StackNode* newNode = new StackNode;
        newNode->command = cmd;
        newNode->next = top;
        top = newNode;
    }

    void Pop() {
        if (top == NULL) return;
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }

    void ViewHistory() {
        if (!top) { cout << "No command history.\n"; return; }
        StackNode* temp = top;
        cout << "--- Command History (Last Actions) ---\n";
        while (temp) {
            cout << ">> " << temp->command << endl;
            temp = temp->next;
        }
    }

    void SaveToFile(ofstream& out) {
        StackNode* cur = top;
        while (cur) {
            out << cur->command << "\n";
            cur = cur->next;
        }
    }
};
