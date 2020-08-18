#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <fstream>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>

using namespace std;

class Dictionary
{
public:
	Dictionary();
	~Dictionary();
	void ExtractWords(ifstream &dictionaryFile);
	bool WordExists(const string & word);
	vector <string> MatchingWords(const string & word);
	void EverythingToUpper(string &word);

private:
	map < string, vector <string> > wordList; //map containing the pairs WORD-SYNONYMS

	void separateWords(string & line, map < string, vector <string> > &wordList);
	bool validWord(string word);
	void trimWord(string &word);
	bool wildcardMatch(const char *str, const char *strWild);
};

#endif