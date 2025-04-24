#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }

    float serialBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "Serial BFS Traversal: ";
        auto start_time = high_resolution_clock::now();

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            // cout << node << " ";

            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        auto end_time = high_resolution_clock::now();
        cout << "\nExecution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds\n";
        return (end_time - start_time).count();
    }

    float parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "Parallel BFS Traversal: ";
        auto start_time = high_resolution_clock::now();

        while (!q.empty()) {
            int size = q.size();
            vector<int> nextLevel;

            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int node;
                #pragma omp critical
                {
                    node = q.front();
                    q.pop();
                }
                // cout << node << " ";

                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        nextLevel.push_back(neighbor);
                    }
                }
            }

            for (int n : nextLevel) {
                q.push(n);
            }
        }
        auto end_time = high_resolution_clock::now();
        cout << "\nExecution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds\n";
        return (end_time - start_time).count();
    }

    float serialDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        cout << "Serial DFS Traversal: ";
        auto start_time = high_resolution_clock::now();

        while (!s.empty()) {
            int node = s.top();
            s.pop();
            // cout << node << " ";

            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    s.push(neighbor);
                }
            }
        }
        auto end_time = high_resolution_clock::now();
        cout << "\nExecution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds\n";
        return (end_time - start_time).count();
    }

    float parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        s.push(start);
        visited[start] = true;

        cout << "Parallel DFS Traversal: ";
        auto start_time = high_resolution_clock::now();

        while (!s.empty()) {
            int node;
            #pragma omp critical
            {
                node = s.top();
                s.pop();
            }
            // cout << node << " ";

            vector<int> neighbors;
            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    neighbors.push_back(neighbor);
                }
            }

            #pragma omp parallel for
            for (int i = neighbors.size() - 1; i >= 0; i--) {
                s.push(neighbors[i]);
            }
        }
        auto end_time = high_resolution_clock::now();
        cout << "\nExecution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds\n";
        return (end_time - start_time).count();
    }
};

int main() {
    Graph g(5000);
    for(int i=0; i<5000; i++){
        for(int j=0; j<i; j++) g.addEdge(i,j);
    }

    float t1 = g.serialBFS(0);
    float t2 = g.parallelBFS(0);
    float t3 = g.serialDFS(0);
    float t4 = g.parallelDFS(0);
    cout<<"BFS SPEED FACTOR "<<t1/t2<<"\n";
    cout<<"DFS SPEED FACTOR "<<t3/t4<<"\n";
    return 0;
}
