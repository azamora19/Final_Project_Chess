#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <experimental/filesystem>


using namespace std;
struct BTRNode;
class BTree;




struct BTRNode {
	//defining node charactaristics
	vector<pair<int, vector<string>>> values;
	int maxValues = 15;
	int currentSize;
	bool isLeaf;
	int lowestIndex;
	int highestIndex;

	vector<BTRNode*> children;


	BTRNode(bool leaf) {
		currentSize = 0;
		isLeaf = leaf;
		lowestIndex = 0;
		highestIndex = 0;

	}

	BTRNode* Split(BTRNode* toSplit) {
		BTRNode* toReturn = this;
		BTRNode* newChild = new BTRNode(toSplit->isLeaf);
		children.push_back(newChild);
		sort(toSplit->values.begin(), toSplit->values.end());
		int beginningSize = toSplit->values.size();
		for (int i = 0; i < beginningSize / 2; i++) {
			//copies bottom half of the values to the new child
			newChild->values.push_back(toSplit->values.at(0));
			toSplit->values.erase(toSplit->values.begin());
		}
		if (!toSplit->isLeaf) {
			//if the node isn't a leaf, it has to give the lower half of its children to the new child
			vector<pair<int, int>> minIndex;
			for (int i = 0; i < children.size(); i++) {
				pair<int, int> thisPair;
				thisPair.first = children.at(i)->lowestIndex;
				thisPair.second = i;
			}
			sort(minIndex.begin(), minIndex.end());
			for (int i = 0; i < minIndex.size() / 2; i++) {
				int currentNode = minIndex.at(i).second;
				newChild->children.push_back(children.at(currentNode));
			}
			for (int i = 0; i < minIndex.size() / 2; i++) {
				int currentNode = minIndex.at(i).second;
				children.erase(children.begin() + currentNode);
			}
		}
		//pushes the middle element up- the lower half has already been erased above, so the first element is the middle
		int index = toSplit->values.at(0).first;
		pair<int, vector<string>> fileContents = toSplit->values.at(0);
		if (currentSize == maxValues - 1) {
			//if the parent tree is full, this splits it and creates another level
			BTRNode* parent = new BTRNode(false);
			toReturn = parent;
			parent->children.push_back(this);
			parent->Split(this);
			int currentMin = INT_MAX;
			int currentMax = 0;
			int minIndex;
			int maxIndex;
			for (int i = 0; i < parent->children.size(); i++) {
				BTRNode* currentNode = parent->children.at(i);
				if (currentNode->lowestIndex < currentMin) {
					currentMin = currentNode->lowestIndex;
					minIndex = i;
				}
				if (currentNode->highestIndex > currentMax) {
					currentMax = currentNode->highestIndex;
					maxIndex = i;
				}
				
				if (index >= currentNode->lowestIndex && index <= currentNode->highestIndex) {
					currentNode->values.push_back(fileContents);
				}
				
			}
		if (index <= currentMin) {
			parent->children.at(minIndex)->values.push_back(fileContents);
			sort(parent->children.at(minIndex)->values.begin(), parent->children.at(minIndex)->values.end());
		}
		if (index >= currentMax) {
			parent->children.at(maxIndex)->values.push_back(fileContents);
			sort(parent->children.at(maxIndex)->values.begin(), parent->children.at(maxIndex)->values.end());
		}

		}
		else {
			values.push_back(fileContents);
			sort(values.begin(), values.end());
		}
		toSplit->values.erase(toSplit->values.begin());
		


		//updates sizes
		sort(toSplit->values.begin(), toSplit->values.end());
		if (toSplit->values.at(0).first < toSplit->lowestIndex)
			toSplit->lowestIndex = toSplit->values.at(0).first;
		if (toSplit->values.at(toSplit->values.size() - 1).first > toSplit->highestIndex)
			toSplit->highestIndex = toSplit->values.at(toSplit->values.size() - 1).first;
		toSplit->currentSize = toSplit->values.size();
		newChild->currentSize = newChild->values.size();
		currentSize = values.size();
		return toReturn;
	}




};


class BTree {
	//initializing B Tree properties
	//needs to be below BTRNode because it uses it (took me too long to figure that one out)
	int n;
	BTRNode* root;
public:
	BTree() {
		n = 10;
		root = nullptr;
	}
	BTRNode* GetRoot() {
		return root;
	}
	void AddValue(string fileName) {
		string name;
		ifstream file;
		file.open(fileName, fstream::in);
		pair<int, string> indexName;
		indexName = MakeIndex(fileName);
		int index = indexName.first;
		name = indexName.second;
		string buffer;
		string fileContent;
		vector<string> current;
		while (getline(file, buffer)) {
			fileContent += buffer;
			fileContent += "\n";
		}
		current.push_back(name);
		current.push_back(fileContent); 
		pair<int, vector<string>> toAdd;
		toAdd.first = index;
		toAdd.second = current;
		
		if (!root) {
			//if this is the first value being added
			root = new BTRNode(true);
			root->values.push_back(toAdd);
			sort(root->values.begin(), root->values.end());
			root->currentSize++;
			root->lowestIndex = root->values.at(0).first;
			root->highestIndex = root->values.at(0).first;
			
		}
		else {
			if (root && root->currentSize == root->maxValues - 1) {
				//if tree is full, this splits tree and adds tree above
				BTRNode* parent = new BTRNode(false);
				parent->children.push_back(root);
				parent = parent->Split(root);
				BTRNode* childToAdd;
				if (index > parent->children.at(0)->lowestIndex && index < parent->children.at(0)->highestIndex) {
					childToAdd = parent->children.at(0);
				}
				else {
					childToAdd = parent->children.at(1);
				}
				while (!childToAdd->isLeaf) {
					for (int i = 0; i < childToAdd->children.size(); i++) {
						BTRNode* currentNode = childToAdd->children.at(i);
						if (index >= currentNode->lowestIndex && index <= currentNode->highestIndex) {
							childToAdd = currentNode;
						}
						else if (i == 0 && index <= currentNode->lowestIndex) {
							childToAdd = currentNode;
						}
						else if (i == childToAdd->children.size() - 1 && index >= currentNode->highestIndex) {
							childToAdd = currentNode;
						}
					}
				}
				if (childToAdd->isLeaf) {
					childToAdd->values.push_back(toAdd);
					sort(childToAdd->values.begin(), childToAdd->values.end());
					childToAdd->currentSize++;
					if (childToAdd->values.at(0).first < childToAdd->lowestIndex)
						childToAdd->lowestIndex = childToAdd->values.at(0).first;
					if (childToAdd->values.at(childToAdd->values.size() - 1).first > childToAdd->highestIndex)
						childToAdd->highestIndex = childToAdd->values.at(childToAdd->values.size() - 1).first;
				}
				root = parent;
			}
			else {
				BTRNode* childToAdd = root;
				BTRNode* currentRoot = root;
				for (int i = 0; i < currentRoot->children.size(); i++) {
					if (index > currentRoot->children.at(i)->lowestIndex && index < currentRoot->children.at(i)->highestIndex) {
						childToAdd = root->children.at(i);
					}
				}
					while (childToAdd && !childToAdd->isLeaf) {
						for (int i = 0; i < childToAdd->children.size(); i++) {
							BTRNode* currentNode = childToAdd->children.at(i);
							if (index >= currentNode->lowestIndex && index <= currentNode->highestIndex) {
								childToAdd = currentNode;
							}
							else if (i == 0 && index <= currentNode->lowestIndex) {
								childToAdd = currentNode;
							}
							else if (i == childToAdd->children.size() - 1 && index >= currentNode->highestIndex) {
								childToAdd = currentNode;
							}
						}
					}
					if (childToAdd->isLeaf) {
						childToAdd->values.push_back(toAdd);
						sort(childToAdd->values.begin(), childToAdd->values.end());
						childToAdd->currentSize = childToAdd->values.size();
						if (childToAdd->values.at(0).first < childToAdd->lowestIndex)
							childToAdd->lowestIndex = childToAdd->values.at(0).first;
						if (childToAdd->values.at(childToAdd->values.size() - 1).first > childToAdd->highestIndex)
							childToAdd->highestIndex = childToAdd->values.at(childToAdd->values.size() - 1).first;
					}
				}
			
		}
	}
	void LoadAll() {
		string currentDir = "data";
		vector<string> fileNames;
		//There are too many files to manually load them all, so I researched directory iterators to easily find their names
		for (auto& itr : experimental::filesystem::directory_iterator(currentDir)) {
			string name = itr.path().string();
			if (name.find(".") != string::npos) {
				replace(name.begin(), name.end(), '\\', '/');
				fileNames.push_back(name);
			}
		}
		for (int i = 0; i < fileNames.size(); i++) {
			AddValue(fileNames.at(i));
			//cout << "Added " << i + 1 << endl;
			//PrintAll(root);
			//cout << endl;
		}

	}
	pair<int, string> MakeIndex(string fileName) {
		int index = 0;
		string name;
		if (fileName.length() < 16) {
			if (fileName.find("k") != string::npos && fileName.find(".") != string::npos)
				name = fileName.substr(fileName.find("k"), fileName.find(".") - fileName.find("k"));
			for (int i = 0; i < name.length(); i++) {
				//calculates index by summing positions in the alphabet
				index += name[i] - 'a';
			}
		}
		else {
			//write stuff for syzgy file format
			if (fileName.find("K") != string::npos && fileName.find(".") != string::npos)
				name = fileName.substr(fileName.find("K"), fileName.find(".") - fileName.find("K"));
			for (int i = 0; i < name.length(); i++) {
				//calculates index by summing positions in the alphabet
				index += name[i] - 'A';
			}
		}
		pair<int, string> result;
		result.first = index;
		result.second = name;
		return result;
	}
	void PrintAll(BTRNode* currentRoot) {
		if (!currentRoot) {
			return;
		}
		for (int i = 0; i < currentRoot->values.size(); i++) {
			cout << currentRoot->values.at(i).second.at(0) << " " << currentRoot->values.at(i).first << endl;
		}
		cout << endl;
		
		for (int i = 0; i < currentRoot->children.size(); i++) {
			PrintAll(currentRoot->children.at(i));
		}
	}

	void DeleteTree(BTRNode* currentRoot) {
		for (int i = 0; i < currentRoot->children.size(); i++) {
			//cout << "Deleted " << currentRoot->values.size() << " values" << endl;
			DeleteTree(currentRoot->children.at(i));
		}
		if (currentRoot->isLeaf) {
			delete currentRoot;
		}
		
	}
	bool Search(string fileName, BTRNode* currentRoot) {
		pair<int, string> fileInfo = MakeIndex(fileName);
		int index = fileInfo.first;
		string name = fileInfo.second;
		bool found = false;
		
			for (int i = 0; i < currentRoot->values.size(); i++) {
				if (currentRoot->values.at(i).first == index && currentRoot->values.at(i).second.at(0) == name) {
					cout << currentRoot->values.at(i).second.at(1);
					found = true;
					return true;
				}
			}
			if (currentRoot->children.size() == 0) {
				return found;
			}
			
		
		for (int i = 0; i < currentRoot->children.size(); i++) {
			BTRNode* thisChild = currentRoot->children.at(i);
			if (index >= thisChild->lowestIndex && index <= thisChild->highestIndex) {
				if (i == currentRoot->children.size() - 1) {
					found = Search(fileName, thisChild);
					
				}
			}
		}
		if (found) {
			return true;
		}
		else
			cout << "Not Found" << endl;

	}
};

