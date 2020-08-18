#include <iostream>
#include "Board.h"

int main()
{
	Board myBoard;
	string user_lcd, user_word;
	unsigned int user_lines, user_columns;
	
	cout << "lines columns??"; cin >> user_lines >> user_columns;

	myBoard.Create(user_lines, user_columns);

	myBoard.Show();

	while (true)
	{
		cout << "Line Column Direction: "; cin >> user_lcd;

		if (cin.eof())
			exit(0);

		cout << "Insert word: "; cin >> user_word;

		if (user_word == "-")
		{
			myBoard.Remove(user_lcd);
			myBoard.Show();
		}
		else
		{
			myBoard.Insert(user_lcd, user_word);
			myBoard.Show();
		}
	}

	system("pause");
	return 0;
}