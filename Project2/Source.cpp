#include <iostream>
#include <ios>
#include <limits>
#include "Dictionary.h"
#include "Board.h"

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
//==========================================================================================
// Set text color
void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
//==========================================================================================
// Set text color & background
void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}

void OpenFile(string &FileName, ifstream &File) //opens the file
{
	File.open(FileName); //open the text file (dictionary)

	while (!File.is_open()) //only allowed to move forward when the text file is opened
	{
		setcolor(RED, WHITE);
		cerr << FileName << " not found, please verify it and introduce it again." << endl;
		setcolor(WHITE);
		if (cin.eof()) //in case the user enters an eof
		{
			cin.clear();
		}
		else
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		cin >> FileName;

		File.open(FileName);
	}
}

void SaveCrosswords(Board board, bool resume_board = 0) //saves the crosswords puzzle. When resume_board is true the saving file is already defined.
{
	int option; //user input used to select what the user wants to do

	cout << endl;
	cout << "Do you wish to" << endl
		<< "(1) fill the white cells on your current board and save," << endl
		<< "(2) save it as it is to finish it later," << endl
		<< "(3) leave to main menu?" << endl
		<< "(insert 1, 2 or 3)  ";
	cin >> option;
	cout << endl;

	while (cin.fail() || option < 1 || option > 3) //only continues when the user inputs a valid value
	{
		setcolor(RED, WHITE);
		cerr << "Invalid input. Insert a valid option.  ";
		setcolor(WHITE);
		if (cin.eof()) //in case the user enters an eof
		{
			cin.clear();
		}
		else
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		cin >> option;
	}

	if (cin.peek() != '\n') //if by accident the user input has two different numbers the second will be ignored
		cin.ignore(10000, '\n');

	switch (option)
	{
	case 1: //the board will be filled with black cells on its white spaces
	{
		if (resume_board)
			board.Save_ResumeBoard(1);
		else
			board.Save_NewBoard(1);
		return;
	}
	case 2: //the board will be saved as it is
	{
		if (resume_board)
			board.Save_ResumeBoard(0);
		else 
			board.Save_NewBoard(0);
		return;
	}
	case 3: return;
	}
}

void Help(Board board, Dictionary dictionary, string user_word, string user_lcd) 
{
	user_word = board.Help(); //string that holds the content of the board from the given lcd
	board.CheckLCD(user_lcd); //used to reset l, c and d in the board class

	if (user_word == "") //when user_word is null a warning is sent to the screen
	{
		setcolor(RED, WHITE);
		cerr << "It is not possible to write a word starting from that position.";
		setcolor(WHITE);
	}
	else
	{
		string word_to_search = ""; //string to temporary hold parts of the user_word
		vector <string> matches; //vector containing the matching words
		bool no_matches = 1; //flag that is true when there were no matches and false otherwise

		//in order to check every possible matching we need to check not only words of the same length as the spaces on board but words with all suitable lengths
		for (size_t i = 0; i < user_word.length(); i++)
		{
			word_to_search = word_to_search + user_word[i];

			if (user_word[i] == '.') //for every dot it searches the dictionary for compatible words
			{
				matches = dictionary.MatchingWords(word_to_search);

				for (size_t n = 0; n < matches.size(); n++)
				{
					if (board.CheckSize(matches[n]) && board.CheckMatching(matches[n]) && board.CheckExistance(matches[n]) && board.CheckParallelism(matches[n])) //displays only the matches that can fit in the board on the given position
					{
						no_matches = 0;
						cout << matches[n] << endl;
					}
				}
			}
		}

		if (no_matches) //when there are no matches the user is informed
			cout << "No matches available." << endl;
	}
}

void Game(Dictionary dictionary, Board board, bool resume_board = 0) //main game. Where the puzzle is built and manipulated in general
{
	string user_lcd, user_word; //inputs from the user

	while (true)
	{
		cout << endl << endl;
		board.Show();

		cout << "Position ( LCD / CTRL-Z = stop ) ? ";
		cin >> user_lcd;

		while (!board.CheckLCD(user_lcd) && !cin.eof()) //only allowed to move forward when a valid lcd or a cin.eof() is given
		{
			setcolor(RED, WHITE);
			cerr << "Invalid input for position." << endl;
			setcolor(WHITE);
			cout << "Verify if you are using the indexes shown in the order: Line(upper) Column(lower) Direction(V or H).  ";
			cin.clear();
			cin.ignore(10000, '\n');
			cin >> user_lcd;
		}

		if (cin.peek() != '\n') //if by accident the user input has two different words the second will be ignored
			cin.ignore(10000, '\n');

		if (cin.eof()) //when user wants to leave it will be asked to save the board
		{
			cin.clear();
			SaveCrosswords(board, resume_board);
			return;
		}

		cout << "Word ( - = remove / ? = help ) ? ";
		cin >> user_word;

		if (cin.eof()) //case the user inputs an eof 
			cin.clear();
		else if (cin.peek() != '\n') //if by accident the user input has two different words the second will be ignored
			cin.ignore(10000, '\n');

		if (user_word == "-") //user choosed to remove a word
		{
			board.Remove(user_lcd);
		}
		else if (user_word == "?") //user wants help
		{
			Help(board, dictionary, user_word, user_lcd);
		}
		else //user wants to insert a word
		{
			dictionary.EverythingToUpper(user_word); //sets all letters to uppercase

			if (dictionary.WordExists(user_word)) //word is on the dictionary
			{
				board.Insert(user_lcd, user_word);
			}
			else //word isn't on the dictionary
			{
				setcolor(RED, WHITE);
				cerr << "Invalid word." << endl;
				setcolor(WHITE);
			}
		}
	}
}

void CreatePuzzle() //handles the displays and procedures when the user wants to create a board from scratch
{
	Dictionary dictionary; //the dictionary to be used
	Board board; //the board where the user will write
	string dictionaryFileName; //name of the dictionary file
	ifstream dictionaryFile;
	unsigned int num_lines, num_columns; //number of lines and columns for the board

	cout << "--------------------------------------------" << endl
		<< "CREATE PUZZLE" << endl
		<< "--------------------------------------------" << endl;

	//creating the dictionary

	cout << "Dictionary file name ? ";
	cin >> dictionaryFileName; //collects the file name of input (dictionary)

	OpenFile(dictionaryFileName, dictionaryFile);

	if (cin.peek() != '\n') //if by accident the user input had two different words the second will be ignored
		cin.ignore(10000, '\n');

	cout << "Text file is loading...Please wait." << endl << endl;

	dictionary.ExtractWords(dictionaryFile); //extracts words from the dictionaryFile
	dictionaryFile.close();

	// creating the board 

	cout << "Board size ? (lines columns, max. 26) ";
	cin >> num_lines >> num_columns;

	while (cin.fail() || num_lines > 26 || num_columns > 26) //only allows to continue if lines and collumns aren't greater than 26 
	{
		setcolor(RED, WHITE);
		cerr << "Introduce a valid input for lines and columns. ";
		setcolor(WHITE);
		if (cin.eof()) //in case the user enters an eof
		{
			cin.clear();
		}
		else
		{
			cin.clear();
			cin.ignore(10000, '\n');
		}
		cin >> num_lines >> num_columns;
	}

	if (cin.peek() != '\n') //if by accident the user input had more than three numbers, after the second all will be ignored
		cin.ignore(10000, '\n');

	board.Create(num_lines, num_columns, dictionaryFileName); //create a new board to start working on it

	Game(dictionary, board); //start the game
}

void ResumePuzzle() //handles the displays and procedures
{
	Dictionary dictionary; //the dictionary to be used
	Board board; //the board where the user will write
	string boardFileName, dictionaryFileName; //the board's and dictionary's file names
	ifstream dictionaryFile, boardFile;

	cout << "--------------------------------------------" << endl
		<< "RESUME PUZZLE" << endl
		<< "--------------------------------------------" << endl;

	//loading board
	cout << "Board file name? (please introduce the extension \".txt\")  ";
	cin >> boardFileName; //collects the file name of input (board)

	OpenFile(boardFileName, boardFile);

	if (cin.peek() != '\n') //if by accident the user input had two different words the second will be ignored
		cin.ignore(10000, '\n');

	getline(boardFile, dictionaryFileName); //collects the file name of input (dictionary), now the variable FileName holds the name of the dictionary file

	OpenFile(dictionaryFileName, dictionaryFile); 

	cout << "Text file is loading...Please wait." << endl;

	dictionary.ExtractWords(dictionaryFile); //extracts the words from the dictionary
	dictionaryFile.close(); 

	board.ResumeBoard(boardFile, boardFileName, dictionaryFileName); //extracts the words present in the board
	boardFile.close(); 

	Game(dictionary, board, 1); //start game
}

void ProgramInterface() //shows the main menu to the user
{
	int option; //user input to select what the user wants to do

	//instructions given to the user
	cout << "CROSSWORDS PUZZLE CREATOR" << endl
		<< "============================================" << endl << endl
		<< "INSTRUCTIONS:" << endl
		<< "- This program let's you build crosswords freely. All you need to introduce is a dictionary file." << endl
		<< "- You may choose how many lines and columns the board will have (having a maximum of 26 each)." << endl
		<< "- To write on the board it will appear Position( LCD / CTRL + Z = stop ), whereas LCD stands for Line Column and Direction, the indexes of the line are marked in the left side of the board," << endl
		<< "the indexes of the column are marked on top of the board, you should introduce it as they are seen: lines in upper case, columns in lower case. " << endl
		<< "This first two letters for lines and columns demark the spot where the first letter of the word will be written. The direction may be V (for vertical) or H (for horizontal)." << endl
		<< "- Then, it will ask you for a word, it will only accept it if the word exists on the dictionary you have given." << endl
		<< "- After inserting a certain position in the first question ( Position ? ), when asked for a word, you may also introduce '-' (hiphen) or '?' (exclamation point)." << endl
		<< "The first means you want to remove the word you have introduced before in the position." << endl
		<< "The second gives you a list of words that fit in the position you have required, taking in consideration the availability of the board." << endl
		<< "- If you want to leave in the middle of building the board you should press 'Ctrl + Z' in answer to ( Position ? )." << endl
		<< "You will then be asked if you want to finish the board, turning all dots into black cells ('#'), or if you want to save your current word. You can also choose to discard your board." << endl
		<< "--------------------------------------------" << endl;

	while (true)
	{
		cout << "Options:" << endl
			<< "1 - Create Puzzle" << endl
			<< "2 - Resume Puzzle" << endl
			<< "0 - Exit" << endl
			<< "Option ? ";

		cin >> option;

		cout << "--------------------------------------------" << endl;

		while (cin.fail() || option < 0 || option > 2) //only allowed to move forward when the user writes a valid input
		{
			setcolor(RED, WHITE);
			cerr << "Invalid input. Choose an option from the menu!" << endl;
			setcolor(WHITE);
			if (cin.eof()) //in case the user enters an eof
			{
				cin.clear();
			}
			else
			{
				cin.clear();
				cin.ignore(10000, '\n'); //cleaning the buffer
			}
			cin >> option;
		}

		if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
			cin.ignore(10000, '\n');

		switch (option)
		{
		case 0: 
			exit(0);
		case 1:
			CreatePuzzle();
			break;
		case 2:
			ResumePuzzle();
			break;
		}
	}
}

int main()
{
	ProgramInterface();

	system("pause");

	return 0;
}