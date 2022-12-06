#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <iterator>
//#include <experimental/filesystem>
#include <filesystem>;
using namespace std;

//This function is used to create a pair that can be put into the hash table.
pair<int, string> createItem(int key, string file)
{
	pair<int, string> entry = {};
	entry.first = key;
	entry.second = file;

	return entry;
}

//This reads the 'data' folder where our chess tablebases are in. This function iterates through the directory and gets all of their raw files names, ex "data/KBBBBBvK.txt"
//However this function later gets rid of the 'data' in the front of the string. This is done so the board and its pieces and be iterated through to get the ASCII values to use in the hash function.
vector<string> loadFiles()
{
	string directory = "data";
	vector<string> fileNames;

	for (auto& iter : experimental::filesystem::directory_iterator(directory))
	{
		string name = iter.path().string();
		if (name.find('.') != string::npos)
		{
			replace(name.begin(), name.end(), '\\', '/');
			fileNames.push_back(name);
		}
	}

	for (int i = 0; i < fileNames.size(); i++)
	{
		string temp = fileNames.at(i);
		temp = temp.substr(5);
		fileNames.at(i) = temp;
	}

	return fileNames;
}

//This function essentially functions the same as the loadFiles() function, however, this leaves in the extra text that we got rid of in the loadFiles() function.
//This function leaves in that text because when the files are put into the hash table, we need the extra text so it can easily be read by a simple fstream.
vector<string> rawFileNames()
{
	string directory = "data";
	vector<string> fileNames;

	for (auto& iter : experimental::filesystem::directory_iterator(directory))
	{
		string name = iter.path().string();
		if (name.find('.') != string::npos)
		{
			replace(name.begin(), name.end(), '\\', '/');
			fileNames.push_back(name);
		}
	}

	return fileNames;
}


//This is the hash function which finds the spot to put the files for the appropriate board for.
//This takes in the modified file with the /data taken off the front
//Then it takes off the file extension at the end.
//it them iterates through the board (KBBBBNvK etc) and generates a hash value using the ASCII values
//of the characters and their position in the string.
//This also uses a probing method similar to quadratic probing
//but instead multiplies a character's ASCII value by its position in the string
//therefore this hash function also serves as a collision handler.
vector<int> hash_function()
{

	vector<string> files = loadFiles();
	vector<string> modifiedFiles;

	vector<int> indices;
	for (string file : files)
	{
		file = file.substr(0, file.length() - 4);
		modifiedFiles.push_back(file);
	}
	int index = 0;

	for (auto file : modifiedFiles)
	{
		for (int i = 0; i < file.size(); i++)
		{
			if (i > 0)
			{
				char temp = file.at(i);
				index += (int)temp / i;
			}
			else
			{
				char temp = file.at(i);
				index += (int)temp;
			}

			if (file.at(i) == 'N')
			{
				char temp = file.at(i);
				index += (int)temp + ((int)'N' + ((int)'n' * i));
			}

			if (file.at(i) == 'K')
			{
				char temp = file.at(i);
				index += (int)temp + ((int)'K' + ((int)'k' * i));
			}

			if (file.at(i) == 'Q')
			{
				char temp = file.at(i);
				index += (int)temp + ((int)'Q' + ((int)'q' * i));
			}

			if (file.at(i) == 'R')
			{
				char temp = file.at(i);
				index += (int)temp + ((int)'R' + ((int)'r' * i));
			}

			if (file.at(i) == 'B')
			{
				char temp = file.at(i);
				index += (int)temp + ((int)'B' + ((int)'b' * i));
			}

			if (file.at(i) == 'v')
			{
				char temp = file.at(i);
				index += (int)temp + ((int)'v' + ((int)'V' * i));
			}
		}
		indices.push_back(index);
		index = 0;
	}
	return indices;
}

//this searches the hash table given an input in the form of the board
//ex. KQRBvRRK (King, Queen, Rook, Bishop vs Rook Rook King)
void searchTable(vector<pair<int, string>> table, string input)
{
	//integers to store hash value and file name.
	int index = 0;
	string temp;

	//gets the appropriate hash value to search the table, and makes appropriate shifts to prevent collisions.
	for (int i = 0; i < input.size(); i++)
	{
		if (i > 0)
		{
			char temp = input.at(i);
			index += (int)temp / i;
		}
		else
		{
			char temp = input.at(i);
			index += (int)temp;
		}

		if (input.at(i) == 'N')
		{
			char temp = input.at(i);
			index += (int)temp + ((int)'N' + ((int)'n' * i));
		}

		if (input.at(i) == 'K')
		{
			char temp = input.at(i);
			index += (int)temp + ((int)'K' + ((int)'k' * i));
		}

		if (input.at(i) == 'Q')
		{
			char temp = input.at(i);
			index += (int)temp + ((int)'Q' + ((int)'q' * i));
		}

		if (input.at(i) == 'R')
		{
			char temp = input.at(i);
			index += (int)temp + ((int)'R' + ((int)'r' * i));
		}

		if (input.at(i) == 'B')
		{
			char temp = input.at(i);
			index += (int)temp + ((int)'B' + ((int)'b' * i));
		}

		if (input.at(i) == 'v')
		{
			char temp = input.at(i);
			index += (int)temp + ((int)'v' + ((int)'V' * i));
		}
	}

	int position = 0;

	//Finds the appropriate node and gets the file name.
	for (auto node : table)
	{
		if (node.first == index)
		{
			temp = node.second;
		}
	}
	
	//Opens the file.
	fstream file;
	char ch;
	file.open(temp, ios::in);
	while (file >> noskipws >> ch)
	{
		cout << ch;
	}

}

//loads the hash table with the hash functions and the raw files names.
vector<pair<int, string>> loadTable()
{
	vector<int> hash_values = hash_function();
	vector<string> files = rawFileNames();
	vector<pair<int, string>> table;
	vector<pair<int, string>> nodes;

	for (int i = 0; i < hash_values.size(); i++)
	{
		pair<int, string> temp = createItem(hash_values.at(i), files.at(i));
		table.push_back(temp);
	}

	return table;
}

