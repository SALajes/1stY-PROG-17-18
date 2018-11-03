#include "Player.h"



Player::Player()
{
}

Player::~Player()
{
}

void Player::NewPlayer(const string & name, const string & board) //saves information about the player
{
	Name_of_player = name;
	BoardName = board; //name of the board that is being used
	num_alternative_clues = 0; //reset the number of extra clues
}

void Player::StartCountingTime() //registers initial time
{
	time(&time_beggining);
}

void Player::IncrementClues() //increments the number of clues asked by the player
{
	num_alternative_clues++;
}

void Player::StopCountingTime() //registers final time
{
	time(&time_final);
}

void Player::SavePlayerInformation()  //saves the player's information into a text file for the designated board
{
	ofstream Board_pFile;
	ostringstream information;

	BoardName = BoardName.substr(0,4) + "_p.txt"; //appends the sufix '_p' and the extension '.txt' to the name of the board

	Board_pFile.open(BoardName, ios::app); //opens a new file or an existing one with the option to append to end of file

	information << "Username: " <<  Name_of_player << "  /  Number of extra clues: " << num_alternative_clues << "  /  " << DiffTime() << endl; //appends the information of the player in that file

	Board_pFile << information.str(); //sends to the file the information

	cout << "Your game statistics were save to " << BoardName << ". Go ahead and check every player's stats!"; //informs the user where his information was saved
}

string Player::DiffTime() //function that calculates the difference between the initial time and the final time and converts it into a string
{
	double seconds; 
	int hours = 0, minutes = 0;
	ostringstream time; 

	seconds = difftime(time_final, time_beggining); //difference between final time and initial time in seconds

	hours = seconds / 3600; //convert into hours (integer number)
	seconds = seconds - hours * 3600; //seconds that remain

	minutes = seconds / 60; //convert into hours the seconds that remain (integer number)
	seconds = seconds - minutes * 60; //remaining seconds

	time << "Time: " << hours << "h " << minutes << "m " << seconds << "s"; //save it in a stringstream

	return time.str(); //convert and return a string 
}