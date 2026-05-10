#include <iostream>
#include <string>
using namespace std;

struct Vehicle {
    string id;
    string type;
    int locationID;
    string status;
};

struct HashNode {
    Vehicle v;
    HashNode* next;
};

class VehicleHash {
private:
    HashNode** buckets;
    int tableSize;

    unsigned int HashFunc(const string& key) {
        unsigned int hash = 0;
        for (char c : key) hash = (hash * 31) + (unsigned int)c;
        return hash % tableSize;
    }

public:
    VehicleHash() {
        tableSize = 53;
        buckets = new HashNode*[tableSize];
        for (int i = 0; i < tableSize; ++i) buckets[i] = NULL;
    }

    ~VehicleHash() {
        for (int i = 0; i < tableSize; ++i) {
            HashNode* cur = buckets[i];
            while (cur) {
                HashNode* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
        }
        delete[] buckets;
    }

    bool Insert(const string& vid, const string& type, int loc) {
        if (Search(vid) != NULL) {
            cout << "Vehicle with ID " << vid << " already exists.\n";
            return false;
        }
        int idx = HashFunc(vid);
        HashNode* node = new HashNode;
        node->v.id = vid;
        node->v.type = type;
        node->v.locationID = loc;
        node->v.status = "Available";
        node->next = buckets[idx];
        buckets[idx] = node;
        return true;
    }

    Vehicle* Search(const string& vid) {
        int idx = HashFunc(vid);
        HashNode* cur = buckets[idx];
        while (cur) {
            if (cur->v.id == vid) return &cur->v;
            cur = cur->next;
        }
        return NULL;
    }

    bool UpdateStatus(const string& vid, const string& status, int newLoc) {
        Vehicle* v = Search(vid);
        if (!v) return false;
        v->status = status;
        if (newLoc != -1) v->locationID = newLoc;
        return true;
    }

    bool Remove(const string& vid) {
        int idx = HashFunc(vid);
        HashNode* cur = buckets[idx];
        HashNode* prev = NULL;
        while (cur) {
            if (cur->v.id == vid) {
                if (prev) prev->next = cur->next;
                else buckets[idx] = cur->next;
                delete cur;
                return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }

    Vehicle* FindAvailableAt(int locID, const string& type) {
        for (int i = 0; i < tableSize; ++i) {
            HashNode* cur = buckets[i];
            while (cur) {
                if (cur->v.locationID == locID && cur->v.type == type && cur->v.status == "Available")
                    return &cur->v;
                cur = cur->next;
            }
        }
        return NULL;
    }

    int GetTableSize() { return tableSize; }
    HashNode* GetBucket(int idx) { return buckets[idx]; }

    void DisplayAll() {
        cout << "--- Vehicles ---\n";
        for (int i = 0; i < tableSize; ++i) {
            HashNode* cur = buckets[i];
            while (cur) {
                cout << cur->v.id << " | " << cur->v.type << " | loc:" << cur->v.locationID << " | " << cur->v.status << endl;
                cur = cur->next;
            }
        }
    }
};
