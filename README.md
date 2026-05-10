ERDS Lite: Emergency Response Dispatch System
ERDS Lite is a high-performance C++ application designed to manage and optimize emergency response workflows. The system simulates a city-wide dispatch center, handling real-time incident logging, vehicle tracking, and automated dispatching using custom-built data structures.

🚀 Key Features
Priority-Based Incident Logging: Automatically ranks incoming emergencies (Medical, Fire, Crime) based on urgency levels using a priority queue.

Proximity-Aware Dispatching: Uses graph traversal to identify the closest available response unit (Ambulance, Fire Truck, or Police Car) based on location IDs.

Dynamic City Mapping: Manage city locations and road networks in real-time, allowing for a flexible response environment.

Administrative Oversight: Track maintenance tasks, view command history, and generate historical incident reports.

State Persistence: Saves the entire system state—including incidents, vehicles, and the city graph—to a local file to resume operations later.

🛠️ Data Structures Implemented
This project avoids standard libraries to implement custom, high-efficiency data structures tailored for emergency dispatch:

Component	Data Structure	Purpose
Incident Management	Min-Heap	Enables O(1) access to the most urgent pending incidents.
Vehicle Tracking	Hash Table	Provides O(1) average-case lookup and status updates for vehicles using chaining.
City Map	Adjacency List Graph	Represents city locations and roads for traversal and distance tracking.
Response History	AVL Tree	Stores resolved incidents in a self-balancing BST for O(logn) searching.
Command Log	Stack	Tracks administrative actions in a LIFO manner for history viewing.
Maintenance	Queue	Manages system tasks in a fair, FIFO (First-In, First-Out) order.
📂 File Structure
The project is organized into modular components, each responsible for a specific data structure or system logic:

main.cpp: Entry point providing the CLI-based management menu.

ERDS.cpp: Central controller integrating all data structures and business logic.

IncidentHeap.cpp: Implementation of the urgency-based priority heap.

VehicleHash.cpp: Implementation of the chained hash table for vehicle management.

CityGraph.cpp: Logic for city navigation and BFS-based resource locating.

HistoryAVL.cpp: Self-balancing tree for searchable resolved incident logs.

CommandStack.cpp: Logic for tracking recent user commands.

TaskQueue.cpp: Queue implementation for pending maintenance tasks.

💻 Getting Started
Prerequisites
A C++ compiler (e.g., GCC/G++).

Compilation
Navigate to the project directory and compile using:

Bash
g++ main.cpp -o ERDS
Running the System
Bash
./ERDS
🎮 Usage
The system operates through a menu-driven interface:

Log Incident: Enter ID, location, type, and urgency (1-5).

Dispatch Next: Automatically assigns the nearest available vehicle to the highest-priority incident.

Add/Remove Vehicle: Register or remove emergency units from the system.

Map Menu: Add or remove locations and roads to update the city topography.

Admin/Reports: View historical data, pending tasks, and recent command logs.

Author: Salman Shoaib

Student ID: 24P-0583

Institution: FAST NUCES Islamabad
