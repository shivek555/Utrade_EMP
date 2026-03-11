**Utrade_EMP**

A simplified **task execution engine** similar to build systems like **Make** or orchestration tools like **Airflow**, implemented in **Turbo C++ (C++98 style)**.

The scheduler executes tasks while respecting:

* **Task priorities**
* **Dependency graphs (DAG)**
* **Worker pool limits**

Because **Turbo C++ does not support modern multithreading**, the scheduler simulates concurrent execution using a **scheduler tick loop**.

---

# 1. Design Decisions

## 1.1 Turbo C++ Compatibility

The system is designed specifically to compile under **Turbo C++ / Borland C++**, therefore:

* No modern C++ features
* No STL containers
* No threading libraries
* Uses legacy headers

```
<iostream.h>
<fstream.h>
<conio.h>
<dos.h>
<time.h>
```

All structures are implemented using:

* Static arrays
* Structs
* Simple loops

This ensures **maximum compatibility with the legacy compiler**.

---

## 1.2 Modular Architecture

The system is split into independent components:

```
Task          → Represents a job
Graph         → Dependency DAG
Scheduler     → Core execution engine
Worker        → Simulated worker instance
PriorityQueue → Selects next ready task
```

Repository layout:

```
task_scheduler_turbo_cpp/

main.cpp
task.h
task.cpp
graph.h
graph.cpp
scheduler.h
scheduler.cpp
priority_queue.cpp
example_tasks.json
README.md
.gitignore
```

This separation keeps the code **clean and easy to reason about**.

---

# 2. Data Structures

Since STL containers are avoided, we rely on **static arrays**.

## Task Structure

Each task stores execution metadata.

```
struct Task
{
    id
    name
    priority
    duration_ms

    dependencies
    dependency count

    execution state
    start/end timestamps
}
```

Runtime fields include:

* `remaining_time`
* `started`
* `completed`

---

## Dependency Graph

Dependencies are stored using an **adjacency matrix**.

```
adj[MAX_TASKS][MAX_TASKS]
```

Advantages:

* Simple implementation
* Fast lookup
* Works well for small DAGs

Also stores:

```
indegree[MAX_TASKS]
```

Which allows **efficient topological validation**.

---

## Worker Pool

Workers are simulated using a struct:

```
struct Worker
{
    busy
    task_index
}
```

Workers are stored in an array:

```
Worker workers[MAX_WORKERS]
```

---

## Ready Queue

Tasks that are ready to run are stored in:

```
int ready[MAX_TASKS]
```

Priority selection chooses the **highest priority task**.

---

# 3. Algorithms Used

## 3.1 Cycle Detection — Kahn's Algorithm

Before execution we validate that the dependency graph is a **DAG**.

Steps:

1. Find nodes with `indegree = 0`
2. Remove them from the graph
3. Reduce indegree of dependent nodes
4. Repeat

If processed nodes < total nodes → **cycle detected**.

Time complexity:

```
O(V + E)
```

---

## 3.2 Priority Scheduling

Among ready tasks we choose the task with the **highest priority**.

Lower number = higher priority.

Example:

```
priority 1 → highest
priority 5 → lowest
```

Selection uses a simple scan:

```
O(N)
```

This was chosen instead of a heap to keep the implementation **Turbo C++ friendly**.

---

## 3.3 Worker Pool Simulation

Turbo C++ lacks modern threading.

Therefore we simulate concurrency using a **scheduler loop**.

Pseudo-flow:

```
while(tasks remain)
    update ready tasks
    assign tasks to idle workers
    delay(100ms)
    decrease remaining time
    mark completed tasks
    unlock dependent tasks
```

This approximates **parallel execution**.

---

## 3.4 Critical Path Calculation

The **critical path** represents the longest dependency chain in the DAG.

It determines the **minimum possible execution time**.

We compute:

```
longest_path[v] =
    max(longest_path[u] + duration[v])
```

for all edges `u → v`.

---

# 4. Execution Flow

Example tasks:

```
A → Fetch
B → Transform (depends on A)
C → Load (depends on A)
D → Report (depends on B,C)
```

Execution timeline:

```
Worker 1 → A
Worker 2 → idle

After A completes

Worker 1 → B
Worker 2 → C

After B,C complete

Worker 1 → D
```

---

# 5. Example Output

```
12:00:01 | worker-1 | START | A
12:00:02 | worker-1 | DONE  | A
12:00:02 | worker-2 | START | B
12:00:02 | worker-3 | START | C
12:00:03 | worker-2 | DONE  | B
12:00:03 | worker-3 | DONE  | C
12:00:03 | worker-1 | START | D
12:00:04 | worker-1 | DONE  | D
```

Final report:

```
Total execution time: 4000 ms

Task A
start: 100
end: 1100

Task B
start: 1100
end: 1900

Task C
start: 1100
end: 1700

Task D
start: 1900
end: 2600

Critical path: 2600 ms
```

---

# 6. Build Instructions

This project is designed for **Turbo C++ / Borland C++**.

## Compile

Inside Turbo C++:

```
tcc main.cpp scheduler.cpp graph.cpp task.cpp priority_queue.cpp
```

## Run

```
main.exe --workers 4
```

Default workers = **4**.

---

# 7. How Tasks Are Executed

The scheduler performs:

1. **Load tasks from JSON**
2. **Build dependency graph**
3. **Check for cycles**
4. **Initialize worker pool**
5. **Run scheduling loop**
6. **Generate execution report**

---

# 8. Trade-offs and Limitations

Because the implementation targets **Turbo C++**, several trade-offs were required.

### No Modern Containers

Instead of:

```
vector
priority_queue
unordered_map
```

We use **static arrays**.

This limits scalability but simplifies compatibility.

---

### Simple Priority Selection

We use **linear scan** instead of a heap.

```
O(N)
```

A heap implementation would reduce scheduling cost but increase complexity.

---

### Limited JSON Parser

The JSON loader assumes a **simple predictable format**.

A full JSON parser would require a much larger codebase.

---

### Simulated Concurrency

Actual threads are unavailable in Turbo C++.

The system simulates parallelism using a **scheduler tick loop**.

---

# 9. Future Improvements

Possible enhancements:

* Binary heap priority queue
* Dynamic memory task storage
* Robust JSON parser
* Task retries and failure handling
* Graphviz DAG visualization
* Event-driven scheduler

---

# 10. Key Takeaways

This project demonstrates:

* DAG-based scheduling
* Build-system style orchestration
* Priority-based task selection
* Worker pool management
* Legacy compiler constraints

The focus was on **clean design, clear algorithms, and working code** rather than unnecessary complexity.

---
