#include <iostream>
#include <string>
#include <limits>
#include "Compressor.hpp"

using namespace std;

void printMainMenu()
{
    cout << "==================================================\n";
    cout << "             HUFFMAN FILE COMPRESSOR\n";
    cout << "==================================================\n\n";
    cout << "1. Compress File\n";
    cout << "2. Decompress File\n";
    cout << "3. Exit\n\n";
    cout << "Enter your choice: ";
}

void handleCompress(Compressor &compressor)
{
    cout << "\n--------------------------------------------------\n";
    cout << "                COMPRESS FILE\n";
    cout << "--------------------------------------------------\n\n";

    string inputPath, outputPath;

    cout << "Enter input file path:\n> ";
    getline(cin, inputPath);

    cout << "\nEnter output file path:\n> ";
    getline(cin, outputPath);

    cout << "\n";
    compressor.compressFile(inputPath, outputPath);
    cout << "\n";
}

void handleDecompress(Compressor &compressor)
{
    cout << "\n--------------------------------------------------\n";
    cout << "               DECOMPRESS FILE\n";
    cout << "--------------------------------------------------\n\n";

    string inputPath, outputPath;

    cout << "Enter compressed file path:\n> ";
    getline(cin, inputPath);

    cout << "\nEnter output file path:\n> ";
    getline(cin, outputPath);

    cout << "\n";
    compressor.decompressFile(inputPath, outputPath);
    cout << "\n";
}

int main()
{
    Compressor compressor;
    bool running = true;

    while (running)
    {
        printMainMenu();

        string line;
        getline(cin, line);

        // Basic, beginner-friendly way to parse the menu choice safely.
        int choice = -1;
        try
        {
            choice = stoi(line);
        }
        catch (...)
        {
            choice = -1; // anything non-numeric becomes an invalid choice
        }

        cout << "\n";

        switch (choice)
        {
        case 1:
            handleCompress(compressor);
            break;
        case 2:
            handleDecompress(compressor);
            break;
        case 3:
            cout << "Goodbye!\n";
            running = false;
            break;
        default:
            cout << "Invalid choice. Please enter 1, 2, or 3.\n\n";
            break;
        }
    }

    return 0;
}
