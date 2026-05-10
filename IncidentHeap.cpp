#include <iostream>
#include <string>
using namespace std;

struct Incident {
    int id;
    int locationID;
    string type;
    int urgency;
    string status;
};

template <typename T>
void mySwap(T& a, T& b) {
    T temp = a; a = b; b = temp;
}

class IncidentHeap {
private:
    Incident* arr;
    int capacity;
    int size;

    void Resize() {
        capacity *= 2;
        Incident* newArr = new Incident[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }

    void HeapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (arr[index].urgency < arr[parent].urgency) {
                mySwap(arr[index], arr[parent]);
                index = parent;
            } else break;
        }
    }

    void HeapifyDown(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < size && arr[left].urgency < arr[smallest].urgency) smallest = left;
        if (right < size && arr[right].urgency < arr[smallest].urgency) smallest = right;
        if (smallest != index) {
            mySwap(arr[index], arr[smallest]);
            HeapifyDown(smallest);
        }
    }

public:
    IncidentHeap() {
        capacity = 32;
        size = 0;
        arr = new Incident[capacity];
    }
    ~IncidentHeap() { delete[] arr; }

    bool IsEmpty() { return size == 0; }

    bool IDExists(int id) {
        for (int i = 0; i < size; ++i) if (arr[i].id == id) return true;
        return false;
    }

    void Insert(int id, int loc, string type, int urg) {
        if (IDExists(id)) {
            cout << "Error: Incident ID already exists in pending incidents.\n";
            return;
        }
        if (size == capacity) Resize();
        arr[size].id = id;
        arr[size].locationID = loc;
        arr[size].type = type;
        arr[size].urgency = urg;
        arr[size].status = "Pending";
        size++;
        HeapifyUp(size - 1);
    }

    Incident ExtractMin() {
        if (size == 0) {
            Incident empty = { -1, -1, "", -1, "" };
            return empty;
        }
        Incident root = arr[0];
        arr[0] = arr[size-1];
        size--;
        if (size > 0) HeapifyDown(0);
        return root;
    }

    int GetSize() { return size; }
    Incident GetAt(int i) { return arr[i]; }

    bool RemoveByID(int id) {
        int idx = -1;
        for (int i = 0; i < size; ++i) if (arr[i].id == id) { idx = i; break; }
        if (idx == -1) return false;
        arr[idx] = arr[size-1];
        size--;
        if (idx < size) {
            HeapifyUp(idx);
            HeapifyDown(idx);
        }
        return true;
    }

    void DisplayAll() {
        if (IsEmpty()) { cout << "No pending incidents.\n"; return; }
        cout << "--- Pending Incidents (heap-order) ---\n";
        for (int i = 0; i < size; ++i) {
            cout << "ID:" << arr[i].id << " Loc:" << arr[i].locationID << " Type:" << arr[i].type
                << " Urg:" << arr[i].urgency << " Status:" << arr[i].status << endl;
        }
    }
};
