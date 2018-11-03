#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>
#include <Windows.h>

using namespace std;

class Board
{
public:
	Board();
	~Board();
	void InitiateBoard(ifstream &boardFile); //starts the process of initializing the board that is going to be used in the game
	void Show(); //shows the board
	bool CheckLCD(string & lcd); //verifies if the selected lcd is correct
	bool CheckSize(const string & word); //verifies if the word writen by the user fits in the board
	bool CheckMatching(const string & word); //verifies if the word writen by the user matches the ones already writen
	bool Insert(const string & lcd, const string & userWord); //inserts the word if everything checks out
	void Remove(const string & lcd); //removes a word writen by the user from the board
	void Replace(string & lcd, const string & word); //replaces the word on board with the new word given 
	vector <string> GetLCD(bool horizontal); //gets the lcds used in the board
	string GetWord(const string & user_lcd); //gets the original word through the lcd introduced
	bool IsFull(); //verifies if the board is full
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
	bool FinalVerification(map <string, string, cmpLCD> & correct_words); //verifies if the words introduced by the user are the same and are in the same position as the original words

private:
	unsigned int lines, columns, l, c; //number of lines and columns of the board. l and c are indexes that point to where the user wants to add/delete words
	char d; //d stands for direction, which is passed by the user to set the direction of writing/deleting
	vector <vector <char>> Board_Cells, Board_Cells_CleanCopy; //Board_Cells is used to show the user the letters writen on the board, the Board_CellsCopy is used to reinitiate the board (when a word is removed)
	vector <char> indexes; //a vector with the indexes corresponding to the boards lines and columns.
	map < string, string > Words_on_Board; //a map that saves the matching LCD - Word from the original board 
	map < string, string > User_Words; //a map that saves the matching LCD - Word with the original guesses
	void DefineIndexes(); // given the number of lines and columns of the board, we find the indexes
	void TrimLine(string &line); //trims the line (deletes the spaces)
	void setcolor(unsigned int color);
	void setcolor(unsigned int color, unsigned int background_color);
};

#endif