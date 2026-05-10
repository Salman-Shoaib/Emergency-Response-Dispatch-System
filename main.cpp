#include <iostream>
#include "ERDS.cpp"
using namespace std;

int main() {
    ERDS erdsApp;
    erdsApp.LoadData();

    int choice = -1;
    while (choice != 0) {
        cout << "\n=== ERDS Lite Dispatch System ===\n";
        cout << "1. Log Incident (Heap)\n";
        cout << "2. Dispatch Next (Priority->BFS)\n";
        cout << "3. Add Vehicle (Hash)\n";
        cout << "4. Admin/Reports (Queue/Stack/AVL)\n";
        cout << "6. Add/Remove Map Location or Roads\n";
        cout << "7. Remove Vehicle\n";
        cout << "8. Resolve/Close Incident (Manual)\n";
        cout << "5. Save State\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000,'\n');
            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: erdsApp.LogIncident(); break;
        case 2: erdsApp.Dispatch(); break;
        case 3: erdsApp.AddVehicle(); break;
        case 4: erdsApp.AdminMenu(); break;
        case 5: erdsApp.SaveData(); break;
        case 6: erdsApp.MapMenu(); break;
        case 7: erdsApp.RemoveVehicleMenu(); break;
        case 8: erdsApp.ResolveIncidentMenu(); break;
        case 0: erdsApp.SaveData(); cout << "Exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }
    }

    return 0;
}
