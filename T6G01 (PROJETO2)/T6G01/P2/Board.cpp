#include "Board.h"

Board::Board()
{
}

Board::~Board()
{
}

//==========================================================================================
//COLOR CODES: (alternative: use symbolic const’s)
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

void Board::InitiateBoard(ifstream &boardFile) //loads a board from a text file
{
	string line, lcd, word;
	bool board = 0; //this variable tells if we are on the lines of the text file that visually define the board
	vector <char> chars_on_line; //vector to temporarily save the characters extracted from the board on the file

	lines = 0;
	columns = 0;

	while (getline(boardFile, line)) //extracts everything on file
	{
		if (board && line != "") //in this case we are dealing with the board on file
		{
			lines++; //for each line extracted the number of lines is incremented in order to set the lines attribute to the right value

			TrimLine(line); //eliminates spaces between characters in the line

			chars_on_line.resize(0); //cleans the vector to ensure no characters from last line remain

			for (size_t i = 0; i < line.length(); i++)
			{
				if (lines == 1)
					columns++; //same as lines++; but to count the columns

				if (isalpha(line[i]))
					line[i] = ' ';

				chars_on_line.push_back(line[i]); //the whole line is swept and every character is put into a vector
			}

			Board_Cells.push_back(chars_on_line); //the vector is pushed into the board that'll be used in the game
		}
		else if (line == "")
			board = 1 - board; //invert the truth value of the variable. The board lines start after a blank line and end when another blank line is found
		else //lines after the board, which follow the standard "LCD [WORD]"
		{
			lcd = line.substr(0, 3); //gets the first three characters of the line which represent the lcd of each word

			word = line.substr(4, line.length() - 4); //gets the word from the line, which always starts at index 4 and its length is the line's length excluding the starting index

			Words_on_Board.insert(pair < string, string >(lcd, word)); //for every line now read we have a pair LCD-WORD that is saved on map
		}
	}

	Board_Cells_CleanCopy = Board_Cells; //saves a copy of this initial state of the board that will be called when removing a word

	DefineIndexes();
}

void Board::Show() //this function displays the board to the user
{
	setcolor(LIGHTCYAN);
	for (size_t i = 0; i <= columns; i++) //displays the first line with the indexes
		cout << setw(2) << indexes[i];

	cout << endl;

	for (size_t i = 0; i < lines; i++) //displays the rest of the board, including the first column with indexes
	{
		setcolor(LIGHTCYAN);
		cout << setw(2) << indexes[i + columns + 1]; //to show the index

		for (size_t k = 0; k < columns; k++)
		{
			if (Board_Cells[i][k] == ' ') //shows white spaces with white
				setcolor(BLACK, WHITE);
			else if (Board_Cells[i][k] == '#') //shows the '#' with black
				setcolor(BLACK, BLACK);
			else
				setcolor(BLUE, WHITE);
			cout << setw(2) << Board_Cells[i][k]; //to show the rest of the line
		}

		setcolor(WHITE);
		cout << endl;
	}
}

bool Board::CheckLCD(string & lcd) //decodes user input lcd (line column and direction) and verifies if it is valid
{

	if (lcd.length() != 3) //lcd can't have more than 3 characters
		return false;

	l = ((int)lcd[0] - (int)'A');
	if (l >= lines)
		return false;

	c = ((int)lcd[1] - (int)'a');
	if (c >= columns)
		return false;

	if (islower(lcd[2]))
		lcd[2] = toupper(lcd[2]); //the last char on the LCD can either be uppercase or lowercase

	d = lcd[2];
	if (d == 'V' || d == 'H') //but has to be either a v or an h indicating the direction that'll be used to work on the board
		return true;

	return false;
}

bool Board::CheckSize(const string & word) //checks if the word fits in the board
{
	size_t i;

	if (d == 'V')
	{
		i = l + word.length();
		if (i > lines)			//when writing vertically, we need to check if the length of the word plus the starting position is greater then the total number of columns
			return false;
	}
	else
	{
		i = c + word.length();
		if (i > columns)		//when writing horizontally, we need to check if the length of the word plus the starting position is greater then the total number of lines
			return false;
	}

	return true;
}

bool Board::CheckMatching(const string & word) //checks if the user input can be matched with the words on the board (crosswords)
{
	size_t i;

	if (d == 'V')			  //writing vertically
	{
		if (l != 0)
		{
			if (isalpha(Board_Cells[l - 1][c])) //if the word doesnt start in the first line, it can't have a letter immediatly before it
				return false;
		}


		for (i = 0; i < word.length(); i++) //goes through the word and checks if there is anything already on board that doesnt match the letters of the word
		{
			if (Board_Cells[l + i][c] == '#')
			{
				return false;
			}
			else if (isalpha(Board_Cells[l + i][c]))
			{
				if (Board_Cells[l + i][c] != word[i])
					return false;
			}
		}

		if ((l + i) < lines)
		{
			if (isalpha(Board_Cells[l + i][c])) //if the word doesnt end in the last line, it can't have a letter immediatly after it
				return false;
		}

	}
	else				 //writing horizontally
	{
		if (c != 0)
		{
			if (isalpha(Board_Cells[l][c - 1])) //if the word doesnt start in the first column, it can't have a letter immediatly before it
				return false;
		}

		for (i = 0; i < word.length(); i++) //goes through the word and checks if there is anything already on board that doesnt match the letters of the word
		{
			if (Board_Cells[l][c + i] == '#')
			{
				return false;
			}
			else if (isalpha(Board_Cells[l][c + i]))
			{
				if (Board_Cells[l][c + i] != word[i])
					return false;
			}
		}

		if ((c + i) < columns)
		{
			if (isalpha(Board_Cells[l][c + i])) //if the word doesnt end in the last line, it can't have a letter immediatly after it
				return false;
		}
	}

	return true;
}

bool Board::Insert(const string & lcd, const string & word) //this function inserts a word into the board
{
	if (!CheckSize(word))
	{
		setcolor(RED, WHITE);
		cerr << "Invalid size. Word is too long." << endl;
		setcolor(WHITE);
		return false;
	}

	if (!CheckMatching(word))
	{
		setcolor(RED, WHITE);
		cerr << "Invalid match. Word doesn't match the existing letters." << endl;
		setcolor(WHITE);
		return false;
	}

	size_t i;

	if (d == 'V')			  //writing vertically
	{
		for (i = 0; i < word.length(); i++)
		{
			Board_Cells[l + i][c] = word[i]; //writes every character of the word on the board
		}
	}
	else					  //writing horizontally
	{
		for (i = 0; i < word.length(); i++)
		{
			Board_Cells[l][c + i] = word[i]; //writes every character of the word on the board
		}
	}

	User_Words.insert(pair < string, string >(lcd, word));

	return true;
}

void Board::Remove(const string & lcd) //removes a word that was insert by the user
{
	if (Words_on_Board.find(lcd) == Words_on_Board.end()) //reads the user lcd and if there is no word corresponding to that lcd in User_Words a warning is sent to the screen
	{
		setcolor(RED, WHITE);
		cerr << "There is no valid word to remove at given position." << endl;
		setcolor(WHITE);
		cout << "Please insert the position of the first letter of the word you want to remove." << endl;
		return;
	}

	User_Words.erase(lcd); //erase the position and word insert by the user

	Board_Cells = Board_Cells_CleanCopy; //reinitiates the board

	for (const auto & x : User_Words) //goes through the map and uses the pairs LCD-WORD to re-insert the words on the board
	{
		string words_lcd = x.first;
		CheckLCD(words_lcd);
		Insert(words_lcd, x.second);
	}
}

void Board::Replace(string & lcd, const string & word) //replaces the word on board with the new word given
{
	string word_copy;

	word_copy = User_Words[lcd]; //saves a copy of the previously placed word

	Remove(lcd); //removes the word on the board

	CheckLCD(lcd); //since remove affects l, c and d we have to update them

	if (!Insert(lcd, word)) //if the word the user gave can't be inserted on the board, then the previous word is placed back
		Insert(lcd, word_copy);
}

vector <string> Board::GetLCD(bool horizontal) //this function extracts all positions (lcd) used as keywords for the words saved in the original board
{
	vector <string> LCDs; 
	char direction;

	if (horizontal)
		direction = 'H'; //either selects all the words written horizontally
	else 
		direction = 'V'; //or vertically

	for (map<string, string>::const_iterator iter = Words_on_Board.begin(); iter != Words_on_Board.end(); iter++)
	{
		string lcd = iter->first;

		if (lcd[2] == direction)
			LCDs.push_back(lcd); //collects in a vector all the words written in a given direction
	}

	return LCDs;
}

string Board::GetWord(const string & user_lcd) //returns the original word that is saved in a position (lcd)
{
	return Words_on_Board[user_lcd];
}

bool Board::IsFull() //checks if the board is full by seeing if the number of words that were input by the user is the same number of original words
{
	for (size_t i = 0; i < lines; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			if (Board_Cells[i][j] == ' ') //the board will be full when there are no white spaces
				return false;
		}
	}

	return true;
}

bool Board::FinalVerification(map <string, string, cmpLCD> & correct_words) //verifies is the words input by the user are the same as the original words in the correct positions
{
	bool correct = 1;
	correct_words.clear(); //words that the user guessed right

	for (map<string, string>::const_iterator iter = Words_on_Board.begin(); iter != Words_on_Board.end(); iter++) //sweeps the map containing the original words
	{
		if (iter->second != User_Words[iter->first]) //and checks if the corresponding word is the same as the one the user input
			correct = 0;
		else
			correct_words.insert(pair<string, string>(iter->first, iter->second)); //every right word is pushed into the vector
	}

	if (correct)
		return true;
	else return false;
}

void Board::DefineIndexes() //this function exists only to indicate the indexes that surround the board to help the user
{
	indexes.resize(lines + columns + 1); //the vector size will be the blank space, the number of indexes on top (columns) and the number of indexes at left (lines)

	indexes[0] = ' '; //the first position of the index is meant to not be seen

	for (size_t i = 1; i <= columns; i++)  // we first write the indexes of the columns (that appear on top of the board)
		indexes[i] = (char)((int)'a' + i - 1);

	for (size_t i = columns + 1; i <= columns + lines; i++) //then, we add the indexes of the lines (that appear on the left side of the board)
		indexes[i] = (char)((int)'A' + (i - columns - 1));
}

void Board::TrimLine(string &line) //eliminates all the spaces on a given line/string
{
	string line_trimed = "";

	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] != ' ') //doesn't accumulate any white spaces
			line_trimed = line_trimed + line[i];
	}

	line = line_trimed;
}

//==========================================================================================
// Set text color
void Board::setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
//==========================================================================================
// Set text color & background
void Board::setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}