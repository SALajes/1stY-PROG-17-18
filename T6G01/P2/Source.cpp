//program 2

#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<vector>
#include<iomanip>
#include<algorithm>

using namespace std;

void OpenFile(string fileName, ifstream &fileStream) //function that opens the file
{
	cout << "Insert the dictionary's file name: ";
	cin >> fileName;

	fileStream.open(fileName);

	while (!fileStream.is_open()) //it only let's you move on when the file is valid and it opens
	{
		cerr << "The file was not found. Please verify it and try again: ";
		cin.ignore(10000, '\n');
		cin >> fileName;

		if (cin.eof())
			exit(0);

		fileStream.open(fileName);
	}
}

void EverythingToUpper(string &word) // this function simply puts all letters in upper case, assuming all of them are letters
{
	size_t i = 0;

	while (i < word.length())
	{
		if (islower(word[i])) //if the letter is not upper, it makes it upper
		{
			word[i] = toupper(word[i]);
		}

		i++;
	}
}

void ExtractWords(ifstream &dictionary, vector<string> &wordList) //extracts the words of the text file into a vector of words
{
	string word;

	while (getline(dictionary, word))
	{
		EverythingToUpper(word); //makes sure every letter is upper before entering the vector
		wordList.push_back(word); //add the word to the end of the vector of words that begin by the same letter
	}
}

void setLetterList(const vector<string> &wordList, vector<char> &LetterList) //prepares the list from where the letters are going to be extracted for game4
{
	vector<int> letters_quantity(26); //registers the frequency of appearance of each letter, A being index 0
	size_t wordList_index = 0, letters_quantity_index, minimum_letter_index = 0;

	while (wordList_index < wordList.size()) //goes through every word on the vector
	{
		string word = wordList[wordList_index];

		for (size_t word_index = 0; word_index < word.length(); word_index++) //goes through every letter
		{
			letters_quantity_index = (int)word[word_index] - (int)'A'; //discovers what is the index that corresponds to the letter (remember A is index 0)

			letters_quantity[letters_quantity_index] = letters_quantity[letters_quantity_index] + 1; //increments the number that is there (number of appearances of the letter)
		}

		wordList_index++;
	}

	for (letters_quantity_index = 0; letters_quantity_index < 26; letters_quantity_index++) //discovers what is the index where the number is smaller (frequency of the letter) is lower
	{
		if (letters_quantity[letters_quantity_index] < letters_quantity[minimum_letter_index] && letters_quantity[letters_quantity_index] != 0)
			minimum_letter_index = letters_quantity_index;
	}

	/*
	decides how many times each letter is going to appear on the list based on the following algorithm:
	if the letter that appears less (let's say it appears x times) has to appear 2 times in the set list,
	then the other words  have to appear (y * 2) / x   times, y being the total number of appearances of each word
	*/
	for (letters_quantity_index = 0; letters_quantity_index < 26; letters_quantity_index++)
	{
		int num_appearance = letters_quantity[letters_quantity_index] * 2 / letters_quantity[minimum_letter_index]; //number of times the letter that corresponds that index has to appear in the set list
		char letter = (char)(letters_quantity_index + (int)'A'); //letter that corresponds that index

		for(int i = 1; i <= num_appearance; i++)
		{
			LetterList.push_back(letter); //put the letter that number of times in the vector
		}
	}
}

bool checkWord(string &word, const vector<string> &wordList) //verifies if a word is in the word list (vector)
{
	if (binary_search(wordList.begin(), wordList.end(), word)) //uses fast binary search
		return true;
	else
		return false;
}

void testInput(string &word, unsigned int flag = 0)
{
	bool validInput;

	if (cin.eof())
		exit(0);

	if (flag) //the flag just marks if we are using wildcards or not, by default we are not.
	{
		do //only used for the 5th game, in this case we allow the user to write letters and wildcards (? and *)
		{
			validInput = true;
			for (size_t i = 0; i < word.length(); i++)
			{
				if (!((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z') || word[i] == '?' || word[i] == '*'))
				{
					validInput = false;
					cerr << "Invalid input. Please insert a word with wildcards: ";
					cin >> word;
				}
			}
		} while (!validInput);
	}
	else //in this simpler case, used for most of all the other inputs, we only allow words composed by letters
	{
		do
		{
			validInput = true;
			for (size_t i = 0; i < word.length(); i++)
			{
				if (!((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z')))
				{
					validInput = false;
					cerr << "Invalid input. Please insert only letters: ";
					cin >> word;
				}
			}
		} while (!validInput);
	}

	EverythingToUpper(word); //if the word is valid, we make everything upper
}

//////////////////////////////////////////////////////////////////////////
// WildcardMatch
// str - Input string to match
// strWild - Match mask that may contain wildcards like ? and *
//
// A ? sign matches any character, except an empty string.
// A * sign matches any string inclusive an empty string.
// Characters are compared caseless.
//
// ADAPTED FROM:
// https://www.codeproject.com/Articles/188256/A-Simple-Wildcard-Matching-Function
//////////////////////////////////////////////////////////////////////////

bool wildcardMatch(const char *str, const char *strWild)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

vector<string> PermutationVector (const vector<string> &wordList, string userLetters) //returns a vector with all the words in the word list that can be built with a set of letters
{
	size_t letterPos;
	string currentWord;
	vector<string> final_vector;

	for (size_t i = 0; i < wordList.size(); i++)
	{
		currentWord = wordList[i];

		if (currentWord.length() == userLetters.length()) // the number of letters given and the number of letters in the word have to be the same
		{
			for (size_t j = 0; j < userLetters.length(); j++) 
			{
				letterPos = currentWord.find(userLetters[j]);

				if (letterPos == string::npos)// if one of the letters can't be found, in the word, than it moves on to the next one
					break;
				else
				{
					currentWord.erase(letterPos, 1);

					if (j + 1 == userLetters.length()) //if it has searched the entire word, and passed on the tests, it adds it to the vector
					{
						final_vector.push_back(wordList[i]);
					}
				}
			}
		}
	}

	return final_vector; //vector that contains all the words that can be built solely with the given set of letters
}

void game1(const vector<string> &wordList)
{
	string word;

	cout << "What word do you want to check on the dictionary? ";
	cin >> word;

	testInput(word); //it tests if the word is valid

	if (checkWord(word, wordList)) //verifies if the word appears on the word list
	{
		cout << "It does exist!!!" << endl;
	}
	else
		cout << "It doesn't exist..." << endl;
}

void game2(const vector<string> &wordList)
{
	int i = rand() % wordList.size();
	string word = wordList[i], word_copy = wordList[i]; //chooses a word from the list and creates a copy of it, this is necessary because the chosen word will be scrambled, so it's necessary to save the original word
	string guess;
	int tries = 2 + PermutationVector(wordList, word).size();; //depending on how many words can be made with that set of letters, it defines how many tries the user has to guess the word (having at least 3)

	random_shuffle(word.begin(), word.end()); //scrambles the word

	cout << "Try to guess what word is this:  ";

	for (size_t i = 0; i < word.length(); i++)
		cout << word[i] << " ";

	cout << endl;

	while (true)
	{
		cout << "You have " << tries << " tries" << endl;

		cin >> guess;

		testInput(guess); //validates the word

		tries--;

		if (guess == word_copy) //avaliates if the user's guess is the same as the original word
		{
			cout << endl << "=========================" << endl
				<< "== GOOD JOB! YOU WON!! ==" << endl
				<< "=========================" << endl;

			break;
		}
		else if (tries == 0) //the user looses when he runs out of tries
		{
			cout << "You've lost :(" << endl
				<< "The word was " << word_copy << endl;
			break;
		}
	}
}

void game3(const vector<string> &wordList)
{
	string userLetters;
	vector<string> GameVector;

	cin.ignore(1000, '\n'); // since cin lets '\n' in the buffer, we have to ignore it to procede with getline
	cout << "Insert some letters and let's see which words can be built with them: ";
	getline(cin, userLetters); 
	testInput(userLetters);//validates the input

	cout << "Your letters were: " << userLetters << endl;

	//calls the PermutationVector function that returns a vector and saves that vector in a variable to be used in this function
	GameVector = PermutationVector(wordList, userLetters); 

	//the vector is filled with all the words that can be made using that set of letters, and is now going to show them
	for (size_t i = 0; i < GameVector.size(); i++)  
		cout << GameVector[i] << endl;

	cout << "Found " << GameVector.size() << " words! :D" << endl;
}	

void game4(const vector<string> &wordList, const vector<char> &LetterList)
{
	int num_letters = rand() % 4 + 3; //selects the quantity of letters that are going to be given to the user, from 3 to 7
	string availableCharacters;
	string word;
	int j;

	cout << "Build a valid word using only the following letters: ";

	do
	{
		for (int i = 0; i < num_letters; i++)
		{
			j = rand() % LetterList.size(); //j is a random index from 0 to the size of the letter set list, so it can choose a random letter

			availableCharacters = availableCharacters + LetterList[j] + "  "; //creates a string to hold the letters that the user can use
		}
	} while (availableCharacters.find_first_of("AEIOU") == string::npos); //garantee there is a vowel, because no word can be built without vowels

	cout << availableCharacters << endl;

	while (true) //this cicle only allows the user to move forward when the user inserts a valid word (containing only the characters that were given)
	{
		cin >> word;

		testInput(word);//searches the word for invalid inputs (not letters)

		if (word.find_first_not_of(availableCharacters) != string::npos) //searches the word for letters that were not given
			cout << "Not a valid word. Use only the characters that were given." << endl;
		else if (word.length() == 1)
			cout << "It's not fair to just write one of the letters down!" << endl; //if the user only writes one of the letters it doesnt count
		else break; //if it passes the tests, it moves forward
	}

	if (checkWord(word, wordList)) //verifies if the word is on the list
		cout << "The word you've built is on the list!!! :)" << endl << endl;
	else cout << "The word you've built is not on the list... :(" << endl << endl;
}

void game5(const vector<string> &wordList)
{
	string word, currentWord;
	unsigned int counter = 0;

	cout << "Insert a word with some wildcards to check which words match it." << endl
		<< "(A ? sign matches any character, except an empty string)" << endl
		<< "(A * sign matches any string inclusive an empty string)" << endl
		<< "Your input: ";
	cin >> word;

	testInput(word, 1);
	cout << "So... this ones match your input: " << endl;

	for (size_t i = 0; i < wordList.size(); i++)
	{
		if (wildcardMatch(wordList[i].c_str(), word.c_str()))
		{
			cout << wordList[i] << endl;
			counter++;
		}
	}

	cout << "And it seems that's all! You found " << counter << " words :)" << endl;
}

void WordGames(const vector<string> &wordList, vector<char> &LetterList)
{
	unsigned int option;

	while (true)
	{
		//menu
		cout << endl << "What would you like to do?" << endl
			<< "1 - Check if a word is in the list!" << endl
			<< "2 - Guess the scrambled word!" << endl
			<< "3 - Give me a set of letters!" << endl
			<< "4 - I give you a set of letters!" << endl
			<< "5 - Wildcard Characters!" << endl 
			<< "(Ctrl+Z / Cmd+Z to Exit)" << endl << endl;

		cin >> option;

		if (cin.eof()) 
			exit(0); //control to exit the game

		while (cin.fail() || option < 1 || option > 5) //only  allowed to move forward when the user writes a valid input
		{
			cerr << "Invalid input. Choose an option from the menu!" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
			cin >> option;

			if (cin.eof())
				exit(0);
		}

		switch (option) //calls the game according to the option
		{
		case 1:
			game1(wordList);
			break;
		case 2:
			game2(wordList);
			break;
		case 3:
			game3(wordList);
			break;
		case 4:
			game4(wordList, LetterList);
			break;
		case 5:
			game5(wordList);
			break;
		}
	}
}

int main()
{
	string fileName; //name of the text file to be opened
	ifstream dictionary; //text file stream
	vector<string> wordList; //vector which will contain all the words that exist in the file
	vector<char> LetterList; //vector which works as a set of letters for game 4 

	srand((unsigned)time(NULL)); // some games need to have a random number, this small function garantees the number is always different

	cout << "===== Welcome to \"Playing With Words\"! ======" << endl << endl;

	do
	{
		OpenFile(fileName, dictionary); //open the file

		ExtractWords(dictionary, wordList); //extract words from the text file to the vector wordList

		dictionary.close(); //close the file

		if (wordList.size() == 0) //verifies if the file is empty to warn the user
			cerr << "File is empty.";
	} while (wordList.size() == 0); //if the file is empty the user has to introduce it again

	setLetterList(wordList, LetterList); //builds a useful list that is going to be used in some games

	WordGames(wordList, LetterList); //calls the interface of the game

	return 0;
}