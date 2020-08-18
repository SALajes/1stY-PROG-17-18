//First Project PROG

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>

using namespace std;

void openFile(string &dictionaryFile, string &wordlistFileName, ifstream &inputFile_dictionary) //this function opens the file
{
	cout << "EXTRACTION OF WORD LIST FROM DICTIONARY" << endl
		<< "=======================================" << endl;

	cout << "Dictionary file ? ";
	cin >> dictionaryFile; //collects the file name of input (dictionary)

	inputFile_dictionary.open(dictionaryFile); //open the text file (dictionary)

	while (!inputFile_dictionary.is_open()) //only allowed to move forward when the text file is opened
	{
		cerr << dictionaryFile << " not found, please verify it and introduce it again." << endl;
		cin.ignore(10000, '\n');
		cin >> dictionaryFile;

		if (cin.eof())
			exit(0);

		inputFile_dictionary.open(dictionaryFile);
	}

	cout << "Word list file ? ";
	cin >> wordlistFileName; //collects the file name of output (word list) 
}

bool IsItHeadline(string line) 
{
	size_t i = 0;

	if (line == "" || line == "\n") //if the line is composed only by a new line or null line it is automaticly false
	{
		return false;
	}
	else
	{
		while (i < line.length()) //it will search the whole line until it ends or until it finds a char than acts according to the if condition 
		{
			char c = line[i];

			if (!((c >= 'A' && c <= 'Z') || c == ' ' || c == ';' || c == '-' || c == '\''))
			{
				return false; //if the line contains a char other than an upper case letter, a space, a semi colon, a hiphen or an apostrophee, it is not an headline
			}

			i++;
		}
	}

	return true; //if none of the conditions invalidates it, a word has been found
}

void countingHeadlines(int &counter, char first_letter)
{
	static char letter = 'A'; //initialize the headline's first letter counter with A, only once

	counter++; //adds an headline found

	//if the first letter of the headline is different from the letter that is been evaluated AND it is the letter that follows:
	if (letter != first_letter && first_letter == (char)(((int)letter) + 1)) 
	{
		counter = 1;	//it counts has an headline for the new, following letter
		letter = first_letter; //the letter is changed for the one that follows
		cout << endl << letter << endl; //the new letter is shown
	}
	// if the letter is that some, or if its different but not the letter that follows, we just keep counting it
	else if (counter == 100) // if a 100 headlines have been found for one letter, we show a dot, and the counter for the letter reinitiates
	{
		cout << ".";
		counter = 0;
	}
}

void trimWord(string &word)
{
	int flag = 0; //this flag identifies which case we are dealing with
	size_t i = word.length() - 1; //index of the last char of the word

	if (word[0] == ' ')
		if (word[i] == ' ') 
			flag = 3; //in this case there is a space before and after the word
		else
			flag = 1; // there is a space only before the word
	else if (word[i] == ' ')
		flag = 2; //there is a space only after the word

	switch (flag) //trims the word (eliminates the spaces) according to the case (flag state)
	{
	case 1:
		word = word.substr(1, i);
		break;

	case 2:
		word = word.substr(0, i);
		break;

	case 3:
		word = word.substr(1, i - 1);
		break;
	}
}

bool validWord(string word) //verifies if a word is valid
{
	size_t i = 0;
	
	while (i < word.length()) 
	{
		if (!(word[i] >= 'A' && word[i] <= 'Z')) // it searches the entire word until it reaches the end or finds a char that is not an upper case letter
			return false;
		else i++;
	}

	return true;
}

void separateWords(string line, vector<string> &wordList, int &numSimpleWords)
{
	string word = "";

	for (size_t i = 0; i <= line.length(); i++) //searches the whole line, separating letters when it finds the end of the line or a ;
	{
		if ((i == line.length() && word != "") || line[i] == ';' || line[i] == '\n')
		{
			trimWord(word); //trims the word
			if (validWord(word)) //if it is a valid word
			{
				wordList.push_back(word); //it adds it to the end of the vector
				numSimpleWords++; //increments the number of found words
			}
			word = ""; //reinitializes the word as null so it doesnt go accumulating characters
		}
		else
			word = word + line[i]; //accumulates chars to build a word
	}
}

void ExtractWords(ifstream &inputFile_dictionary, vector<string> &wordList, string dictionaryFile)
{
	size_t i = 0;
	int numSimpleWords = 0;
	int counter = 0;
	string word = ""; 
	string line;

	cout << "Extracting simple words from file " << dictionaryFile << "," << endl
		<< "beginning with letter ..." << endl << "A" << endl;

	while (getline(inputFile_dictionary, line))//verify every line until the end of file
	{
		if (IsItHeadline(line)) //verify if the line is a headline
		{
			countingHeadlines(counter, line[0]);//count the headline
			separateWords(line, wordList, numSimpleWords); //separates and pushes the valid words to vector
		}
	}

	cout << endl << "==============END OF FILE==============" << endl 
		<< "Number of simple words = " << numSimpleWords << endl; //show the total number of collected words
}

void organizeVector(vector<string> &wordList)
{
	int numNonDuplicateWords;

	//organize the structure where the words are being kept, and showing the resulting number of words
	cout << "Sorting words ..." << endl;

	sort(wordList.begin(), wordList.end()); //sort the vector (of the current first letter of every word) alphabetically

	cout << "Removing duplicate words ..." << endl;

	wordList.erase(unique(wordList.begin(), wordList.end()), wordList.end()); //remove the duplicates in that vector

	numNonDuplicateWords = wordList.size();

	cout << "Number of non-duplicate simple words = " << numNonDuplicateWords << endl;
}

void WriteonFile(string wordlistFileName, vector<string> wordList) //save the list of collected words in a selected text file
{
	ofstream outputFile_wordList;
	size_t i = 0;

	cout << "Saving words into file " << wordlistFileName << " ..." << endl;

	outputFile_wordList.open(wordlistFileName); //open the file 

	while (i < wordList.size()) //search every word of the vector of words
	{
		outputFile_wordList << wordList[i] << endl; //write the current word in the file

		i++;
	}

	outputFile_wordList.close(); //close the file
}

int main()
{
	string dictionaryFile, wordlistFileName;
	ifstream inputFile_dictionary;
	vector <string> wordList; //create vector of type string to save the words

	openFile(dictionaryFile, wordlistFileName, inputFile_dictionary);//open the file

	ExtractWords(inputFile_dictionary, wordList, dictionaryFile); //extract the words available in the file

	inputFile_dictionary.close(); //close the file of extraction

	organizeVector(wordList); //organize  the vector of words

	WriteonFile(wordlistFileName, wordList); //save the words in a text file

	cout << "End of processing." << endl;

	system("pause");

	return 0;
}