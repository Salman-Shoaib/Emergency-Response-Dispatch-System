# 🚑 ERDS Lite: Emergency Response Dispatch System

ERDS Lite is a high-performance **C++ application** designed to manage and optimize emergency response workflows. The system simulates a city-wide dispatch center, handling real-time incident logging, vehicle tracking, and automated dispatching using custom-built data structures.

---

# 🚀 Key Features

## 🔥 Priority-Based Incident Logging
Automatically ranks incoming emergencies (**Medical, Fire, Crime**) based on urgency levels using a **priority queue (Min-Heap)**.

## 🚓 Proximity-Aware Dispatching
Uses graph traversal algorithms to identify the **closest available response unit** (Ambulance, Fire Truck, or Police Car) based on location IDs.

## 🗺️ Dynamic City Mapping
Manage city locations and road networks in real time, allowing a flexible and scalable response environment.

## 📊 Administrative Oversight
Track maintenance tasks, view command history, and generate historical incident reports.

## 💾 State Persistence
Saves the entire system state — including incidents, vehicles, and the city graph — to a local file so operations can resume later.

---

# 🛠️ Data Structures Implemented

This project avoids relying heavily on STL containers and instead implements custom high-efficiency data structures tailored for emergency dispatch operations.

| Component | Data Structure | Purpose |
|----------|----------------|---------|
| Incident Management | Min-Heap | Enables efficient access to the most urgent pending incidents |
| Vehicle Tracking | Hash Table | Provides O(1) average-case lookup and updates using chaining |
| City Map | Adjacency List Graph | Represents city locations and road networks |
| Response History | AVL Tree | Stores resolved incidents with O(log n) search performance |
| Command Log | Stack | Tracks recent administrative actions in LIFO order |
| Maintenance Tasks | Queue | Manages pending tasks using FIFO scheduling |

---

# 📂 Project Structure

```bash
ERDS-Lite/
│
├── main.cpp              # Entry point and CLI menu system
├── ERDS.cpp              # Core system controller and business logic
├── IncidentHeap.cpp      # Min-Heap implementation for incidents
├── VehicleHash.cpp       # Chained hash table for vehicle management
├── CityGraph.cpp         # Graph traversal and city mapping logic
├── HistoryAVL.cpp        # AVL Tree for resolved incident records
├── CommandStack.cpp      # Stack for command history tracking
├── TaskQueue.cpp         # Queue for maintenance task management
│
└── data/
    └── state.txt         # Saved system state
```

---

# ⚙️ Getting Started

## 📋 Prerequisites

- A C++ compiler such as **GCC/G++**
- Terminal or command prompt

---

# 🧩 Compilation

Navigate to the project directory and compile using:

```bash
g++ main.cpp -o ERDS.exe
```

---

# ▶️ Running the System

### Linux / macOS

```bash
./ERDS
```

### Windows

```bash
ERDS.exe
```

---

# 🎮 Usage Guide

The application operates through a **menu-driven CLI interface**.

## 📌 Available Operations

### 🚨 Log Incident
Enter:
- Incident ID
- Location ID
- Emergency Type
- Urgency Level (1–5)

---

### 🚑 Dispatch Next Incident
Automatically:
- Selects the highest-priority incident
- Finds the nearest available response vehicle
- Dispatches the unit

---

### 🚓 Vehicle Management
- Add emergency vehicles
- Remove vehicles
- Update vehicle availability

---

### 🗺️ Map Management
- Add locations
- Remove locations
- Create or delete roads between locations

---

### 📈 Reports & Administration
- View resolved incidents
- Check maintenance queue
- Display command history
- Generate historical reports

---

# 🧠 Core Concepts Used

- Object-Oriented Programming (OOP)
- Graph Traversal Algorithms
- AVL Tree Rotations
- Heap Operations
- Hashing with Chaining
- Queue and Stack Management
- File Handling & Persistence

---

# 📊 Time Complexity Overview

| Operation | Complexity |
|-----------|------------|
| Insert Incident | O(log n) |
| Get Highest Priority Incident | O(1) |
| Vehicle Lookup | O(1) average |
| Graph Traversal (BFS) | O(V + E) |
| AVL Insert/Search | O(log n) |
| Queue Operations | O(1) |
| Stack Operations | O(1) |

---

# 🔮 Future Improvements

- GUI-based control panel
- GPS-based shortest path routing
- Real-time vehicle tracking
- Multi-threaded dispatch engine
- Database integration
- Networked multi-user support

---

# 👨‍💻 Author

Developed as a **Data Structures Project** focused on building real-world systems using custom implementations of fundamental data structures in C++.

---

# 📜 License

This project is open-source and available for educational and learning purposes.
