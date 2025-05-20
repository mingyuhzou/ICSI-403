#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <regex>
#include <stdexcept>
#include <algorithm>

using namespace std;

// Global variables and data structures
int rows, cols;
vector<vector<string>> table;    // Stores the original spreadsheet
map<string, vector<string>> adj; // Adjacency list for dependency graph
map<string, bool> visited;       // Tracks visited nodes during DFS
map<string, bool> onPath;        // Tracks nodes on current DFS path (for cycle detection)
stack<string> topoStack;         // Stack to store topological order
map<string, string> expressions; // Maps cell names to original expressions
map<string, int> values;         // Stores computed values of cells

// Convert row and column index to cell name (e.g., A1, B2)
string getCellName(int row, int col)
{
    char colChar = 'A' + col;
    return string(1, colChar) + to_string(row + 1);
}

// Read spreadsheet data from input file
void readInput(const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Cannot open file.\n";
        exit(1);
    }
    file >> rows >> cols;
    table.resize(rows, vector<string>(cols));
    string line;
    getline(file, line); // Skip the rest of the first line

    for (int i = 0; i < rows; ++i)
    {
        getline(file, line);
        stringstream ss(line);
        for (int j = 0; j < cols; ++j)
        {
            ss >> table[i][j];
            string cell = getCellName(i, j);
            expressions[cell] = table[i][j];
        }
    }
}

// Extract all cell dependencies from a given expression
vector<string> extractDependencies(const string &expr)
{
    vector<string> deps;
    regex cellRegex("([A-Z][1-9][0-9]*)"); // Matches A1 to Z99 etc.
    auto begin = sregex_iterator(expr.begin(), expr.end(), cellRegex);
    auto end = sregex_iterator();
    for (auto it = begin; it != end; ++it)
    {
        deps.push_back(it->str());
    }
    return deps;
}

// Build a directed graph based on cell dependencies
void buildGraph()
{
    for (const auto &[cell, expr] : expressions)
    {
        vector<string> deps = extractDependencies(expr);
        for (const string &dep : deps)
        {
            adj[dep].push_back(cell); // dep → cell
        }
        if (adj.find(cell) == adj.end())
        {
            adj[cell] = {}; // Ensure all cells appear in the graph
        }
    }
}

// Depth-First Search with cycle detection
bool dfs(const string &node)
{
    visited[node] = true;
    onPath[node] = true;

    for (const string &neighbor : adj[node])
    {
        if (!visited[neighbor])
        {
            if (!dfs(neighbor))
                return false;
        }
        else if (onPath[neighbor])
        {
            cerr << "Error: Circular dependency detected involving " << node << " and " << neighbor << ".\n";
            return false;
        }
    }

    onPath[node] = false;
    topoStack.push(node); // Add to topological sort
    return true;
}

// Perform topological sort using DFS
vector<string> topologicalSort()
{
    visited.clear();
    onPath.clear();

    // Initialize visited and onPath maps
    for (const auto &pair : adj)
    {
        visited[pair.first] = false;
        onPath[pair.first] = false;
    }

    // Visit all nodes
    for (const auto &pair : adj)
    {
        if (!visited[pair.first])
        {
            if (!dfs(pair.first))
            {
                cerr << "Topological sort failed due to circular dependency.\n";
                exit(1);
            }
        }
    }

    // Collect the result
    vector<string> order;
    while (!topoStack.empty())
    {
        order.push_back(topoStack.top());
        topoStack.pop();
    }
    return order;
}

// Replace cell names in the expression with their actual values
string substituteVariables(const string &expr, const map<string, int> &values)
{
    string result = expr;
    regex cellRegex("([A-Z][1-9][0-9]*)");
    smatch match;
    string output;
    size_t pos = 0;

    auto begin = sregex_iterator(expr.begin(), expr.end(), cellRegex);
    auto end = sregex_iterator();

    size_t last_pos = 0;
    for (auto it = begin; it != end; ++it)
    {
        string var = it->str();
        int val = values.at(var); // Replace with actual value
        output += expr.substr(last_pos, it->position() - last_pos);
        output += to_string(val);
        last_pos = it->position() + var.length();
    }
    output += expr.substr(last_pos);
    return output;
}

// Evaluate a basic arithmetic expression (support +, -, *, /)
int evaluateSimpleExpression(const string &expr)
{
    istringstream ss(expr);
    int result;
    ss >> result;
    char op;
    int num;
    while (ss >> op >> num)
    {
        if (op == '+')
            result += num;
        else if (op == '-')
            result -= num;
        else if (op == '*')
            result *= num;
        else if (op == '/')
            result /= num;
    }
    return result;
}

int main()
{
    string filename;
    cout << "Please enter the input file name (for example, input.txt): ";
    cin >> filename;

    readInput(filename);                      // Step 1: Read table
    buildGraph();                             // Step 2: Build dependency graph
    vector<string> order = topologicalSort(); // Step 3: Topological sort

    cout << "Topological Order:\n";
    for (const string &cell : order)
    {
        cout << cell << " ";
    }
    cout << "\n\nEvaluated Results:\n";

    // Step 4: Evaluate expressions in topological order
    for (const string &cell : order)
    {
        string expr = expressions[cell];
        string substituted = expr;

        // Replace cell references with values
        if (!extractDependencies(expr).empty())
        {
            substituted = substituteVariables(expr, values);
        }

        // Evaluate the final expression
        int val = evaluateSimpleExpression(substituted);
        values[cell] = val;

        // Display full evaluation trace
        cout << cell << " = " << expr;
        if (expr != substituted)
        {
            cout << " = " << substituted;
        }
        cout << " = " << val << endl;
    }

    // Step 5: Output final values sorted by cell name
    cout << "\nFinal Values (sorted by cell name):\n";
    vector<pair<string, int>> sortedValues(values.begin(), values.end());
    sort(sortedValues.begin(), sortedValues.end());
    for (const auto &[cell, val] : sortedValues)
    {
        cout << cell << " = " << val << endl;
    }

    return 0;
}
