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

map<string, vector<int>> NaiveAlgorithm(vector<char> textVector, vector<char> wordVectror, int minimumMatchs)
{
	map<string, vector<int>> matchesWords;
	int matches = 0;
	int missMatches = 0;

	for (int textIndex = 0; textIndex < textVector.size(); textIndex++)
	{
		for (int wordIndex = 0; wordIndex < wordVectror.size(); wordIndex++)
		{
			if (textIndex + wordIndex >= textVector.size())break;
			if (textVector[textIndex + wordIndex] != wordVectror[wordIndex])
			{
				missMatches++;
			}
			else matches++;

			if (missMatches > wordVectror.size() - minimumMatchs)
			{
				break;// not found
			}
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
		missMatches = 0;
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

void PrintMatchesToText(map<string, vector<int>> matches, vector<char> word, ofstream& outputFile)
{
	if (matches.size() > 0)
	{

		outputFile << endl << "For the word ";
		for (const auto& i : word)
			outputFile << i;
		outputFile << " found the next similar words:" << endl;
		for (map<string, vector<int>>::iterator word = matches.begin(); word != matches.end(); ++word)
		{
			outputFile << word->first << endl;
			outputFile << "	Indices: ";
			for (int i = 0; i < word->second.size(); i++)
			{
				outputFile << word->second[i];
				if (i != word->second.size() - 1)outputFile << " ,";

			}
			outputFile << endl;
		}
	}
}

int main()
{
	// Generate text file with size 1M
	int sizeOfFileInBytes = pow(2,20);// 1MB
	ofstream inputFile("OurFiles\\inputFile.txt");
	ofstream basicOutputFile("OurFiles\\Basic Algorithm Output.txt");
	ofstream naiveOutputFile("OurFiles\\Naive Algorithm Output.txt");
	ofstream OutputConsoleFile("OurFiles\\Algorithm Output.txt");
	if (!inputFile.is_open())
	{
		cout << "Couldnt open file";
		return 1;
	}
	if (!basicOutputFile.is_open())
	{
		cout << "Couldnt open file";
		return 1;
	}
	if (!naiveOutputFile.is_open())
	{
		cout << "Couldnt open file";
		return 1;
	}
	if (!OutputConsoleFile.is_open())
	{
		cout << "Couldnt open file";
		return 1;
	}
	cout << "Generating file with size: " << sizeOfFileInBytes <<" bytes"<< endl;
	OutputConsoleFile << "Generating file with size: " << sizeOfFileInBytes << endl;
	for (int i = 0; i < sizeOfFileInBytes; i++)
	{
		char randChar = (char)(rand() % 26 + 97);
		inputFile.write(&randChar, 1);
	}
	inputFile.close();

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
	ifstream textFile("OurFiles\\inputFile.txt");
	int numberOfTavniot = form.Nform1_Glob;
	int** TavniotMatrix = form.forms1Glob;
	int TavnitSize = form.Nsovp1_Glob;
	map<string, vector<int>>* maps;// = new map< string, vector<int>>[numberOfTavniot];
	char ch;

	vector<char> textVector;
	cout << "Reading file into Vector..." << endl;
	OutputConsoleFile << "Reading file into Vector..." << endl;

	while (textFile.get(ch))// read text into vector
	{
		textVector.push_back(ch);
	}
	cout << "Basic Algorithm Pre-processing:" << endl;
	OutputConsoleFile << "Basic Algorithm Pre-processing:" << endl;
	cout << "Creating maps for text" << endl;
	OutputConsoleFile << "Creating maps for text" << endl;
	maps = FillMaps(textVector, TavniotMatrix, numberOfTavniot, TavnitSize);
	cout << "Finished" << endl;
	OutputConsoleFile << "Finished" << endl;
	// End of pre-processing

	// Creating the word bag for search:

	map<string, vector<int>>* wordMaps;
	map<string, vector<int>> matchesWords;
	vector< vector<char>> allWords;

	//string testString = "fdxfircvscxggbwkfnq";
	//allWords.push_back(vector<char>(testString.begin(), testString.end()));


	for (int i = 0; i < textVector.size() / 2; i++)
	{
		vector<char> word;
		for (int j = 0; j < sizeS; j++)
		{
			word.push_back(textVector.at(i + j));
		}
		allWords.push_back(word);
	}


	// Now we will search each word and calculate how much it took to find it, eventually calculate the average time.
	time_point<steady_clock> start_time;
	time_point<steady_clock> stop_time;

	/*int start = 0;
	int end;
	int wordIndex;
	bool exist;
	int misMatches;*/
	cout << endl << "Starting Basic Algorithm search..." << endl;
	OutputConsoleFile << endl << "Starting Basic Algorithm search..." << endl;

	start_time = high_resolution_clock::now();
	for (auto& wordVectror : allWords)
	{
		wordMaps = FillMaps(wordVectror, TavniotMatrix, numberOfTavniot, TavnitSize);
		matchesWords.clear();
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
							/*CheckAround*/
							int start = textIncices[indexInText] - checkBackward;
							int end = start + sizeS;
							int wordIndex = 0;
							bool exist = true;

							if (start < 0)exist = false;
							if (end > textVector.size())exist = false;
							int misMatches = 0;
							if (exist == true)
							{
								for (int i = start; i < end; i++)
								{
									if (textVector[i] != wordVectror[wordIndex])
									{
										misMatches++;
										if (misMatches > sizeS - minimumMatches)
										{
											exist = false;
											break;
										}
									}
									wordIndex++;
								}
							}
							/**/
							if (exist)
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
		if (matchesWords.size() > 0)
			PrintMatchesToText(matchesWords, wordVectror, basicOutputFile);
	}
	stop_time = high_resolution_clock::now();
	double timesAvg = 0;
	timesAvg = (double)(duration_cast<microseconds>(stop_time - start_time)).count() / allWords.size();
	cout << "Average time taken by Basic Algorithm: " << timesAvg << " microseconds" << endl;
	OutputConsoleFile << "Average time taken by Basic Algorithm: " << timesAvg << " microseconds" << endl;


	cout << endl << "Starting Naive Algorithm search..." << endl;
	OutputConsoleFile << endl << "Starting Naive Algorithm search..." << endl;
	start_time = high_resolution_clock::now();

	for (auto& wordVectror : allWords)
	{
		map<string, vector<int>> matchesWordsForNaive;
		matchesWordsForNaive = NaiveAlgorithm(textVector, wordVectror, minimumMatches);
		if (matchesWordsForNaive.size() > 0)
			PrintMatchesToText(matchesWordsForNaive, wordVectror, naiveOutputFile);
	}

	stop_time = high_resolution_clock::now();
	timesAvg = (double)(duration_cast<microseconds>(stop_time - start_time)).count() / allWords.size();
	cout << "Average time taken by Naive Algorithm: " << timesAvg << " microseconds" << endl;
	OutputConsoleFile << "Average time taken by Naive Algorithm: " << timesAvg << " microseconds" << endl;

	cout << "Finished all." << endl;
	OutputConsoleFile << "Finished all." << endl;

}


