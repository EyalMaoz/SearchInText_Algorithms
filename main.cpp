// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>
#include "main.h"
#include <map>
#include <vector>
#include <chrono> 

using namespace std::chrono;

char dontCare = '-';


map<string, vector<int>>* FillMaps(vector<char> textVector, int** TavniotMatrix, int numberOfTavniot, int numberOfMatches)
{
	int matches = 0;
	int indexTavnit = 0;
	string str;
	map<string, vector<int>>* maps = new map< string, vector<int>>[numberOfTavniot];

	for (int tavnitNum = 0; tavnitNum < numberOfTavniot; tavnitNum++)
	{
		for (int textIndex = 0; textIndex < textVector.size(); textIndex++)
		{
			if (TavniotMatrix[tavnitNum][indexTavnit] == 1)
			{
				matches++;
				str += textVector.at(textIndex);
			}
			else str += dontCare;
			if (matches == numberOfMatches)
			{
				int startIndex = textIndex - indexTavnit;
				if (maps[tavnitNum].find(str) == maps[tavnitNum].end())
				{// first time for this pair
					maps[tavnitNum].insert(pair<string, vector<int>>(str, { startIndex }));
				}
				else
				{
					maps[tavnitNum].at(str).push_back(startIndex);
				}
				matches = 0;
				textIndex = startIndex;
				indexTavnit = -1;
				str = "";
			}
			indexTavnit++;
		}
		matches = 0;
		indexTavnit = 0;
		str = "";
	}
	return maps;
}

bool CheckAround(vector<char> wordVectror, vector<char> textVector, int indexInText, int checkBackward, int sizeOfWord, int minimumMatchs)
{
	int wordIndex = 0;
	int matches = 0;
	int start = indexInText - checkBackward;
	int end = start + sizeOfWord;

	if (start < 0)return false;
	if (end > textVector.size())return false;

	for (int i = start; i < end; i++)
	{
		if (textVector[i] == wordVectror[wordIndex])
		{
			matches++;
		}
		wordIndex++;
	}
	if (matches >= minimumMatchs) return true;
	return false;
}

map<string, vector<int>> NativeAlgorithm(vector<char> textVector, vector<char> wordVectror, int minimumMatchs)
{
	map<string, vector<int>> matchesWords;
	int matches = 0;

	for (int textIndex = 0; textIndex < textVector.size(); textIndex++)
	{
		for (int wordIndex = 0; wordIndex < wordVectror.size(); wordIndex++)
		{
			if (textIndex + wordIndex >= textVector.size())break;
			if (textVector[textIndex + wordIndex] == wordVectror[wordIndex]) matches++;
		}
		if (matches >= minimumMatchs)
		{
			string matchesWord = "";
			for (int i = 0; i < wordVectror.size(); i++)
			{
				matchesWord += textVector[textIndex + i];
			}
			if (matchesWords.find(matchesWord) == matchesWords.end())
			{// first time for this pair
				matchesWords.insert(pair<string, vector<int>>(matchesWord, { textIndex }));
			}
			else
			{
				if (std::find(matchesWords.at(matchesWord).begin(), matchesWords.at(matchesWord).end(), textIndex) == matchesWords.at(matchesWord).end())
				{
					matchesWords.at(matchesWord).push_back(textIndex);
				}
			}
		}
		matches = 0;
	}
	return matchesWords;
}

void PrintMatches(map<string, vector<int>> matches)
{
	if (matches.size() > 0)
	{
		cout << "Found the next similar words:" << endl;
		for (map<string, vector<int>>::iterator word = matches.begin(); word != matches.end(); ++word)
		{
			cout << word->first << endl;
			cout << "Indices: ";
			for (int i = 0; i < word->second.size(); i++)
			{
				cout << word->second[i];
				if (i != word->second.size() - 1)cout << " ,";

			}
			cout << endl;
		}
	}
	else
	{
		cout << "No words were found." << endl;
	}
}

int main()
{
	//// Generate text file with size 1M
	//int sizeOfFileInBytes = 1000 * 1024;// 1MB
	//ofstream inputFile("inputFile.txt");
	//if (!inputFile.is_open())
	//{
	//	cout << "Couldnt open file";
	//	return 1;
	//}
	//cout << "Generating file with size: " << sizeOfFileInBytes << endl;
	//for (int i = 0; i < sizeOfFileInBytes; i++)
	//{
	//	char randChar = (char)(rand() % 26 + 97);
	//	inputFile.write(&randChar, 1);
	//}
	//inputFile.close();

	// Create Tavniot
	int x = (6 + 7 + 5) / 3;
	int sizeS = 25 - x;
	int minimumMatches = ceil(sizeS * 0.75);
	Forms form;

	form.N_glob = sizeS; //size of word for search
	form.N_2_glob = minimumMatches; //minimal amount of matches
	form.Nsovp1_Glob = 4; //amount of matches in the filter

	form.create_mas1();
	form.chetv_struct_Generation();
	// Now we read the text and arrange the maps by tavniot:
	ifstream textFile("inputFile.txt");
	int numberOfTavniot = form.Nform1_Glob;
	int** TavniotMatrix = form.forms1Glob;
	int TavnitSize = form.Nsovp1_Glob;
	map<string, vector<int>>* maps;// = new map< string, vector<int>>[numberOfTavniot];
	char ch;

	vector<char> textVector;
	cout << "Reading file into Vector..." << endl;

	while (textFile.get(ch))// read text into vector
	{
		textVector.push_back(ch);
	}
	cout << "Basic Algorithm Pre-processing:" << endl;
	cout << "Creating maps for text" << endl;
	maps = FillMaps(textVector, TavniotMatrix, numberOfTavniot, TavnitSize);
	cout << "Finished" << endl;
	// End of pre-processing

	map<string, vector<int>>* wordMaps;
	map<string, vector<int>> matchesWords;
	cout << "Please enter word to search with size " << sizeS << endl;
	vector<char> wordVectror;
	for (int i = 0; i < sizeS; i++)
	{
		char ch;
		cin >> ch;
		wordVectror.push_back(ch);
	}
	cout << endl << "Starting Basic Algorithm search..." << endl;
	// Get starting timepoint 
	auto start = high_resolution_clock::now();
	wordMaps = FillMaps(wordVectror, TavniotMatrix, numberOfTavniot, TavnitSize);
	for (int mapIndex = 0; mapIndex < numberOfTavniot; mapIndex++)
	{
		for (map<string, vector<int>>::iterator subWord = wordMaps[mapIndex].begin(); subWord != wordMaps[mapIndex].end(); ++subWord)
		{
			string str = subWord->first;
			vector<int> wordIndices = subWord->second;
			if (maps[mapIndex].find(str) != maps[mapIndex].end())
			{// exists
				for (int indexInWord = 0; indexInWord < wordIndices.size(); indexInWord++)
				{
					int checkBackward = wordIndices[indexInWord];
					vector<int> textIncices = maps[mapIndex].at(str);
					for (int indexInText = 0; indexInText < textIncices.size(); indexInText++)
					{
						if (CheckAround(wordVectror, textVector, textIncices[indexInText], checkBackward, sizeS, minimumMatches))
						{
							string matchesWord = "";
							for (int i = 0; i < sizeS; i++)
							{
								matchesWord += textVector[textIncices[indexInText] - checkBackward + i];
							}
							if (matchesWords.find(matchesWord) == matchesWords.end())
							{// first time for this pair
								matchesWords.insert(pair<string, vector<int>>(matchesWord, { textIncices[indexInText] - checkBackward }));
							}
							else
							{
								if (std::find(matchesWords.at(matchesWord).begin(), matchesWords.at(matchesWord).end(), textIncices[indexInText] - checkBackward) == matchesWords.at(matchesWord).end())
								{
									matchesWords.at(matchesWord).push_back(textIncices[indexInText] - checkBackward);
								}
							}
						}
					}
				}
			}
		}
	}
	auto stop = high_resolution_clock::now();

	PrintMatches(matchesWords);
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Time taken by Basic Algorithm: " << duration.count() << " microseconds" << endl;

	cout << endl << "Starting Native Algorithm search..." << endl;
	start = high_resolution_clock::now();
	PrintMatches(NativeAlgorithm(textVector, wordVectror, minimumMatches));
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);

	cout << "Time taken by Native Algorithm: " << duration.count() << " microseconds" << endl;
	cout << "Finished all." << endl;

}


