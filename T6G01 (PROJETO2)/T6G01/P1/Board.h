#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

class Board
{
public:
	Board();
	~Board();
	void Create(unsigned int num_lines, unsigned int num_columns, const string & dictionaryFileName);
	void ResumeBoard(ifstream &boardFile, const string & boardFileName, const string &dictionaryFileName);
	void Show();
	bool IsFull();
	bool CheckLCD(string & lcd);
	bool CheckSize(const string & word);
	bool CheckMatching(const string & word);
	bool CheckExistance(const string & word);
	bool CheckParallelism(const string & word);
	void Insert(const string & lcd, const string & word, bool remove_flag = 0);
	void Remove(const string & lcd);
	string Help();
	void Save_NewBoard(bool finish = 0);
	void Save_ResumeBoard(bool finish = 0); 

private:
	unsigned int lines, columns, l, c; //number of lines and columns of the board. l and c are indexes that point to where the user wants to add/delete words
	char d; //d stands for direction, which is passed by the user to set the direction of writing/deleting
	string Board_FileName, Dictionary_FileName; //board and dictionary names to be saved
	vector <vector <char>> Board_Cells; //the actual content of the board
	vector <char> indexes; //a vector with the indexes corresponding to the boards lines and columns.
	class cmpLCD //a new method of comparing that sorts the LCD's by horizontal and vertical first and only then orders them alphabetically
	{
	public:
		bool operator() (const string & a, const string & b) const
		{
			if (a[2] < b[2]) //a[3] = H && b[3] = V
			{
				return true;
			}
			else if (a[2] > b[2])
			{
				return false;
			}
			else
			{
				string shortA, shortB;
				shortA = a.substr(0, 2);
				shortB = b.substr(0, 2);

				return shortA < shortB;
			}
		}
	};
	map <string, string, cmpLCD> Words_on_Board; //a map that saves the matching LCD - Word. Useful to save the board and to the 2nd part of the project

	void DefineIndexes();
	void TrimLine(string &line);
	void setcolor(unsigned int color);
	void setcolor(unsigned int color, unsigned int background_color);
};

#endif