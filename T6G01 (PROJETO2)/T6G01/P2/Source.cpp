#include "Board.h"
#include "Dictionary.h"
#include "Player.h"

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

void OpenBoard(string &FileName, ifstream &File) //this function opens a specific file, the board file
{
		while (true)
		{
			cout << "Insert name of the file you want to open. (It must be \"bXXX\", X being numbers.)  "; //briefly describes the format of the file
			
			cin >> FileName;

			if (FileName.length() == 4 && FileName[0] == 'b' && isdigit(FileName[1]) && isdigit(FileName[2]) && isdigit(FileName[3]))
			{ //verifies if the name for the file is valid by checking if there are exactly 4 characters, if the first is 'b' and the last three are numbers
				FileName = FileName + ".txt"; //if the name is valid it hads the extension '.txt' (since it is a text file)

				File.open(FileName); //tries to open the file

				if (!File.is_open()) //if the file doesn't open then it doesn't exist in the program folder
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
				}
				else 
					break;
			}
			else
			{
				setcolor(RED, WHITE);
				cerr << "Invalid name. " << endl; //in this case, it is the insert name that is invalid
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
			}
		}

		if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
			cin.ignore(10000, '\n');
}

bool valid_word(const string &user_word) //checks if a word is valid (it only has letters)
{
	if (user_word == "")
		return false;

	for (size_t i = 0; i < user_word.length(); i++)
	{
		if (!isalpha(user_word[i]))
			return false;
	}

	return true;
}

string randomSynonym(Dictionary & dictionary, Board & board, const string & user_lcd) //function that randomly searches a synonym for the word allocated in a certain position and returns true if found
{
	string real_word, synonym;

	real_word = board.GetWord(user_lcd); //through the lcd, we find the original word

	synonym = dictionary.GetSynonym(user_lcd, real_word); //we call a class function and return the synonym given by it

	return synonym;
}

void ShowClues(Dictionary & dictionary, Board & board, bool first_calling = 0)
{
	static vector <string> LCDs_horizontal = board.GetLCD(1);
	static vector <string> LCDs_vertical = board.GetLCD(0);
	vector <string> clues;
	string lcd, synonym;

	cout << endl << "HORIZONTAL:" << endl;

	for (size_t i = 0; i < LCDs_horizontal.size(); i++)
	{
		lcd = LCDs_horizontal[i];

		if (first_calling)
			randomSynonym(dictionary, board, lcd); //get a synonym for the word held in that lcd

		cout << endl << lcd << "   ";

		clues = dictionary.GetClues(lcd);
		
		for (size_t n = 0; n < clues.size() - 1; n++)
		{
			cout << clues[n] << ", ";
		}

		cout << clues[clues.size() - 1];
	}

	cout << endl << endl << "VERTICAL:" << endl;

	for (size_t i = 0; i < LCDs_vertical.size(); i++)
	{
		lcd = LCDs_vertical[i];

		if (first_calling)
			randomSynonym(dictionary, board, lcd); //get a synonym for the word held in that lcd

		cout << endl << lcd << "   ";

		clues = dictionary.GetClues(lcd);

		for (size_t n = 0; n < clues.size() - 1; n++)
		{
			cout << clues[n] << ", ";
		}

		cout << clues[clues.size() - 1];
	}
}

bool BoardIsFull(Dictionary & dictionary, Board & board, Player & player) //tests if the board is full
{
	char option;
	map<string, string, Board::cmpLCD> correctWords;

	cout << "The board is full, do you want to validate your guesses?(Y/N) ";

	do
	{
		cin >> option;

		if (cin.fail() || !(option == 'Y' || option == 'N'))
		{
			setcolor(RED, WHITE);
			cerr << "Invalid input. Do you wish to validate your guesses?(Y/N)  ";
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
		}
		else break;

	} while (true);

	if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
		cin.ignore(10000, '\n');

	if (option == 'Y') //if he wants to validate
		if (board.FinalVerification(correctWords)) //and he has got it right
		{
			player.StopCountingTime(); //we get the final time
			setcolor(GREEN,WHITE);
			cout << "You are correct!!!" << endl;
			setcolor(WHITE);
			player.SavePlayerInformation(); //and save his information
			cout << endl << endl;

			return false; //we return false to the boolean variable in the function 'void Game(...)': continue_playing, because he finished the board successfully he will not continue playing this particular set
		}
		else //if he got it wrong
		{
			cout << "It seems you've got it wrong... ";
			
			if (correctWords.size() == 0)
				cout << "None of your words was correct" << endl;
			else
			{
				cout << "This are the words you got right:" << endl;

				setcolor(GREEN);
				for (const auto & i : correctWords)
				{
					cout << i.first << "  " << i.second << endl;
				}
				setcolor(WHITE);
			}
				
			cout << "Do you want to continue?(Y/N) "; //we ask him if he wants to continue

			do
			{
				cin >> option;

				if (cin.fail() || !(option == 'Y' || option == 'N'))
				{
					setcolor(RED, WHITE);
					cerr << "Invalid input. Do you want to continue?(Y/N)  ";
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
				}
				else break;

			} while (true); //accepts only Y for yes or N for no

			if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
				cin.ignore(10000, '\n');

			if (option == 'N') //he doesn't want to keep playing
				return false; // so : continue_playing = false
			else 
				return true; // he wants to keep playing : continue_playing = true
		}
	else 
		return true; //if he does not want to validate his guesses, he keeps playing
}

void Game(Dictionary & dictionary, Board & board, Player & player) //main game. Where the puzzle is solved and manipulated in general
{
	string user_lcd, user_word;

	while (true)
	{	
		cout << endl << endl;
		board.Show(); //we show the board

		if (board.IsFull()) //verify if the board is full
		{
			bool continue_playing = BoardIsFull(dictionary, board, player); //if it is full, we call the function BoardIsFull, and receive a boolean value that tells us if the current game will be continued or stopped

			if (!continue_playing) //the current game is stopped 
				return;
		}
		
		bool proceed = 1; //this will be used to determine if it makes sense to ask the second question: "Word?"

		cout << "Position ( LCD / S = show clues / CTRL-Z = stop ) ? ";

		do
		{
			cin >> user_lcd;

			if (cin.eof()) //in this case, if the user wants to leave, the information will not be saved
			{
				cin.clear();

				char option;

				cout << "Every content will be lost, are you sure you want to leave?(Y/N)  ";
				
				do
				{
					cin >> option;

					if (cin.fail() || !(option == 'Y' || option == 'N'))
					{
						setcolor(RED, WHITE);
						cerr << "Please select Yes (Y) or No (N).  ";
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
					}
					else break;

				} while (true);

				if (cin.peek() != '\n') //if by accident the user input has two different words the second will be ignored
					cin.ignore(10000, '\n');

				if (option == 'Y') //the current game is stopped
					return;
				else
				{
					proceed = 0; //the question for position already has been answered but no lcd has been given, so it makes no sense to ask for a word
					break;
				}
			}
			else if (user_lcd == "S")
			{
				ShowClues(dictionary, board);
				proceed = 0;
				break;
			}
			else if (!board.CheckLCD(user_lcd))
			{
				setcolor(RED, WHITE);
				cerr << "Invalid input for position." << endl;
				setcolor(WHITE);
				cout << "Verify if you are using the correct indexes or if they point to the beggining of the word. ";
				cin.clear();
				cin.ignore(10000, '\n');
			}
			else break;

		} while (true);

		if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
			cin.ignore(10000, '\n');

		if (proceed)
		{
			cout << "Word ( R = replace / - = remove / ? = help ) ?  ";

			cin >> user_word;

			if (cin.eof()) //case the user inputs an eof
				cin.clear();
			else if (cin.peek() != '\n') //if by accident the user input has two different words the second will be ignored
				cin.ignore(10000, '\n');

			if (user_word == "R")
			{
				cout << "Word ?  ";

				do
				{
					cin >> user_word;

					if (cin.fail() || !valid_word(user_word))
					{
						setcolor(RED, WHITE);
						cerr << "Introduce a valid input for word." << endl;
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
					}
					else break;

				} while (true);

				if (cin.peek() != '\n') //if by accident the user input has two different words the second will be ignored
					cin.ignore(10000, '\n');

				dictionary.EverythingToUpper(user_word);

				board.Replace(user_lcd, user_word);
			}
			else if (user_word == "-") //user wants to remove the word that begins in the position 'user_lcd'
			{
				board.Remove(user_lcd);
			}
			else if (user_word == "?") //user wants a extra synonym for the word that begins in the position 'user_lcd'
			{
				string synonym;

				synonym = randomSynonym(dictionary, board, user_lcd); //if a synonym was given

				if (synonym == "")
					cout << "There are no more synonyms for this word." << endl;
				else
				{
					cout << synonym << endl;
					player.IncrementClues();  //the extra clue was registered and will appear on the players stats
				}
			}
			else //user wants to write a word
			{
				if (!valid_word(user_word)) //the user is warned if the input wasn't a valid word
				{
					setcolor(RED, WHITE);
					cerr << "Invalid word." << endl;
					setcolor(WHITE);
				}
				else
				{
					dictionary.EverythingToUpper(user_word); //we make the word upper case

					board.Insert(user_lcd, user_word); //and insert it in the board
				}
			}
		}
	}
}

void ExtractPuzzle() //extracts a selected puzzle to play
{
	Dictionary dictionary; //this is an object of the class Dictionary, that will define the dictionary that is going to be used
	Board board; //this is an object of the class Board, that will define the board that is going to be used
	Player player; //this is an object of the class Player, which will contain the user's information
	string FileName, PlayerName; 
	ifstream BoardFile, DictionaryFile; //These two will be used to read the contents of a board and a dictionary

	cout << "What is your name? (no spaces allowed) ";

	do
	{
		cin >> PlayerName;

		if (cin.fail())
		{
			cerr << "Invalid input for name. Please introduce it again.  ";
			if (cin.eof()) //in case the user enters an eof
			{
				cin.clear();
			}
			else
			{
				cin.clear();
				cin.ignore(10000, '\n'); //cleaning the buffer
			}
		}
		else break;

	} while (true);

	if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
		cin.ignore(10000, '\n');

	OpenBoard(FileName, BoardFile); //we open the board, this is a constructed function in this source.cpp

	player.NewPlayer(PlayerName, FileName); //Create a new player, remember that the FileName holds the name of the board file, for now

	getline(BoardFile, FileName); //collects the name of the dictionary file that is writen on the first line of the board file, now the variable FileName holds the name of the dictionary file

	DictionaryFile.open(FileName); //opens normally using a function that belongs to the compilator

	if (!DictionaryFile.is_open()) //if it doesn't open, then the dictionary file does not exist in the program's folder, it is necessary to shut the program so the user can put that file in the folder
	{
		setcolor(RED, WHITE);
		cerr << "Dictionary file not found." << endl;
		setcolor(WHITE);
		cout << "The dictionary used to create the board wasn't found. Make sure it exists and try again." << endl;
		return;
	}

	cout << "Text file is loading...Please wait." << endl << endl;

	dictionary.ExtractWords(DictionaryFile); //extract the words from the dictionary file
	DictionaryFile.close(); //close the dictionary file

	board.InitiateBoard(BoardFile); //initiate the board that was saved in the board file

	ShowClues(dictionary, board, 1);

	player.StartCountingTime(); //after showing the player the clues, we capture the initial time

	Game(dictionary, board, player); //start playing
}

void ProgramInterface() //shows the main menu to the user
{
	int option;  //user input to select what the user wants to do

	cout << "CROSSWORDS PUZZLE" << endl
		<< "============================================" << endl << endl;

	//instructions given to the user
	cout << "INSTRUCTIONS:" << endl
		<< "This is a crossword solving game." << endl
		<< "You are going to be given clues (synonyms) for each word originally writen on the board and you must guess the word." << endl
		<< "- To guess a word use the indexes shown in the order: Line(upper) Column(lower) Direction(V or H), and make sure they point to the beggining of a word." << endl 
		<< "- You may ask for more clues, but those extra clues will be counted and appear on your stats." << endl
		<< "- Time is ticking and will appear on your stats too." << endl 
		<< "- You can remove and replace a previously inserted word." << endl
		<< "- And most important, have fun :) " << endl;

	while (true)
	{
		cout << endl << "-----------------------" << endl
			<< "Options:" << endl
			<< "1 - Play" << endl
			<< "0 - Exit" << endl
			<< "Option ? ";

		do
		{
			cin >> option;

			if (cin.fail() || option < 0 || option > 1) //only allowed to move forward when the user writes a valid input
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
			}
			else break;

		} while (true);

		if (cin.peek() != '\n') //if by accident the user input had two different numbers or words the second will be ignored
			cin.ignore(10000, '\n');
		
		cout << "-----------------------" << endl;

		switch (option)
		{
		case 0: 
			exit(0);
		case 1:
			ExtractPuzzle();
			break;
		}
	}
}

int main()
{
	srand((unsigned)time(NULL)); //we declare this so the random numbers that will be needed are always different

	ProgramInterface();

	system("pause");

	return 0;
}