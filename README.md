# MeshGraph-and-Binary-Heap-Implementation.

Computer-generated 3D imagery, which is implemented through a Graph data structure,<br />
MeshGraph.cpp has functions that create and also colorize particular nodes and edges, <br />
BinaryHeap.cpp functions are helping the coloring process, <br />

# 3D objects:<br />
# Person snowboarding: <br />
![Screenshot from 2023-01-11 18-30-00](https://user-images.githubusercontent.com/95969634/211882007-d05f215d-6052-4ff7-adfb-1233ff219ed1.png)
<br />

# A face: <br />
![Screenshot from 2023-01-11 18-34-31](https://user-images.githubusercontent.com/95969634/211882162-681df592-8647-44df-b70f-d0cd949c6aa1.png)

![Screenshot from 2023-01-11 18-36-43](https://user-images.githubusercontent.com/95969634/211882222-93cc97ea-2965-447a-91ae-6814e8eaffdb.png)
<br />

# A plane: <br />  
![Screenshot from 2023-01-11 18-36-31](https://user-images.githubusercontent.com/95969634/211882226-4f28a3e7-228d-460e-9560-631af461770c.png)

![Screenshot from 2023-01-11 18-36-27](https://user-images.githubusercontent.com/95969634/211882647-a4bd2675-43e2-466e-b445-31c58123e524.png)


Adjacency list style of implementation is used for Graph <br />
Edges are bi-directional <br />
Binary heap Implementation helps to develop the shortest path algorithm on the MeshGraph data structure<br />

## Overview
The project is designed to implement a graph data structure to represent and manipulate a 3D object. The project involves implementing a **binary heap data structure** to support efficient shortest path calculations and utilizing **various painting methods** to color 3D object vertices. This implementation requires working with **graph structures**, **Dijkstra's shortest path algorithm**, **breadth-first search (BFS)**, and **binary heap operations**.

## Features

### 1. Binary Heap Implementation
`Binary heap` implementation consists of heap data structure functionality. In addition to that, it also has a change priority functionality. This functionality is useful for implementing the `shortest path` algorithm on the `MeshGraph` data structure.

There are key differences between the classic heap and this data structure. First of all, each element of this data structure is unique. This will be determined by the `uniqueId` variable on the heap elements. So, this data structure does not accept another element with the same unique id.
It is important to mention that this data structure is implicitly a `min heap`; meaning that, the root of the data structure is the smallest item that is determined by the weight variable.

#### Binary Heap API
```cpp
class BinaryHeap {
private:
    std::vector<HeapElement> elements;
public:
    BinaryHeap();
    bool Add(int uniqueId, double weight);
    void PopHeap(int& outUniqueId, double& outWeight);
    bool ChangePriority(int uniqueId, double newWeight);
    int HeapSize() const;
};
```

### 2. MeshGraph Implementation
- Uses an **adjacency list representation**.
- Edges are **bi-directional** with non-negative weights.
- Supports shortest path calculations using **Dijkstra's Algorithm**.
- Defines **vertex positions in 3D space**.

Graph holds its data in a dynamic array and an array of linked list. For these, the class utilizes `std::vector<T>` class and `std::list<T>` class from the Standard Template Library (STL). Graph class and its helper structures are declared in `MeshGraph.h` header file

#### MeshGraph API
```cpp
class MeshGraph {
private:
    std::vector<Vertex> vertices;
    std::vector<std::list<Vertex*>> adjList;
public:
    MeshGraph(const std::vector<Double3>& vertexPositions, const std::vector<IdPair>& edges);
    double AverageDistanceBetweenVertices() const;
    double AverageEdgePerVertex() const;
    int TotalVertexCount() const;
    int TotalEdgeCount() const;
    int VertexEdgeCount(int vertexId) const;
    void ImmediateNeighbours(std::vector<int>& outVertexIds, int vertexId) const;
    void PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex, int vertexIdFrom, int vertexIdTo, const Color& color) const;
    void PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex, int vertexId, const Color& color, int maxDepth, FilterType type, double alpha) const;
    void PaintInRangeEuclidean(std::vector<Color>& outputColorAllVertex, int vertexId, const Color& color, int maxDepth, FilterType type, double alpha) const;
    static void WriteColorToFile(const std::vector<Color>& colors, const std::string& fileName);
    static void PrintColorToStdOut(const std::vector<Color>& colors);
};
```

### 3. Graph Coloring and Painting
- Implements various **painting techniques** on the 3D object.
- **PaintInBetweenVertex**: Colors the shortest path between two vertices.
- **PaintInRangeGeodesic**: Colors vertices within a given depth using BFS.
- **PaintInRangeEuclidean**: Colors vertices based on direct Euclidean distance.
- Utilizes **Gaussian and Box filtering** to determine color intensity.

### 4. Graph Calculations
- **Shortest Path Calculation** using **Dijkstra's Algorithm**.
- **Average Distance Between Vertices**.
- **Edge and Vertex Counts**.
- **Immediate Neighbours Calculation**.

## Technical Details
### 1. Color Representation
Each vertex is assigned an **RGB color**:
```cpp
struct Color {
    unsigned char r, g, b;
};
```

### 2. Vertex Structure
Each vertex has a **unique ID** and **3D position**:
```cpp
struct Vertex {
    int id;
    Double3 position3D;
};
```

### 3. Filtering Methods
Gaussian and Box filtering techniques are used to determine how colors propagate:
```cpp
f(x) = e^(-x^2 / α^2)  // Gaussian Filter
f(x) = 1 if -α ≤ x ≤ α, otherwise 0 // Box Filter
```
