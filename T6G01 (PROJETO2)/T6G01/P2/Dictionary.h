#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;

class Dictionary
{
public:
	Dictionary(); //object constructor by default
	~Dictionary();
	void ExtractWords(ifstream &dictionaryFile); //extracts the words of a dictionary for a the map variable 'wordList'
	void EverythingToUpper(string &word); //makes every letter upper case
	string GetSynonym(const string &lcd, const string &real_word); //gets a random synonym
	void AddClue(const string &lcd, const string &synonym); //adds a synonym to the list of words that will be shown if the user wants to see all clues already shown for each word
	vector <string> GetClues(const string &lcd); //returns a vector containing all clues already given to a certain word (represented by its lcd)

private:
	map < string, vector <string> > wordList; //variable that holds the words used on the board as keywors and a vector of synonyms for each as value
	map < string, vector <string> > Clues; //list of clues that the user may see for each position
	void separateWords(string line, map < string, vector <string> > &wordList); //separates the words in the dictionary file (to help the extraction)
	void trimWord(string &word); //trims a word (to help the extraction)
	bool validWord(const string &word); //averiguates if the word may be added to the 'wordList' (also used in the extraction)
};

#endif