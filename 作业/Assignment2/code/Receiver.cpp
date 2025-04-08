#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

// Huffman tree node structure
struct HuffmanNode
{
    char data;
    HuffmanNode *left, *right;
    HuffmanNode(char d) : data(d), left(nullptr), right(nullptr) {}
};

// Function to decode compressed file using Huffman coding
void decodeFile(const string &dataFile, const string &encodingFile, const string &outputFile)
{
    // 1. Read encoding file to get code-to-character mapping
    ifstream encIn(encodingFile, ios::binary);
    map<string, char> codeToChar;

    // Read file header (2 SOH characters + code count)
    char header[3];
    encIn.read(header, 3);
    if (header[0] != 1 || header[1] != 1)
    {
        cerr << "Invalid encoding file header" << endl;
        return;
    }

    // Read each character and its corresponding code
    int codeCount = static_cast<unsigned char>(header[2]);
    for (int i = 0; i < codeCount; i++)
    {
        char ch;
        encIn.get(ch);
        string code;
        char bit;
        while (encIn.get(bit) && (bit == '0' || bit == '1'))
        {
            code += bit;
        }
        encIn.unget();
        codeToChar[code] = ch;
    }
    encIn.close();

    // 2. Rebuild Huffman tree from codes
    HuffmanNode *root = new HuffmanNode('\0');
    for (const auto &pair : codeToChar)
    {
        HuffmanNode *current = root;
        for (char bit : pair.first)
        {
            if (bit == '0')
            {
                if (!current->left)
                    current->left = new HuffmanNode('\0');
                current = current->left;
            }
            else
            {
                if (!current->right)
                    current->right = new HuffmanNode('\0');
                current = current->right;
            }
        }
        current->data = pair.second;
    }

    // 3. Read and decode data file
    ifstream dataIn(dataFile, ios::binary);
    ofstream out(outputFile, ios::binary);

    // Process each data block (each starts with 2 SYN chars + length)
    vector<char> blockHeader(3);
    while (dataIn.read(blockHeader.data(), 3))
    {
        if (blockHeader[0] != 22 || blockHeader[1] != 22)
        {
            cerr << "Invalid data block header" << endl;
            break;
        }

        // Get number of characters in this block
        int length = static_cast<unsigned char>(blockHeader[2]);
        HuffmanNode *current = root;
        char bit;

        // Decode each character in the block
        for (int i = 0; i < length;)
        {
            dataIn.get(bit);
            if (bit == '0')
            {
                current = current->left;
            }
            else if (bit == '1')
            {
                current = current->right;
            }

            // When reaching a leaf node, write the character
            if (!current->left && !current->right)
            {
                out << current->data;
                current = root;
                i++;
            }
        }
    }

    dataIn.close();
    out.close();
}

int main()
{
    string dataFile, encodingFile, outputFile;
    cout << "Enter data file path: ";
    cin >> dataFile;
    cout << "Enter encoding file path: ";
    cin >> encodingFile;
    cout << "Enter output file path: ";
    cin >> outputFile;

    decodeFile(dataFile, encodingFile, outputFile);
    cout << "Decompression complete! Output file: " << outputFile << endl;

    return 0;
}