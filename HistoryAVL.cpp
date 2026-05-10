#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct HistoricalRecord {
    int incidentID;
    int urgency;
    string type;
    string resolveTime;
    HistoricalRecord* left;
    HistoricalRecord* right;
    int height;
};

int myMaxInt2(int a, int b) { return (a > b) ? a : b; }

class HistoryAVL {
private:
    HistoricalRecord* root;

    int GetHeight(HistoricalRecord* n) { return (n == NULL) ? 0 : n->height; }
    int GetBalance(HistoricalRecord* n) { return (n == NULL) ? 0 : GetHeight(n->left) - GetHeight(n->right); }

    HistoricalRecord* RightRotate(HistoricalRecord* y) {
        HistoricalRecord* x = y->left;
        HistoricalRecord* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = myMaxInt2(GetHeight(y->left), GetHeight(y->right)) + 1;
        x->height = myMaxInt2(GetHeight(x->left), GetHeight(x->right)) + 1;
        return x;
    }

    HistoricalRecord* LeftRotate(HistoricalRecord* x) {
        HistoricalRecord* y = x->right;
        HistoricalRecord* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = myMaxInt2(GetHeight(x->left), GetHeight(x->right)) + 1;
        y->height = myMaxInt2(GetHeight(y->left), GetHeight(y->right)) + 1;
        return y;
    }

    HistoricalRecord* InsertRec(HistoricalRecord* node, int id, int urg, string type, string time) {
        if (node == NULL) {
            HistoricalRecord* newNode = new HistoricalRecord;
            newNode->incidentID = id;
            newNode->urgency = urg;
            newNode->type = type;
            newNode->resolveTime = time;
            newNode->left = newNode->right = NULL;
            newNode->height = 1;
            return newNode;
        }

        if (id < node->incidentID)
            node->left = InsertRec(node->left, id, urg, type, time);
        else if (id > node->incidentID)
            node->right = InsertRec(node->right, id, urg, type, time);
        else {
            return node;
        }

        node->height = 1 + myMaxInt2(GetHeight(node->left), GetHeight(node->right));
        int balance = GetBalance(node);

        if (balance > 1 && id < node->left->incidentID) return RightRotate(node);
        if (balance < -1 && id > node->right->incidentID) return LeftRotate(node);
        if (balance > 1 && id > node->left->incidentID) {
            node->left = LeftRotate(node->left);
            return RightRotate(node);
        }
        if (balance < -1 && id < node->right->incidentID) {
            node->right = RightRotate(node->right);
            return LeftRotate(node);
        }

        return node;
    }

    void InOrderRec(HistoricalRecord* node) {
        if (!node) return;
        InOrderRec(node->left);
        cout << "ID: " << node->incidentID << " | Type: " << node->type
            << " | Urgency: " << node->urgency << " | Time: " << node->resolveTime << endl;
        InOrderRec(node->right);
    }

    void SaveRec(HistoricalRecord* node, ofstream& out) {
        if (!node) return;
        out << node->incidentID << " " << node->urgency << " " << node->type << " " << node->resolveTime << "\n";
        SaveRec(node->left, out);
        SaveRec(node->right, out);
    }

public:
    HistoryAVL() { root = NULL; }

    void Insert(int id, int urg, string type, string time) {
        root = InsertRec(root, id, urg, type, time);
    }

    bool Search(int id) {
        HistoricalRecord* cur = root;
        while (cur) {
            if (id == cur->incidentID) return true;
            if (id < cur->incidentID) cur = cur->left;
            else cur = cur->right;
        }
        return false;
    }

    void GenerateReport() {
        cout << "--- Historical Incident Report ---\n";
        InOrderRec(root);
    }

    void SaveToFile(ofstream& out) {
        SaveRec(root, out);
    }
};
