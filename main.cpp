#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cctype>
#include "BTree.h"
#include "hashTable.h"

using namespace std;

string Capitalize(string input)
{
    string output;
    for (int ii = 0; ii < input.size(); ii++)
    {
        output = output + " ";
        output[ii] = toupper(input[ii]);
    }
    return output;
}

string CodifyInput(string input, bool& flag)
{
    int numOfPieces = 0;
    int kingCount = 0;
    int queenCount = 0;
    int rookCount = 0;
    int bishopCount = 0;
    int knightCount = 0;
    int expectedSize = 0;

    vector<string> pieces = {"k", "q", "r", "b", "n", "p"};
    string codified;
    string output = "";

    if (input.length() < 42)
    {
        for (int ii = 0; ii < input.length(); ii++)
        {
            if (input[ii] == 'k' || input[ii] == 'K')
            {
                if (input.substr(ii,4).compare("king") == 0 || input.substr(ii,4).compare("King") == 0)
                {
                    numOfPieces++;
                    codified = codified + "k";
                    kingCount++;
                    expectedSize += 4;
                }
                else if (input.substr(ii,6).compare("knight") == 0 || input.substr(ii,6).compare("Knight") == 0)
                {
                    numOfPieces++;
                    codified = codified + "n";
                    knightCount++;
                    expectedSize += 6;
                }
            }
            else if (input[ii] == 'q' || input[ii] == 'Q')
            {
                if (input.substr(ii,5).compare("queen") == 0 || input.substr(ii,5).compare("Queen") == 0)
                {
                    numOfPieces++;
                    codified = codified + "q";
                    queenCount++;
                    expectedSize += 5;
                }
            }
            else if (input[ii] == 'r' || input[ii] == 'R')
            {
                if (input.substr(ii,4).compare("rook") == 0 || input.substr(ii,4).compare("Rook") == 0)
                {
                    numOfPieces++;
                    codified = codified + "r";
                    rookCount++;
                    expectedSize += 4;
                }
            }
            else if (input[ii] == 'b' || input[ii] == 'B')
            {
                if (input.substr(ii,6).compare("bishop") == 0 || input.substr(ii,6).compare("Bishop") == 0)
                {
                    numOfPieces++;
                    codified = codified + "b";
                    bishopCount++;
                    expectedSize += 6;
                }
            }
            else if (input[ii] == 'p' || input[ii] == 'P')
            {
                if (input.substr(ii,4).compare("pawn") == 0)
                {
                    numOfPieces++;
                    codified = codified + "p";
                    expectedSize += 4;
                }
            }
        }

        expectedSize += numOfPieces - 1;
            // check for standard piece counts, no need to keep track of pawns because there are too many of them!
            if ((numOfPieces > 6 || numOfPieces < 0) || expectedSize != input.size() || kingCount != 1 || queenCount > 1 || rookCount > 2 || bishopCount > 2 || knightCount > 2)
            {
                flag = false;
                return "";
            }

        for (int ii = 0; ii < pieces.size(); ii++)
        {
            for (int jj = 0; jj < codified.length(); jj++)
            {
                if (pieces[ii][0] == codified[jj])
                {
                    output = output + pieces[ii];
                }
            }
        }
    }
    else
    {
        flag = false;
        output = " ";
    }

    return output;
}

int main()
{
    bool running;
    bool playerTurn;
    string blackPieces;
    string whitePieces;
    string whiteCodified;
    string blackCodified;
    string fileName;
    string extension = "";

    // Introduction & instructions for white pieces text block
    cout << "******************************************" << endl;
    cout << "Welcome to the Chess Endgame Database!" << endl;
    cout << "******************************************" << endl;
    cout << "Developed by Matthew Hamilton, Aaron" << endl;
    cout << "Zamora, and Ethan Cecil" << endl;
    cout << "******************************************" << endl;
    cout << "This database holds all possible endgame" << endl;
    cout << "positions for 3-7 piece endgame situations" << endl;
    cout << "using the syzygy table base found at this" << endl;
    cout << "link: http://tablebase.sesse.net.syzygy/" << endl;
    cout << "******************************************" << endl;

    bool sentinel = true;
    while (sentinel)
    {
        bool whitePieceSentinel = true;
        while (whitePieceSentinel)
        {
            cout << endl << "Please enter White's pieces followed by a" << endl;
            cout << "space (EX. pawn king) keep in mind that" << endl;
            cout << "each side needs a king piece and the" << endl;
            cout << "number of total pieces on the board can" << endl;
            cout << "not exceed 7 pieces and can not be lower" << endl;
            cout << "than 3 pieces. Standard chess piece" << endl;
            cout << "counts also apply" << endl << endl;

            // take user input
            getline(cin, whitePieces);

            // trim input
            if (whitePieces[whitePieces.size()-1] == ' ')
            {
                whitePieces = whitePieces.substr(0, whitePieces.size()-1);
            }

            // translate to file name code
            whiteCodified = CodifyInput(whitePieces, whitePieceSentinel);
            if (whitePieceSentinel == false)
            {
                cout << endl << "Invalid input" << endl;
                whitePieceSentinel = true;
            }
            else
            {
                //cout << whiteCodified << endl;
                whitePieceSentinel = false;
            }
        }

        bool blackPieceSentinel = true;
        while (blackPieceSentinel)
        {
            // instructions for black pieces text block
            cout << endl << "Please enter Black's pieces followed by a" << endl;
            cout << "space (EX. pawn king) keep in mind that" << endl;
            cout << "each side needs a king piece and the" << endl;
            cout << "not exceed 7 pieces and can not be lower" << endl;
            cout << "than 3 pieces. Standard chess piece" << endl;
            cout << "counts also apply" << endl << endl;

            // take user input
            getline(cin, blackPieces);

            // trim input
            if (blackPieces[blackPieces.size()-1] == ' ')
            {
                blackPieces = blackPieces.substr(0, blackPieces.size()-1);
            }

            // translate to file name code
            blackCodified = CodifyInput(blackPieces, blackPieceSentinel);
            if (blackPieceSentinel == false)
            {
                cout << endl << "Invalid input" << endl;
                blackPieceSentinel = true;
            }
            else
            {
                //cout << blackCodified << endl;
                blackPieceSentinel = false;
            }

            if ((whiteCodified.size() + blackCodified.size()) == 7)
            {
                extension = ".txt";
                whiteCodified = Capitalize(whiteCodified);
                blackCodified = Capitalize(blackCodified);
                fileName = whiteCodified + "v" + blackCodified;
            }
            else
            {
                extension = ".tbs";
                fileName = whiteCodified + blackCodified;
            }
            
            //cout << fileName << endl;

            if (fileName.size() > 8)
            {
                cout << endl << "Too many pieces on the board." << endl;
            }
            else if (fileName.size() < 3)
            {
                cout << endl << "Too few pieces on the board." << endl;
            }
            else
            {
                sentinel = false;
            }
        }

        // use data structures
        cout << endl << "BST Search:" << endl;
        bool ifFound;
        
        string fileNameDir = "data/";
        fileNameDir = fileNameDir + fileName + extension;
        BTree* tree = new BTree();
	    tree->LoadAll();
        tree->Search(fileNameDir, tree->GetRoot());
        tree->DeleteTree(tree->GetRoot());
        cout << endl << endl;
    
        cout << "******************************************" << endl;
        cout << "Hash Search:" << endl;
        vector<pair<int, string>> table = loadTable();
        searchTable(table, fileName);
    }

    return 0;
}
