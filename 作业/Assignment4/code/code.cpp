#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <iomanip>
#include <stack>
#include <string>
#include "mypq.h"

using namespace std;

const double INF = numeric_limits<double>::infinity();

class Graph
{
private:
    int V;
    vector<vector<pair<int, double>>> adj;

public:
    Graph(int vertices) : V(vertices), adj(vertices) {}

    void addDirectedEdge(int u, int v, double weight)
    {
        adj[u].emplace_back(v, weight);
    }

    bool dijkstra(int src, vector<double> &dist, vector<int> &prev)
    {
        dist.assign(V, INF);
        prev.assign(V, -1);
        dist[src] = 0;

        PriorityQueue *pq = PriorityQueue::create();
        pq->insert(src, 0);

        while (!pq->isEmpty())
        {
            int u = pq->extractMin();

            for (const auto &edge : adj[u])
            {
                int v = edge.first;
                double weight = edge.second;

                if (weight < 0)
                {
                    cout << "Error: Dijkstra encountered negative edge weight (" << u << "->" << v << "=" << weight << ")\n";
                    delete pq;
                    return false;
                }

                if (dist[v] > dist[u] + weight)
                {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    if (pq->contains(v))
                        pq->decreaseKey(v, dist[v]);
                    else
                        pq->insert(v, dist[v]);
                }
            }
        }

        delete pq;
        return true;
    }

    bool bellmanFord(int src, vector<double> &dist, vector<int> &prev)
    {
        dist.assign(V, INF);
        prev.assign(V, -1);
        dist[src] = 0;

        for (int i = 0; i < V - 1; ++i)
        {
            bool updated = false;
            for (int u = 0; u < V; ++u)
            {
                if (dist[u] == INF)
                    continue;
                for (const auto &edge : adj[u])
                {
                    int v = edge.first;
                    double weight = edge.second;
                    if (dist[v] > dist[u] + weight)
                    {
                        dist[v] = dist[u] + weight;
                        prev[v] = u;
                        updated = true;
                    }
                }
            }
            if (!updated)
                break;
        }

        for (int u = 0; u < V; ++u)
        {
            if (dist[u] == INF)
                continue;
            for (const auto &edge : adj[u])
            {
                int v = edge.first;
                double weight = edge.second;
                if (dist[v] > dist[u] + weight)
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Return path string like "<0,1,2>" including src and dest
    string getPath(int src, int dest, const vector<int> &prev, const vector<double> &dist)
    {
        if (dist[dest] == INF)
            return "< >";

        stack<int> pathStack;
        for (int cur = dest; cur != -1; cur = prev[cur])
            pathStack.push(cur);

        if (pathStack.top() != src)
            return "< >";

        string pathStr = "<";
        while (!pathStack.empty())
        {
            pathStr += to_string(pathStack.top());
            pathStack.pop();
            if (!pathStack.empty())
                pathStr += ",";
        }
        pathStr += ">";

        return pathStr;
    }

    void printShortestPaths(int src, const vector<double> &dist, const vector<int> &prev)
    {
        cout << "The shortest-length path is:\n";
        for (int i = 0; i < V; ++i)
        {
            if (dist[i] == INF)
                cout << i << "=INF; path = < >\n";
            else
            {
                cout << i << "=" << fixed << setprecision(0) << dist[i] << "; path = " << getPath(src, i, prev, dist) << "\n";
            }
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        cerr << "Error: Cannot open input file\n";
        return 1;
    }

    int V, src;
    inputFile >> V >> src;

    Graph g(V);

    int u, v;
    double weight;
    while (inputFile >> u >> v >> weight)
    {
        g.addDirectedEdge(u, v, weight);
    }

    vector<double> dist;
    vector<int> prev;

    cout << "=== Dijkstra Algorithm ===\n";
    if (g.dijkstra(src, dist, prev))
    {
        g.printShortestPaths(src, dist, prev);
    }
    else
    {
        cout << "Dijkstra algorithm terminated due to negative edge weight\n";
    }

    cout << "\n=== Bellman-Ford Algorithm ===\n";
    if (g.bellmanFord(src, dist, prev))
    {
        g.printShortestPaths(src, dist, prev);
    }
    else
    {
        cout << "Warning: Negative cycle detected, program terminating\n";
        return 1;
    }

    return 0;
}
