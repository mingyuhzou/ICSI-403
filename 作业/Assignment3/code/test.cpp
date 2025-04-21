map<string, string> expressions;
map<string, vector<string>> adj;
map<string, bool> visited;
stack<string> topoStack;
map<string, int> values;
vector<vector<string>> table;

string getCellName(int row, int col)
{
    char colChar = 'A' + col;
    return string(1, colChar) + to_string(row + 1);
}

void readInput(const string &filename)
{
    ifstream infile(filename);
    int rows, cols;
    infile >> rows >> cols;
    table.resize(rows, vector<string>(cols));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            infile >> table[i][j];
            string cellName = getCellName(i, j);
            expressions[cellName] = table[i][j];
        }
    }
    infile.close();
}

vector<string> extractDependencies(const string &expr)
{
    regex cellRegex("[A-Z][0-9]+");
    sregex_iterator iter(expr.begin(), expr.end(), cellRegex);
    sregex_iterator end;
    vector<string> dependencies;
    while (iter != end)
    {
        dependencies.push_back(iter->str());
        ++iter;
    }
    return dependencies;
}

void buildGraph()
{
    for (auto &[cell, expr] : expressions)
    {
        vector<string> deps = extractDependencies(expr);
        for (const string &dep : deps)
        {
            adj[dep].push_back(cell);
        }
    }
}

void dfs(const string &node)
{
    visited[node] = true;
    for (const string &neighbor : adj[node])
    {
        if (!visited[neighbor])
        {
            dfs(neighbor);
        }
    }
    topoStack.push(node);
}

vector<string> topologicalSort()
{
    for (auto &[cell, _] : expressions)
    {
        if (!visited[cell])
        {
            dfs(cell);
        }
    }

    vector<string> sortedOrder;
    while (!topoStack.empty())
    {
        sortedOrder.push_back(topoStack.top());
        topoStack.pop();
    }
    return sortedOrder;
}

string substituteVariables(const string &expr, const map<string, int> &values)
{
    string result = expr;
    for (const auto &[var, val] : values)
    {
        size_t pos;
        while ((pos = result.find(var)) != string::npos)
        {
            result.replace(pos, var.length(), to_string(val));
        }
    }
    return result;
}

int evaluateSimpleExpression(const string &expr)
{
    istringstream iss(expr);
    int result, num;
    char op;
    iss >> result;
    while (iss >> op >> num)
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
    cout << "Enter the filename: ";
    cin >> filename;

    readInput(filename);                      // Step 1: Read the spreadsheet
    buildGraph();                             // Step 2: Build dependency graph
    vector<string> order = topologicalSort(); // Step 3: Determine evaluation order

    // Step 4: Evaluate each cell
    for (const string &cell : order)
    {
        string originalExpr = expressions[cell];
        string replacedExpr = substituteVariables(originalExpr, values);
        int value = evaluateSimpleExpression(replacedExpr);
        values[cell] = value;

        cout << cell << " = " << originalExpr << " => " << replacedExpr
             << " = " << value << endl;
    }

    // Optional: Print final table
    cout << "\nFinal Table:\n";
    for (int i = 0; i < table.size(); ++i)
    {
        for (int j = 0; j < table[0].size(); ++j)
        {
            string cellName = getCellName(i, j);
            cout << values[cellName] << " ";
        }
        cout << endl;
    }

    return 0;
}
