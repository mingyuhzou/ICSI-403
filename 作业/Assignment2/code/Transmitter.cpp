#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// Huffman tree node structure
struct HuffmanNode
{
    char data;
    int freq;
    HuffmanNode *left, *right;
    HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

// Min-heap implementation (strictly following required interface names)
class MinHeap
{
    vector<HuffmanNode *> A;

    int Parent(int i) { return (i - 1) / 2; }
    int Left(int i) { return 2 * i + 1; }
    int Right(int i) { return 2 * i + 2; }

public:
    // Maintain heap property for subtree rooted at index i
    void Min_Heapify(vector<HuffmanNode *> &A, int i)
    {
        int l = Left(i);
        int r = Right(i);
        int smallest = i;

        if (l < A.size() && A[l]->freq < A[i]->freq)
            smallest = l;
        if (r < A.size() && A[r]->freq < A[smallest]->freq)
            smallest = r;
        if (smallest != i)
        {
            swap(A[i], A[smallest]);
            Min_Heapify(A, smallest);
        }
    }

    // Build min-heap from unordered array
    void Build_Min_Heap(vector<HuffmanNode *> &A, int n)
    {
        for (int i = n / 2 - 1; i >= 0; i--)
            Min_Heapify(A, i);
    }

    // Get minimum element without extraction
    HuffmanNode *Min_Heap_Minimum(vector<HuffmanNode *> &A)
    {
        return A.empty() ? nullptr : A[0];
    }

    // Extract and return minimum element
    HuffmanNode *Min_Heap_Extract_Min(vector<HuffmanNode *> &A)
    {
        if (A.empty())
            return nullptr;

        HuffmanNode *min = A[0];
        A[0] = A.back();
        A.pop_back();
        Min_Heapify(A, 0);

        return min;
    }

    // Decrease key value at index i
    void Min_Heap_Increase_Key(vector<HuffmanNode *> &A, int i, HuffmanNode *x)
    {
        if (x->freq < A[i]->freq)
        {
            A[i] = x;
            while (i > 0 && A[Parent(i)]->freq > A[i]->freq)
            {
                swap(A[i], A[Parent(i)]);
                i = Parent(i);
            }
        }
    }

    // Insert new element into heap
    void Min_Heap_Insert(vector<HuffmanNode *> &A, HuffmanNode *x, int &n)
    {
        A.push_back(new HuffmanNode('\0', INT_MAX));
        n = A.size();
        Min_Heap_Increase_Key(A, n - 1, x);
    }
};

// Generate Huffman codes from tree
void generateCodes(HuffmanNode *root, string code, map<char, string> &huffmanCodes)
{
    if (!root)
        return;
    if (!root->left && !root->right) // Leaf node
    {
        huffmanCodes[root->data] = code;
    }
    generateCodes(root->left, code + "0", huffmanCodes);
    generateCodes(root->right, code + "1", huffmanCodes);
}

// Write compressed data file with 16-character blocks
void writeDataFile(const string &inputFile, const string &dataFile, const map<char, string> &huffmanCodes)
{
    ifstream fin(inputFile, ios::binary);
    ofstream fout(dataFile, ios::binary);

    vector<char> buffer(16);
    while (fin)
    {
        fin.read(buffer.data(), 16);
        streamsize count = fin.gcount();

        if (count == 0)
            break;

        // Write block header: 2 SYN chars + length
        fout << char(22) << char(22) << char(count);

        // Write encoded data
        for (int i = 0; i < count; i++)
        {
            fout << huffmanCodes.at(buffer[i]);
        }
    }

    fin.close();
    fout.close();
}

// Write encoding file with alphabetical sorting
void writeEncodingFile(const string &encodingFile, map<char, string> &huffmanCodes)
{
    ofstream fout(encodingFile, ios::binary);

    // Sort codes alphabetically
    vector<pair<char, string>> sortedCodes(huffmanCodes.begin(), huffmanCodes.end());
    sort(sortedCodes.begin(), sortedCodes.end());

    // Write file header: 2 SOH chars + code count
    fout << char(1) << char(1) << char(sortedCodes.size());

    // Write encoding table
    for (const auto &pair : sortedCodes)
    {
        fout << pair.first << pair.second;
    }

    fout.close();
}

int main()
{
    string inputFile, dataFile, encodingFile;
    cout << "Enter input file path: ";
    cin >> inputFile;
    cout << "Enter data file output path: ";
    cin >> dataFile;
    cout << "Enter encoding file output path: ";
    cin >> encodingFile;

    // 1. Read file and calculate character frequencies
    ifstream fin(inputFile, ios::binary);
    map<char, int> freq;
    char ch;
    while (fin.get(ch))
        freq[ch]++;
    fin.close();

    // 2. Build Huffman tree using min-heap
    MinHeap minHeap;
    vector<HuffmanNode *> heap;
    int n = 0;

    // Insert all characters into min-heap
    for (auto pair : freq)
    {
        minHeap.Min_Heap_Insert(heap, new HuffmanNode(pair.first, pair.second), n);
    }
    minHeap.Build_Min_Heap(heap, n);

    // Build Huffman tree by combining nodes
    while (heap.size() > 1)
    {
        HuffmanNode *left = minHeap.Min_Heap_Extract_Min(heap);
        HuffmanNode *right = minHeap.Min_Heap_Extract_Min(heap);
        HuffmanNode *newNode = new HuffmanNode('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        minHeap.Min_Heap_Insert(heap, newNode, n);
    }

    HuffmanNode *root = minHeap.Min_Heap_Extract_Min(heap);

    // 3. Generate Huffman codes from tree
    map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // 4. Write output files
    writeDataFile(inputFile, dataFile, huffmanCodes);
    writeEncodingFile(encodingFile, huffmanCodes);

    // Print statistics
    cout << "\nCompression complete! Statistics:" << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(15) << "Character" << setw(15) << "Frequency" << "Code" << endl;
    cout << "----------------------------------------" << endl;

    // Sort frequencies alphabetically for display
    vector<pair<char, int>> sortedFreq(freq.begin(), freq.end());
    sort(sortedFreq.begin(), sortedFreq.end());

    for (const auto &pair : sortedFreq)
    {
        cout << setw(15) << pair.first
             << setw(15) << pair.second
             << huffmanCodes[pair.first] << endl;
    }

    cout << "----------------------------------------" << endl;
    cout << "Data file saved to: " << dataFile << endl;
    cout << "Encoding file saved to: " << encodingFile << endl;

    return 0;
}