#include "Dictionary.h"

Dictionary::Dictionary()
{
}

Dictionary::~Dictionary()
{
}

void Dictionary::ExtractWords(ifstream &dictionaryFile) //extracts the words of a dictionary for a the map variable 'wordList'
{
	string line;

	while (getline(dictionaryFile, line))//verify every line until the end of file
	{
		separateWords(line, wordList);
	}
}

void Dictionary::EverythingToUpper(string &word) // this function simply puts all letters in upper case
{
	size_t i = 0;

	while (i < word.length())
	{
		if (islower(word[i])) //if it is a letter and is in lower case, it makes it upper
		{
			word[i] = toupper(word[i]);
		}

		i++;
	}
}

string Dictionary::GetSynonym(const string &lcd, const string &real_word) //returns a synonym for a given word or an empty string if there are no more synonyms available for that word
{
	string synonym;
	size_t i;

	if (wordList[real_word].size() == 0)
	{
		synonym = ""; //when no more synonyms are available returns an empty string
	}
	else
	{
		i = rand() % wordList[real_word].size(); //randomly selects a synonym

		synonym = wordList[real_word][i]; //that will be returned

		wordList[real_word].erase(wordList[real_word].begin() + i); //erases the synonym from the vector holding the synonyms

		AddClue(lcd, synonym); //and from now on the synonym is part of the clues show to the user
	}

	return synonym;
}

void Dictionary::AddClue(const string &lcd, const string &synonym) //adds a synonym to the list of words that will be shown if the user wants to see all clues already shown for each word
{
	if (Clues.find(lcd) == Clues.end()) //in this case there isn't any clue yet registered
	{
		vector <string> vector_of_synonyms;

		vector_of_synonyms.push_back(synonym); //a vector is created and the synonym is pushed into it
		
		Clues.insert(pair < string, vector <string> >(lcd, vector_of_synonyms)); //inserts a new pair LCD-Clues into the map Clues
	}
	else
	{
		Clues[lcd].push_back(synonym); //otherwise the synonym is simply pushed into the vector of clues already existant
	}
}

vector <string> Dictionary::GetClues(const string &lcd) //returns a vector containing all clues already given to a certain word (represented by its lcd)
{
	return Clues[lcd];
}

void Dictionary::separateWords(string line, map < string, vector <string> > &wordList) //separates the words in the dictionary file (to help the extraction)
{
	bool first_word = 1; //flag that is true when the keyword (first word of the line) is being selected
	string word = "";
	string keyword = "";
	vector <string> synonyms; //vector containing the synonyms of a given keyword

	for (size_t i = 0; i < line.length(); i++) //searches the whole line, separating letters when it finds the end of the line or a ;
	{
		if (i == line.length() - 1 || line[i] == ':' || line[i] == ',' || line[i] == '\n') //conditions that delimit possible words
		{
			if (isalpha(line[i])) //this applies if the last character of the line is a letter
				word = word + line[i]; //so it is added

			trimWord(word); //trims the word

			if (word != "" && validWord(word)) //if it is a valid word
			{
				EverythingToUpper(word); //makes every letter upper case

				if (first_word) //if it is the first word of the line, it is a keyword (will be a keyword in the 'wordList' map)
				{
					keyword = word; //keyword is the first word of the line

					first_word = 0;
				}
				else synonyms.push_back(word); //in this case it is a synonym, so it is added to the end of the vector of synonyms of that word
			}
			word = ""; //reinitializes the word as null so it doesn't accumulating characters
		}
		else
			word = word + line[i]; //accumulates chars to build a word
	}

	if (synonyms.size() != 0) //only had a word and its synonyms to the list if the word has synonyms
		wordList.insert(pair < string, vector <string> >(keyword, synonyms)); //insert the word and it's synonyms into the map
}

void Dictionary::trimWord(string &word) //cleans the word from any space after and/or before it
{
	size_t i = word.length() - 1; //index of the last char of the word

	while (word != "" && word[0] == ' ')
	{
		word = word.substr(1, i); //the first character is removed
		i--; //if one letter is removed then the last char index has to be updated
	}

	while (word != "" && word[i] == ' ')
	{
		word = word.substr(0, i); //the last character is removed
		i--;
	}
}

bool Dictionary::validWord(const string &word) //averiguates if the word may be added to the 'wordList' (also used in the extraction)
{
	for (size_t i = 0; i < word.length(); i++)
	{
		if (!(isalpha(word[i]) || word[i] == ' ')) // it searches the entire word until it reaches the end or finds a char that is not an upper case letter or a blank space
			return false;
	}

	return true;
}