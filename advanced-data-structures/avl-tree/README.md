# Smart City Critical Event System

A critical urban event management system built in C, using a
self-balancing AVL Tree as its core data structure.

Developed as an academic project for the Advanced Data Structures course.


## Description

The system monitors and manages critical urban events in real time,
such as traffic accidents, power outages, flooding and fires.

Each event is stored as a node in an AVL Tree, ordered by event ID,
guaranteeing O(log n) time complexity for insertions, removals
and searches (even under high data volume).


## ✨ Features

- **Event Management**: register, remove (resolved only) and search by ID
- **Advanced Queries**:
  - Active events by severity range
  - Active events by city region (ordered by ID)
  - Events by ID range (with subtree pruning for efficiency)
- **Update Operations**: mark events as resolved, update severity
- **AVL Tree Metrics**:
  - Total tree height
  - Total node count
  - Active event count
  - Average balance factor
  - Total rotations performed
- **Interactive text menu**: with input validation and submenus


## 🗂️ Project Structure

```
smart-city-events/
├── main.c              # Entry point
└── src/
    ├── event.h         # Event types and node structure
    ├── event.c         # Helper functions (print, type/status to string)
    ├── avl.h           # AVL tree interface
    ├── avl.c           # AVL tree implementation
    ├── queries.h       # Advanced queries interface
    ├── queries.c       # Advanced queries implementation
    ├── menu.h          # Menu interface
    └── menu.c          # Interactive menu implementation
```


## 🔧 Compilation

Make sure you have [MinGW](https://www.mingw-w64.org/) installed.

Open a terminal in the project root folder and run:

```powershell
gcc main.c src/event.c src/avl.c src/queries.c src/menu.c -o smart-city -lm
```

---

## ▶️ Running

**Windows (PowerShell):**
```powershell
.\smart-city.exe
```

**Linux / macOS:**
```bash
./smart-city
```

---

Thanks!
