FEUP - MIEIC
Programação - Projeto 2
T6G01
16/05/2018

-> cwcreator

The first part of the project (cwcreator) is meant for the construction of crosswords, using boards with a maximum number of 26 columns and lines. It allows you to create a new board or to continue developing a unfinished one.
In the first case, when saving the board, we create a new, automatic, file, that goes under the format bXXX.txt, never overwriting the already existing ones. To avoid conflict, 999 files like this one are supported (from b001 to b999), if a user wishes to save one more file, it is obligated to erase one that has been created before.
In the second case (resume), a board already exists, so you can modify it as you intent to and, when saving, it will save under the same name it had in the beggining.
In both situations, the following possibilities are presented to the user: finish (in which all empty cells (considered to be the ones who hold the character '.') are closed (a hash is inserted in that position), becoming "black cells"), save it as it is so the user can finish it later or to leave without saving. For this possibilities to come up, the user must answer with the key 'Ctrl+Z' to the question "Position?". Only to this situation does this key produce any effect, for to any other situation the program has been prepared to deal with invalid inputs.
In order to grant the user with more "free-will", per say, we found not necessary the verification for when the board is full. The user will finish the board when he finds it suitable to do so. Furthermore, we also found it not useful to have a final verification, because intermedium methods have been implemented to verify the validity of the construction.
Finally, after much consideration, we decided to build a type of crosswords (there many types of this kind of puzzle) that does not allow to insert words whereas there are already, in the same direction, adjacent lines or columns. We intend, for a future development of this project, to improve it to the type where it allows to write words in adjacent lines or columns solely when it constructs a valid word.


-> cwplayer

The second part of the project (cwplayer) focus on solving the crosswords that were previously created and finished with the "cwcreator" program, on a more interactive style. 
A set of clues (which are randomly picked synonyms for the words that appear in the announced positions) will be shown and more clues may be requested by the user, which, when successfully presented to him, increment the number of aditional clues. If the user answers "S" to the "Position?" question, a list of all the clues (initial and extra) will be shown to him. 
The player is also allowed to remove (by inserting '-') or replace (by inserting "R") words after specifying the position. 
When the board is full, the player will be asked if he wants to validate his answers, if he does, it will verify if every word that was insert by him is correct and in the right position, if it his, the player's information (username, number of extra clues asked and time he took to finish) will be saved in a file (bXXX_p.txt), if not, the program will find which words are correct and those will appear in color green.