#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

class Player
{
public:
	Player(); //player constructor by default
	~Player();
	void NewPlayer(const string & name, const string & board); //saves information about the player
	void StartCountingTime(); //registers initial time
	void IncrementClues(); //increments the number of clues asked by the player
	void StopCountingTime(); //registers final time
	void SavePlayerInformation(); //saves the player's information into a text file for the designated board

private:
	string Name_of_player; //name of the player
	string BoardName; //contains the name of the board that the player is currently using
	unsigned int num_alternative_clues; //extra clues asked by the user
	time_t time_beggining, time_final; // times registered by the functions StartCountingTime() and StopCountingTime()
	string DiffTime(); //function that calculates the difference between the initial time and the final time and converts it into a string
};

#endif