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



void Board::Create(unsigned int num_lines, unsigned int num_columns, const string & dictionaryFileName) //creates the board filled with dots
{
	lines = num_lines; //sets the attribute lines of the board as the user input lines
	columns = num_columns; //sets the attribute columns of the board as the user input columns

	Board_Cells.resize(lines);

	Dictionary_FileName = dictionaryFileName;

	for (size_t i = 0; i < lines; i++)
	{
		Board_Cells[i].resize(columns, '.'); //every line is a vector and this fills every vector with dots
	}

	DefineIndexes();
}

void Board::ResumeBoard(ifstream &boardFile, const string & boardFileName, const string & dictionaryFileName) //loads a board from a text file
{
	string line, lcd, word;
	bool board = 0; //this variable tells if we are on the lines of the text file that visually define the board
	vector <char> chars_on_line; //vector to temporarily save the characters extracted from the board on the file

	Board_FileName = boardFileName; 
	Dictionary_FileName = dictionaryFileName;

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

				chars_on_line.push_back(line[i]);  //the whole line is swept and every character is put into a vector
			}

			Board_Cells.push_back(chars_on_line); //the vector is pushed into the board that'll be used in the game
		}
		else if (line == "")
			board = 1 - board; //invert the truth value of the variable. The board lines start after a blank line and end when another blank line is found
		else //lines after the board, which follow the standard "LCD [WORD]"
		{
			lcd = line.substr(0, 3); //gets the first three characters of the line which represent the lcd of each word

			word = line.substr(4, line.length() - 4); //gets the word from the line, which always starts at index 4 and its length is the line's length excluding the starting index

			Words_on_Board.insert(pair <string, string > (lcd, word)); //for every line now read we have a pair LCD-WORD that is saved on map
		}
	}

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

		setcolor(BLUE, WHITE);
		for (size_t k = 0; k < columns; k++)
			cout << setw(2) << Board_Cells[i][k]; //to show the rest of the line
		setcolor(WHITE);
		cout << endl;
	}
}


bool Board::IsFull() //Returns true if the board is full/completed and false otherwise
{
	for (size_t i = 0; i < lines; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			if (Board_Cells[i][j] == '.') //a board will be completed if there are only letters and black cells on it (no dots)
				return false;
		}
	}

	return true;
}

bool Board::CheckLCD(string & lcd)//decodes user input lcd (line column and direction) and verifies if it is valid
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

bool Board::CheckExistance(const string & word) //cheks if the word is already placed on the board
{
	for (const auto & x : Words_on_Board) //goes through the saved pairs LCD-WORD to see if the word the user wants to input is already part of it
	{
		if (x.second == word)
		{
			return false;
		}
	}

	return true;
}

bool Board::CheckParallelism(const string & word) //checks if there are words written in the same direction of the user input word in the next line/column or in the previous line/column
{
	if (d == 'V') //user is trying to write a word vertically
	{
		for (size_t i = 0; i < word.length(); i++)
		{
			if (c > 0 && isalpha(Board_Cells[l + i][c - 1])) //verifies the letters on the left if not the first column
			{
				if (i == 0) //if it is the first letter of the word
				{
					if (l > 0) //if it is not the first line
						if (isalpha(Board_Cells[l - 1][c - 1])) //if there is a letter above on the left
							return false;

					if (isalpha(Board_Cells[l + 1][c - 1])) //if there is a letter under on the left
						return false;
				}
				else if (i == word.length() - 1) //if it is the last letter of the word
				{
					if (l + i < lines - 1) //if it is not the last line
						if (isalpha(Board_Cells[l + i + 1][c - 1])) //if there is a letter under on the left
							return false;
				}
				else if (isalpha(Board_Cells[l + i + 1][c - 1])) //if it is a letter in the middle it verifies if there is a letter in the next line on the left
					return false;
			}

			if (c < columns - 1 && isalpha(Board_Cells[l + i][c + 1])) // verifies the letters on the right if any
			{
				if (i == 0) //if it is the first letter of the word
				{
					if (l > 0) //if it is not the first line
						if (isalpha(Board_Cells[l - 1][c + 1])) //if there is a letter above on the right
							return false;

					if (isalpha(Board_Cells[l + 1][c + 1])) //if there is a letter under on the right
						return false;
				}
				else if (i == word.length() - 1) //if it is the last letter of the word
				{
					if (l + i < lines - 1) //if it is not the last line
						if (isalpha(Board_Cells[l + i + 1][c + 1])) //if there is a letter under on the right
							return false;
				}
				else if (isalpha(Board_Cells[l + i + 1][c + 1])) //if it is a letter in the middle it verifies if there is a letter in the next line on the right
					return false;
			}
		}
	}
	else //user is trying to write a word horizontally
	{
		for (size_t i = 0; i < word.length(); i++)
		{
			if (l > 0 && isalpha(Board_Cells[l - 1][c + i])) //verifies the letters on the left if not the first column
			{
				if (i == 0) //if it is the first letter of the word
				{
					if (c > 0) //if it is not the first line
						if (isalpha(Board_Cells[l - 1][c - 1])) //if there is a letter above on the left
							return false;

					if (isalpha(Board_Cells[l - 1][c + 1])) //if there is a letter under on the left
						return false;
				}
				else if (i == word.length() - 1) //if it is the last letter of the word
				{
					if (c + i < columns - 1) //if it is not the last line
						if (isalpha(Board_Cells[l - 1][c + i + 1])) //if there is a letter under on the left
							return false;
				}
				else if (isalpha(Board_Cells[l - 1][c + i + 1])) //if it is a letter in the middle it verifies if there is a letter in the next line on the left
					return false;
			}

			if (l < lines - 1 && isalpha(Board_Cells[l + 1][c + i])) // verifies the letters on the right if any
			{
				if (i == 0) //if it is the first letter of the word
				{
					if (c > 0) //if it is not the first line
						if (isalpha(Board_Cells[l + 1][c - 1])) //if there is a letter above on the right
							return false;

					if (isalpha(Board_Cells[l + 1][c + 1])) //if there is a letter under on the right
						return false;
				}
				else if (i == word.length() - 1) //if it is the last letter of the word
				{
					if (c + i < columns - 1) //if it is not the last line
						if (isalpha(Board_Cells[l + 1][c + i + 1])) //if there is a letter under on the right
							return false;
				}
				else if (isalpha(Board_Cells[l + 1][c + i + 1])) //if it is a letter in the middle it verifies if there is a letter in the next line on the right
					return false;
			}
		}
	}

	return true;
}

void Board::Insert(const string & lcd, const string & word, bool remove_flag) //this function inserts a word into the board
{
	if (!remove_flag) //the remove_flag marks if the function is or not called by remove and disables the checkExistance for every inserted word
	{
		if (!CheckExistance(word))
		{
			setcolor(RED, WHITE);
			cerr << "Word already exists on board." << endl;
			setcolor(WHITE);
			return;
		}
	}

	if (!CheckSize(word))
	{
		setcolor(RED, WHITE);
		cerr << "Invalid size. Word is too long." << endl;
		setcolor(WHITE);
		return;
	}

	if (!CheckMatching(word))
	{
		setcolor(RED, WHITE);
		cerr << "Invalid match. Word doesn't match the existing letters." << endl;
		setcolor(WHITE);
		return;
	}

	if (!CheckParallelism(word))
	{
		setcolor(RED, WHITE);
		cerr << "There is a word written in the same direction on an adjacent line/column." << endl;
		setcolor(WHITE);
		cout << "Try to insert a word on another line/column." << endl;
		return;
	}

	size_t i;

	if (d == 'V')			  //writing vertically
	{
		if (l != 0)
		{
			Board_Cells[l - 1][c] = '#'; //if the word doesnt start in the first line, a black cell is written before it
		}

		for (i = 0; i < word.length(); i++)
		{
			Board_Cells[l + i][c] = word[i]; //writes every character of the word on the board
		}

		if ((l + i) < lines)
		{
			Board_Cells[l + i][c] = '#'; //if the word doesnt end in the last line, a black cell is written after it
		}
	}
	else					  //writing horizontally
	{
		if (c != 0)
		{
			Board_Cells[l][c - 1] = '#'; //if the word doesnt start in the first column, a black cell is written before it
		}

		for (i = 0; i < word.length(); i++)
		{
			Board_Cells[l][c + i] = word[i]; //writes every character of the word on the board
		}

		if ((c + i) < columns)
		{
			Board_Cells[l][c + i] = '#'; //if the word doesnt end in the last column, a black cell is written after it
		}
	}

	Words_on_Board.insert(pair <string, string >(lcd, word)); //for every word inserted the pair LCD-WORD is afterwards saved in order to keep track of what is written on the board
}

void Board::Remove(const string & lcd) //removes a word from the board
{
	if (Words_on_Board.find(lcd) == Words_on_Board.end()) //reads the user lcd and if there is no word corresponding to that lcd a warning is sent to the screen
	{
		setcolor(RED, WHITE);
		cerr << "There is no valid word to remove at given position." << endl;
		setcolor(WHITE);
		cout << "Please insert the position of the first letter of the word you want to remove." << endl;
		return;
	}

	Words_on_Board.erase(lcd); //deletes the pair LCD-WORD from the map

	//the board will now be reset and filled with dots
	Board_Cells.clear();
	Board_Cells.resize(lines); 

	for (size_t i = 0; i < lines; i++)
	{
		Board_Cells[i].resize(columns, '.');
	}

	for (const auto & x : Words_on_Board) //goes through the map and uses the pairs LCD-WORD to re-insert the words on the board
	{
		string words_lcd = x.first;
		CheckLCD(words_lcd);
		Insert(words_lcd, x.second, 1);
	}
}

string Board::Help() //reads the content of the board starting from lcd and returns the result to allow the checking of matches with words on the dictionary
{
	string word = "";

	if (d == 'V') //reading vertically
	{
		for (; l < lines; l++)
		{
			if (Board_Cells[l][c] == '#')
				break;
			else word = word + Board_Cells[l][c]; //goes through the board starting at l and ending when it reaches the end of the board or a black cell. Accumulates every character in a string.
		}
	}
	else //reading horizontally
	{
		for (; c < columns; c++)
		{
			if (Board_Cells[l][c] == '#')
				break;
			else word = word + Board_Cells[l][c]; //goes through the board starting at c and ending when it reaches the end of the board or a black cell. Accumulates every character in a string.
		}
	}

	return word;
}

void Board::Save_NewBoard(bool finish) //create a new file (create board option) to save the board. When finish is true, every dot on the board will be set to a black cell (board is finished)
{
	string FileName; //name of the saving file
	ifstream testing_existingFiles; //auxiliar file stream to ensure which will be the name of the saving file
	ofstream savingFile; //testing_existingFiles;
	unsigned int i = 1;

	while (true)
	{
		ostringstream convert; //string stream where the file name will be put in

		convert << "b";

		if (i < 10)
			convert << 0 << 0 << i;
		else if (i < 100)
			convert << 0 << i;
		else if (i < 1000)
			convert << i;
		else //when all files from b001 to b999 already exist a warning is sent to the screen
		{
			setcolor(RED, WHITE);
			cerr << "It is not possible to create new files, please delete some and try again." << endl << endl;
			setcolor(WHITE);
			return;
		}

		convert << ".txt";

		FileName = convert.str(); //the filename becomes bXXX.txt

		testing_existingFiles.open(FileName);

		if (!testing_existingFiles.is_open()) //tries to open the file to ensure it doesnt exist
			break;

		testing_existingFiles.close();

		i++;

		FileName = ""; //while the file is opened it means some other file with same name exists, so it passes to the next possible file and clear the filename
	}

	//from now on we already have a file to open and write on
	savingFile.open(FileName);

	savingFile << Dictionary_FileName << endl << endl; //first line is the dictionary's name and the second a blank line

	for (size_t i = 0; i < lines; i++) //sends every character on board to the file
	{
		for (size_t k = 0; k < columns; k++)
		{
			if (finish) //when the user wants to finish the board every dot is replaced by a black cell
				if (Board_Cells[i][k] == '.')
					Board_Cells[i][k] = '#';

			savingFile << setw(2) << Board_Cells[i][k];
		}

		savingFile << endl;
	}

	savingFile << endl; //other blank line after the board on file

	for (const auto & x : Words_on_Board) //goes through the map and writes every lcd and correspondent word after the board
	{
		savingFile << x.first << " " << x.second << endl;
	}

	savingFile.close();

	cout << endl << "Board has been saved under the name \"" << FileName << "\"." << endl << endl; //infroms the user where its board was saved
}

void Board::Save_ResumeBoard(bool finish) //save the board in a file that already exists (resume board option). The finish flag works like just in Save_NewBoard
{
	/*
	Does the same as the previous function Save_NewBoard, but doesn't need to create a new file, only updates the file were the board was first loaded from.
	*/
	ofstream savingFile;

	savingFile.open(Board_FileName);

	savingFile << Dictionary_FileName << endl << endl;

	for (size_t i = 0; i < lines; i++)
	{
		for (size_t k = 0; k < columns; k++)
		{
			if (finish)
				if (Board_Cells[i][k] == '.')
					Board_Cells[i][k] = '#';

			savingFile << setw(2) << Board_Cells[i][k];
		}

		savingFile << endl;
	}

	savingFile << endl;

	for (const auto & x : Words_on_Board)
	{
		savingFile << x.first << " " << x.second << endl;
	}

	savingFile.close();

	cout << endl << "Board has been saved under the name \"" << Board_FileName << "\"." << endl << endl;
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