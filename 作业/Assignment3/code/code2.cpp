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

int rows, cols;
vector<vector<string>> table;

map<string, vector<string>> adj; // adjacency list: from -> to
map<string, bool> visited;       // for DFS
map<string, bool> onPath;        // to detect cycles
stack<string> topoStack;

map<string, string> expressions; // cell -> original expression
map<string, int> values;         // cell -> computed value

// 获取单元格名称，例如 A1、B2
string getCellName(int row, int col)
{
    char colChar = 'A' + col;
    return string(1, colChar) + to_string(row + 1);
}

// 读取输入文件
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
    getline(file, line); // skip rest of the line

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

// 提取表达式中的单元格依赖，如 A1、C3
vector<string> extractDependencies(const string &expr)
{
    vector<string> deps;
    regex cellRegex("([A-Z][1-9][0-9]*)");
    auto begin = sregex_iterator(expr.begin(), expr.end(), cellRegex);
    auto end = sregex_iterator();
    for (auto it = begin; it != end; ++it)
    {
        deps.push_back(it->str());
    }
    return deps;
}

// 构建依赖图
void buildGraph()
{
    for (const auto &[cell, expr] : expressions)
    {
        vector<string> deps = extractDependencies(expr);
        for (const string &dep : deps)
        {
            adj[dep].push_back(cell); // dep -> cell
        }
        if (adj.find(cell) == adj.end())
        {
            adj[cell] = {}; // ensure all nodes in the graph
        }
    }
}

// DFS + 环检测 + 拓扑排序
void dfs(const string &node)
{
    visited[node] = true;
    onPath[node] = true;

    for (const string &neighbor : adj[node])
    {
        if (!visited[neighbor])
        {
            dfs(neighbor);
        }
        else if (onPath[neighbor])
        {
            cerr << "Error: Circular dependency detected involving " << neighbor << "!\n";
            exit(1);
        }
    }

    onPath[node] = false;
    topoStack.push(node);
}

// 拓扑排序封装
vector<string> topologicalSort()
{
    for (const auto &p : adj)
    {
        visited[p.first] = false;
        onPath[p.first] = false;
    }

    for (const auto &p : adj)
    {
        if (!visited[p.first])
        {
            dfs(p.first);
        }
    }

    vector<string> order;
    while (!topoStack.empty())
    {
        order.push_back(topoStack.top());
        topoStack.pop();
    }
    return order;
}

// 替换表达式中的变量为数值
string substituteVariables(const string &expr, const map<string, int> &values)
{
    string result = expr;
    regex cellRegex("([A-Z][1-9][0-9]*)");
    smatch match;
    string output;
    size_t last_pos = 0;

    auto begin = sregex_iterator(expr.begin(), expr.end(), cellRegex);
    auto end = sregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        string var = it->str();
        int val;
        if (values.find(var) == values.end())
        {
            cerr << "Error: Variable " << var << " used before definition.\n";
            exit(1);
        }
        output += expr.substr(last_pos, it->position() - last_pos);
        output += to_string(values.at(var));
        last_pos = it->position() + var.length();
    }
    output += expr.substr(last_pos);
    return output;
}

// 简单整数四则运算求值器
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
    cout << "请输入输入文件名（例如 input.txt）：";
    cin >> filename;

    readInput(filename);
    buildGraph();
    vector<string> order = topologicalSort();

    cout << "\nTopological Order:\n";
    for (const string &cell : order)
    {
        cout << cell << " ";
    }
    cout << "\n\nEvaluated Results:\n";

    for (const string &cell : order)
    {
        string expr = expressions[cell];
        string substituted = expr;

        if (!extractDependencies(expr).empty())
        {
            substituted = substituteVariables(expr, values);
        }

        int val = evaluateSimpleExpression(substituted);
        values[cell] = val;

        cout << cell << " = " << expr;
        if (expr != substituted)
        {
            cout << " = " << substituted;
        }
        cout << " = " << val << endl;
    }

    cout << "\nFinal Values (sorted by cell name):\n";
    vector<pair<string, int>> sortedValues(values.begin(), values.end());
    sort(sortedValues.begin(), sortedValues.end());
    for (const auto &[cell, val] : sortedValues)
    {
        cout << cell << " = " << val << endl;
    }

    return 0;
}
