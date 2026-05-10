#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct AdjNode {
    int dest;
    int weight;
    AdjNode* next;
};

struct Vertex {
    int id;
    AdjNode* head;
    Vertex() { id = -1; head = NULL; }
};

struct IntNode { int val; IntNode* next; };
class IntQueue {
    IntNode* f; IntNode* r;
public:
    IntQueue() { f = r = NULL; }
    void push(int v) {
        IntNode* n = new IntNode; n->val = v; n->next = NULL;
        if (!r) f = r = n; else { r->next = n; r = n; }
    }
    int pop() {
        if (!f) return -1;
        IntNode* t = f; int v = t->val; f = f->next;
        if (!f) r = NULL; delete t; return v;
    }
    bool empty() { return f == NULL; }
};

class VehicleHash; // forward declaration

class CityGraph {
private:
    Vertex* vertices;
    int maxNodes;
    bool exists_nodes[200];
public:
    CityGraph() {
        maxNodes = 100;
        vertices = new Vertex[maxNodes];
        for (int i = 0; i < maxNodes; ++i) { vertices[i].id = i; vertices[i].head = NULL; exists_nodes[i]=false; }
    }
    ~CityGraph() {
        for (int i = 0; i < maxNodes; ++i) {
            AdjNode* cur = vertices[i].head;
            while (cur) { AdjNode* nxt = cur->next; delete cur; cur = nxt; }
        }
        delete[] vertices;
    }

    bool AddLocation(int id) {
        if (id < 0 || id >= maxNodes) return false;
        if (exists_nodes[id]) return false;
        exists_nodes[id] = true;
        vertices[id].head = NULL;
        return true;
    }

    bool RemoveLocation(int id) {
        if (id < 0 || id >= maxNodes) return false;
        if (!exists_nodes[id]) return false;
        AdjNode* cur = vertices[id].head;
        while (cur) { AdjNode* nxt = cur->next; delete cur; cur = nxt; }
        vertices[id].head = NULL;
        for (int i = 0; i < maxNodes; ++i) {
            if (!exists_nodes[i]) continue;
            AdjNode* prev = NULL; AdjNode* p = vertices[i].head;
            while (p) {
                if (p->dest == id) {
                    AdjNode* toDel = p;
                    if (prev) prev->next = p->next; else vertices[i].head = p->next;
                    p = p->next;
                    delete toDel;
                } else { prev = p; p = p->next; }
            }
        }
        exists_nodes[id] = false;
        return true;
    }

    bool AddRoad(int src, int dest, int weight) {
        if (src < 0 || src >= maxNodes || dest < 0 || dest >= maxNodes) return false;
        if (!exists_nodes[src] || !exists_nodes[dest]) return false;
        AdjNode* n = new AdjNode; n->dest = dest; n->weight = weight; n->next = vertices[src].head; vertices[src].head = n;
        AdjNode* m = new AdjNode; m->dest = src; m->weight = weight; m->next = vertices[dest].head; vertices[dest].head = m;
        return true;
    }

    bool RemoveRoad(int src, int dest) {
        if (!exists_nodes[src] || !exists_nodes[dest]) return false;
        AdjNode* prev = NULL; AdjNode* cur = vertices[src].head;
        while (cur) {
            if (cur->dest == dest) {
                AdjNode* toDel = cur;
                if (prev) prev->next = cur->next; else vertices[src].head = cur->next;
                cur = cur->next;
                delete toDel;
                break;
            } else { prev = cur; cur = cur->next; }
        }
        prev = NULL; cur = vertices[dest].head;
        while (cur) {
            if (cur->dest == src) {
                AdjNode* toDel = cur;
                if (prev) prev->next = cur->next; else vertices[dest].head = cur->next;
                cur = cur->next;
                delete toDel;
                break;
            } else { prev = cur; cur = cur->next; }
        }
        return true;
    }

    string FindClosestVehicle(int startLoc, const string& neededType, VehicleHash& vTable);

    void SaveGraph(ofstream& out) {
        out << "GRAPH_NODES\n";
        for (int i = 0; i < maxNodes; ++i) if (exists_nodes[i]) out << i << " ";
        out << "\nEND_GRAPH_NODES\n";
        out << "GRAPH_EDGES\n";
        for (int i = 0; i < maxNodes; ++i) {
            if (!exists_nodes[i]) continue;
            AdjNode* cur = vertices[i].head;
            while (cur) {
                if (i < cur->dest) out << i << " " << cur->dest << " " << cur->weight << "\n";
                cur = cur->next;
            }
        }
        out << "END_GRAPH_EDGES\n";
    }

    void LoadGraph(ifstream& in) {
        string token;
        if (!(in >> token)) return;
        if (token != "GRAPH_NODES") return;
        while (in >> token) {
            if (token == "END_GRAPH_NODES") break;
            int nid = atoi(token.c_str());
            if (nid >= 0 && nid < maxNodes) AddLocation(nid);
        }
        if (!(in >> token)) return;
        if (token != "GRAPH_EDGES") return;
        while (in >> token) {
            if (token == "END_GRAPH_EDGES") break;
            int u = atoi(token.c_str());
            int v, w;
            in >> v >> w;
            AddRoad(u, v, w);
        }
    }

    void Display() {
        cout << "--- City Graph ---\n";
        for (int i = 0; i < maxNodes; ++i) {
            if (!exists_nodes[i]) continue;
            cout << "Node " << i << " -> ";
            AdjNode* cur = vertices[i].head;
            while (cur) {
                cout << "(" << cur->dest << "," << cur->weight << ") ";
                cur = cur->next;
            }
            cout << endl;
        }
    }
};

string CityGraph::FindClosestVehicle(int startLoc, const string& neededType, VehicleHash& /*vTable*/) {
    if (startLoc < 0 || startLoc >= maxNodes) return "NOT_FOUND";
    if (!exists_nodes[startLoc]) {
        // find nearest existing node using BFS
        IntQueue q;
        bool visited[200] = { false };
        q.push(startLoc);
        visited[startLoc] = true;
        while (!q.empty()) {
            int u = q.pop();
            if (u >= 0 && u < maxNodes && exists_nodes[u]) {
                // placeholder: we found an existing node, but we don't know vehicles yet.
                // return its id as a string so callers can at least get a location.
                return to_string(u);
            }
            // push neighbors
            AdjNode* cur = vertices[u].head;
            while (cur) {
                if (!visited[cur->dest]) {
                    visited[cur->dest] = true;
                    q.push(cur->dest);
                }
                cur = cur->next;
            }
        }
        return "NOT_FOUND";
    } else {
        // startLoc exists. As placeholder, return the startLoc id.
        // Real implementation should check vTable for vehicles of neededType at startLoc.
        return to_string(startLoc);
    }
}
