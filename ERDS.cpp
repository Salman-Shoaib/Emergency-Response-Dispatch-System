#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#include "IncidentHeap.cpp"
#include "VehicleHash.cpp"
#include "HistoryAVL.cpp"
#include "CityGraph.cpp"
#include "TaskQueue.cpp"
#include "CommandStack.cpp"

class ERDS {
private:
    IncidentHeap incidents;
    VehicleHash vehicles;
    HistoryAVL history;
    CityGraph city;
    TaskQueue tasks;
    CommandStack commands;

public:
    ERDS() {
        for (int i = 0; i < 10; ++i) city.AddLocation(i);
        city.AddRoad(0,1,5);
        city.AddRoad(1,2,3);
        city.AddRoad(2,3,2);
        city.AddRoad(0,4,10);
    }

    void LogIncident() {
        int id, loc, urg;
        string type;
        cout << "Enter Incident ID: "; cin >> id;
        if (incidents.IDExists(id) || history.Search(id)) {
            cout << "Error: Incident ID already exists (pending or resolved).\n";
            return;
        }
        cout << "Enter Location ID (0-99): "; cin >> loc;
        if (loc < 0 || loc >= 100) { cout << "Invalid location.\n"; return; }
        cout << "Enter Type (Medical/Fire/Crime): "; cin >> type;
        if (!(type == "Medical" || type == "Fire" || type == "Crime")) { cout << "Invalid type.\n"; return; }
        cout << "Enter Urgency (1-5): "; cin >> urg;
        if (urg < 1 || urg > 5) { cout << "Invalid urgency.\n"; return; }

        incidents.Insert(id, loc, type, urg);
        commands.Push("Logged Incident " + to_string(id));
        cout << "Incident Logged.\n";
    }

    void AddVehicle() {
        string id, type;
        int loc;
        cout << "Enter Vehicle ID: "; cin >> id;
        cout << "Enter Type (Ambulance/Fire/Police): "; cin >> type;
        if (!(type=="Ambulance"||type=="Fire"||type=="Police")) { cout << "Invalid type.\n"; return; }
        cout << "Enter Location ID: "; cin >> loc;
        // Note: CityGraph currently doesn't expose a public "exists" function.
        // If you want strict validation that location exists in graph, we can add
        // a CityGraph::LocationExists(int) method and check it here.
        bool ok = vehicles.Insert(id, type, loc);
        if (ok) {
            commands.Push("Added Vehicle " + id);
            cout << "Vehicle Added.\n";
        }
    }

    void Dispatch() {
        if (incidents.IsEmpty()) {
            cout << "No pending incidents.\n"; return;
        }
        Incident inc = incidents.ExtractMin();
        cout << "Processing Incident ID: " << inc.id << " (Urgency: " << inc.urgency << ")\n";
        string vType = "Police";
        if (inc.type == "Medical") vType = "Ambulance";
        else if (inc.type == "Fire") vType = "Fire";

        string vID = city.FindClosestVehicle(inc.locationID, vType, vehicles);
        if (vID != "NONE") {
            cout << "Vehicle " << vID << " found nearby! Dispatching...\n";
            vehicles.UpdateStatus(vID, "EnRoute", inc.locationID);
            commands.Push("Dispatched " + vID + " to " + to_string(inc.id));
            cout << "Incident resolved. Moving to History.\n";
            history.Insert(inc.id, inc.urgency, inc.type, "Now");
            vehicles.UpdateStatus(vID, "Available", inc.locationID);
        } else {
            cout << "No available " << vType << " found! Incident re-queued.\n";
            incidents.Insert(inc.id, inc.locationID, inc.type, inc.urgency);
        }
    }

    void AdminMenu() {
        int c;
        cout << "1. Add Task\n2. Process Task\n3. History Report\n4. Cmd Log\n5. Show Pending Incidents\n6. Show Vehicles\nChoice: ";
        cin >> c;
        if (c == 1) {
            string t; cout << "Task (single word/no spaces): "; cin >> t; tasks.Enqueue(t);
        } else if (c == 2) {
            string t = tasks.Dequeue();
            if (t != "") cout << "Did: " << t << endl;
            else cout << "No tasks.\n";
        } else if (c == 3) history.GenerateReport();
        else if (c == 4) commands.ViewHistory();
        else if (c == 5) incidents.DisplayAll();
        else if (c == 6) vehicles.DisplayAll();
        else cout << "Invalid.\n";
    }

    void MapMenu() {
        int c;
        cout << "1. Add Location\n2. Remove Location\n3. Add Road\n4. Remove Road\n5. Display Map\nChoice: ";
        cin >> c;
        if (c == 1) {
            int id; cout << "Location ID: "; cin >> id;
            if (city.AddLocation(id)) cout << "Location added.\n"; else cout << "Failed or exists.\n";
        } else if (c == 2) {
            int id; cout << "Location ID: "; cin >> id;
            if (city.RemoveLocation(id)) cout << "Location removed.\n"; else cout << "Failed (not exists).\n";
        } else if (c == 3) {
            int u,v,w; cout << "Src Dest Weight: "; cin >> u >> v >> w;
            if (city.AddRoad(u,v,w)) cout << "Road added.\n"; else cout << "Failed to add road.\n";
        } else if (c == 4) {
            int u,v; cout << "Src Dest: "; cin >> u >> v;
            if (city.RemoveRoad(u,v)) cout << "Road removed.\n"; else cout << "Failed.\n";
        } else if (c == 5) city.Display();
        else cout << "Invalid.\n";
    }

    void RemoveVehicleMenu() {
        string vid; cout << "Vehicle ID to remove: "; cin >> vid;
        if (vehicles.Remove(vid)) cout << "Vehicle removed.\n";
        else cout << "Vehicle not found.\n";
    }

    void ResolveIncidentMenu() {
        int id; cout << "Incident ID to resolve: "; cin >> id;
        bool removed = incidents.RemoveByID(id);
        if (!removed) {
            if (history.Search(id)) { cout << "Incident already resolved in history.\n"; return; }
            cout << "Incident not found among pending; will add as resolved record if you confirm.\n";
        }
        int urg; string type;
        cout << "Enter urgency for record (1-5): "; cin >> urg;
        cout << "Enter type (Medical/Fire/Crime): "; cin >> type;
        history.Insert(id, urg, type, "Manual");
        commands.Push("Manually resolved " + to_string(id));
        cout << "Incident closed and saved to history.\n";
    }

    void SaveData() {
        ofstream out("erds_data.txt");
        if (!out.is_open()) { cout << "Error saving file.\n"; return; }

        out << "VEHICLES\n";
        int ts = vehicles.GetTableSize();
        out << ts << "\n";
        for (int i = 0; i < ts; ++i) {
            HashNode* n = vehicles.GetBucket(i);
            while (n) {
                out << n->v.id << " " << n->v.type << " " << n->v.locationID << " " << n->v.status << "\n";
                n = n->next;
            }
        }
        out << "END_VEHICLES\n";

        out << "INCIDENTS\n";
        out << incidents.GetSize() << "\n";
        for (int i = 0; i < incidents.GetSize(); ++i) {
            Incident it = incidents.GetAt(i);
            out << it.id << " " << it.locationID << " " << it.type << " " << it.urgency << " " << it.status << "\n";
        }
        out << "END_INCIDENTS\n";

        out << "HISTORY\n";
        history.SaveToFile(out);
        out << "END_HISTORY\n";

        city.SaveGraph(out);

        out << "TASKS\n";
        tasks.SaveToFile(out);
        out << "END_TASKS\n";

        out << "CMDS\n";
        commands.SaveToFile(out);
        out << "END_CMDS\n";

        out.close();
        cout << "System State Saved.\n";
    }

    void LoadData() {
        ifstream in("erds_data.txt");
        if (!in.is_open()) { cout << "No saved state found (starting fresh).\n"; return; }

        string token;
        while (in >> token) {
            if (token == "VEHICLES") {
                int tableSz = 0; in >> tableSz;
                string vid, vtype, vstat; int vloc;
                while (in >> vid) {
                    if (vid == "END_VEHICLES") break;
                    in >> vtype >> vloc >> vstat;
                    vehicles.Insert(vid, vtype, vloc);
                    vehicles.UpdateStatus(vid, vstat, vloc);
                }
            } else if (token == "INCIDENTS") {
                int count = 0; in >> count;
                for (int i = 0; i < count; ++i) {
                    int id, loc, urg; string type, status;
                    in >> id >> loc >> type >> urg >> status;
                    incidents.Insert(id, loc, type, urg);
                }
                string s; in >> s;
            } else if (token == "HISTORY") {
                string maybe;
                while (in >> maybe) {
                    if (maybe == "END_HISTORY") break;
                    int hid = atoi(maybe.c_str());
                    int hurg; string htype, htime;
                    in >> hurg >> htype >> htime;
                    history.Insert(hid, hurg, htype, htime);
                }
            } else if (token == "GRAPH_NODES") {
                in.seekg(- (int)token.size(), ios::cur);
                city.LoadGraph(in);
            } else if (token == "TASKS") {
                string line;
                getline(in, line);
                while (getline(in, line)) {
                    if (line == "END_TASKS") break;
                    if (line.size() == 0) continue;
                    tasks.Enqueue(line);
                }
            } else if (token == "CMDS") {
                string line;
                getline(in, line);
                while (getline(in, line)) {
                    if (line == "END_CMDS") break;
                    if (line.size() == 0) continue;
                    commands.Push(line);
                }
            } else {
                string skip;
                getline(in, skip);
            }
        }

        in.close();
        cout << "System State Loaded.\n";
    }
};
