#include "Dictionary.h"

Dictionary::Dictionary()
{
}

Dictionary::~Dictionary()
{
}

void Dictionary::ExtractWords(ifstream &dictionaryFile) //extracts words from the dictionary file
{
	string line;

	while (getline(dictionaryFile, line)) //verify every line until the end of file
	{
		separateWords(line, wordList);
	}
}

bool Dictionary::WordExists(const string & word) //returns true if the word is on the dictionary and false otherwise
{
	if (wordList.find(word) != wordList.end())
		return true;
	else return false;
}

vector <string> Dictionary::MatchingWords(const string & word) //returns a vector with the words that are a wildcard match with the word given
{
	vector <string> matches; //the vector where the matching words are stored

	for (const auto & x : wordList) //goes through the map and everytime there is a Wildcard Match the word is added to the matches
	{
		if (wildcardMatch(x.first.c_str(), word.c_str()))
		{
			matches.push_back(x.first);
		}
	}

	return matches;
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

void Dictionary::separateWords(string & line, map < string, vector <string> > &wordList)
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

				if (first_word)
				{
					keyword = word; //keyword is the first word of the line

					first_word = 0;
				}
				else synonyms.push_back(word); //adds the word to the end of the vector
			}
			word = ""; //reinitializes the word as null so it doesnt go accumulating characters
		}
		else
			word = word + line[i]; //accumulates chars to build a word
	}

	if (synonyms.size() != 0) //only add a word and its synonyms to the map if the word has synonyms
		wordList.insert(pair < string, vector <string> >(keyword, synonyms)); //insert the word and it's synonyms into the map
}

bool Dictionary::validWord(string word) //verifies if a word is valid
{
	size_t i = 0;

	while (i < word.length())
	{
		if (!(isalpha(word[i]) || word[i] == ' ')) // it searches the entire word until it reaches the end or finds a char that is not a letter
			return false;
		else i++;
	}

	return true;
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

//////////////////////////////////////////////////////////////////////////
// WildcardMatch - MODIFIED VERSION
// str - Input string to match
// strWild - Match mask that may contain wildcards
//
// A . sign matches any character, except an empty string.
// Characters are compared caseless.
//
// ADAPTED FROM:
// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
//////////////////////////////////////////////////////////////////////////

bool Dictionary::wildcardMatch(const char *str, const char *strWild)
{
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '.')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}