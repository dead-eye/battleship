/*Name: Matthew Stewart
  Date: December 4, 2013
  Program: Final Program - Battleship
  Description:
    This program will allow a single player to play a game of Battleship
	against the computer or  two human players to play against each other.
	Each player must create a player account, which the computer will save
	to a file.  A player must be validated before beginning a single-player
	game and two players must be validated before beginning a two-player
	game.  The second player in a one-player game will be the computer,
	for which game stats will be kept, just as a human player.  Once the
	player(s) have been logged in and validated, each will place their
	ships on the board.  The opposing player's game board will be hidden
	during the ship placement period.  As ships are placed on the board,
	improper ship placement, which would result in ships being placed out-
	of-bounds, will be rejected.  After both players have placed their
	ships on the board, one of the players is randomly chosen to go first.
	Each player takes turns to choose coordinates on the opposing game
	board.  When a ship has been hit it is indicated with an "X" on the 
	game board and a miss is indicated with an "O".  When a ship is sunk,
	an "X" appears next to that player's ship, shown between the two game
	boards.  Once one player has all of his/her ships sunk, the other player
	is announced as the winner and the game stats are updated in the
	playerdata file.  After the game is over, the players are prompted for
	a rematch or to quit the game.  In a single-player game, all of the 
	player 2 functions are carried out through the use of the random function.
*/


#include <iostream>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <fstream>
#include <conio.h>
#include <cctype>
#include <cstring>
#include "utilities.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

//declare global constants
const int ARRAY_SIZE = 100,					//maximum of 100 players in the player data file
		  PASSWORD_WIDTH = 16,				//maximum password length of 15 chars (plus 1 null char)
		  ROWS = 9,				    		//setting up the game board as a 9 x 9, 2-dimensional array
		  COLS = 9,                         //
		  CARRIER_SIZE = 5,					//Aircraft Carriers are 5 spaces long
		  BATTLESHIP_SIZE = 4,				//Battleships are 4 spaces long
		  SUBMARINE_SIZE = 3,				//Submarines are 3 spaces long
		  DESTROYER_SIZE = 3,				//Destroyers are 3 spaces long
		  PATROLBOAT_SIZE = 2;				//Patrol Boats are 2 spaces long

//struct declaration
struct UserAcct
{
	string userName;
	string password;
	string firstName;
	int    wins;
	int    losses;
	double winPercent;
};

//function prototypes
void showSplash();
void accountSettings(fstream &, UserAcct[], char &, int &);
void onePlayerGame(fstream &, char &, int, UserAcct[]);
void twoPlayerGame(fstream &, char &, int, UserAcct[]);
void showBoard(int [][COLS], int [][COLS], bool, bool, string, string,
			   int, int, int, int, int, int, int, int, int, int);
void createNewAcct(fstream &, UserAcct[], char &, int &);
void editAcct(fstream &, UserAcct[], char &, int &);
void deleteAcct(fstream &, UserAcct[], char &, int &);
void showUsers(UserAcct[], char &, int );
void placeShip(int [][COLS], int [][COLS], bool, bool, string, string, string, int,
			   int, int, int, int, int, int, int, int, int, int);
void autoPlaceShip(int [][COLS], int [][COLS], bool, bool, string, string, string, int,
				   int, int, int, int, int, int, int, int, int, int);
void showHitMiss(char, char, int, int, int, int [][COLS], int [][COLS]);
char showMainMenu();
char showAcctMenu();
char showRowLetter(int);
char showHidden(int);
char showVisible(int);
char getOrientation(int [][COLS], int [][COLS], bool, bool, string, string, string,
			   int, int, int, int, int, int, int, int, int, int);
char getRowLocation(int [][COLS], int [][COLS], bool, bool, string, string, string, char,
			   int, int, int, int, int, int, int, int, int, int);
char getColLocation(int [][COLS], int [][COLS], bool, bool, string, string, string, char,
			   int, int, int, int, int, int, int, int, int, int);
char getRowCoord(int [][COLS], int [][COLS], bool, bool, string, string,
			   int, int, int, int, int, int, int, int, int, int);
char getColCoord(int [][COLS], int [][COLS], bool, bool, string, string,
			   int, int, int, int, int, int, int, int, int, int);
char rowIntegerToCharacter(int);
char colIntegerToCharacter(int);
string getPass(int);
string showNeutralGround(int, int, int, int, int, int, int, int, int, int, int);
int characterToInteger(char);
bool overlappingHShips(int [][COLS], int, int, string);
bool overlappingVShips(int [][COLS], int, int, string);
bool userCheck(UserAcct[], string, int);

int main()
{
	ccl::activateConsoleColor();
	//declare local variables
	char mainChoice;
	int count = 0;						//counts number of players in the Player Array

	//declare local arrays
	UserAcct player[ARRAY_SIZE];

	//checking for userData file and creating if needed
	fstream userData;
	userData.open("playerdata.dat", ios::in);
	if (userData.fail())				//file does not exist, so create it
	{
		userData.open("playerdata.dat", ios::out);

		//add computer user
		player[0].userName = "computer";
		player[0].password = "0U812";
		player[0].firstName = "Computer";
		player[0].wins = 0;
		player[0].losses = 0;
		if (player[0].wins == 0 && player[0].losses == 0)
			player[0].winPercent = 0.0;
		else
			player[0].winPercent = player[0].wins / ((double)player[0].wins + player[0].losses) * 100.0;

		//write the computer user to the userData file
		userData << player[0].userName << "\n";
		userData << player[0].password << "\n";
		userData << player[0].firstName << "\n";
		userData << player[0].wins << "\n";
		userData << player[0].losses << "\n";
		userData << player[0].winPercent << "\n";

		userData.close();				//close the file for now
	}

	//if the file exists, read the players into the player array
	else
	{
		//read first player into player array
		getline(userData, player[count].userName);
		getline(userData, player[count].password);
		getline(userData, player[count].firstName);
		userData >> player[count].wins;
		userData >> player[count].losses;
		userData >> player[count].winPercent;

		count++;

		//while more players exist in the userData file,
		//continue reading players into the player array
		while (userData >> player[count].userName)
		{
			userData >> player[count].password;
			userData >> player[count].firstName;
			userData >> player[count].wins;
			userData >> player[count].losses;
			userData >> player[count].winPercent;

			count++;
		
		}
		
		userData.close();				//close the file for now
	}

	/*
	//testing the player array
	//   (uncomment this block to see the entire contents of the
	//		player data file displayed on screen)
	for (int index = 0; index < count; index++)
	{
		cout << "Player #" << index << endl;
		cout << "User Name:\t" << player[index].userName << endl;
		cout << "Password:\t" << player[index].password << endl;
		cout << "First Name:\t" << player[index].firstName << endl;
		cout << "Wins:\t\t" << player[index].wins << endl;
		cout << "Losses:\t\t" << player[index].losses << endl;
		cout << "Win Percentage:\t" << player[index].winPercent << endl << endl;
	}
	system("pause");
	*/


	//this block refreshes the screen and displays the Battleship logo
	//on the screen -- it is used extensively throughout the entire game
	system("cls");
	showSplash();

	//displays the main menu and gets the choice from the user
	mainChoice = showMainMenu();

	{
		while (mainChoice != '4')
		{	
			switch (mainChoice)
			{
				case '1':
					onePlayerGame(userData, mainChoice, count, player);
					break;
				case '2':
					twoPlayerGame(userData, mainChoice, count, player);
					break;
				case '3':
					accountSettings(userData, player, mainChoice, count);
					break;
				default :
					system("cls");
					showSplash();
					
					cout << Text::red << "\t\t      You have entered an invalid choice."
						<< Text::white;
					cout << "\a";
						
					mainChoice = showMainMenu();

			}
		}
				
		system("cls");
		showSplash();
		
		cout << "\n\n\t\t\tYou have chosen to end the game.\n\n\t\t\t    Thank you for playing!";
		cout << "\n\n\n\n\n\n\nGame is exiting....\n";
		Sleep(2000);
	}
	//system("pause");
	return 0;
}


//This function shows the main menu and returns the main menu choice
char showMainMenu()
{
	char choice = ' ';
	cout << Text::white;
	cout << "\n\n\t\t\t\t   MAIN MENU\n\n";
	cout << "\t\t\t    1 -- 1-Player Game\n";
	cout << "\t\t\t    2 -- 2-Player Game\n";
	cout << "\t\t\t    3 -- Account Settings\n";
	cout << "\t\t\t    4 -- Quit Game\n\n";
	cout << "\t\t\t      Enter 1, 2, 3 or 4 ";
//	ccl::setToDefaultColors();

	 choice = _getch();
	 if (choice == 0)
	 {
		 while (choice == 0)
		{

			system("cls");
			showSplash();
			cout << Text::red << "\t\t      WARNING!!!  Num Lock is turned off.";
					cout << "\a";
			Sleep(2000);		//learned this one the hard way!!
								//thanks to the James's for help in detecting this
			cout << Text::white;
			cout << "\n\n\t\t\t\t   MAIN MENU\n\n";
			cout << "\t\t\t    1 -- 1-Player Game\n";
			cout << "\t\t\t    2 -- 2-Player Game\n";
			cout << "\t\t\t    3 -- Account Settings\n";
			cout << "\t\t\t    4 -- Quit Game\n\n";
			cout << "\t\t\t      Enter 1, 2, 3 or 4 ";
//			ccl::setToDefaultColors();
			choice = _getch();
		}
	 }

	 if (choice == '1')
		 return choice;
	 if (choice == '2')
		 return choice;
	 if (choice == '3')
		 return choice;
	 if (choice == '4')
		 return choice;
}



//This function shows the splash screen for the menus
//Font: big (www.network-science.de/ascii/)
//Extra points if you noticed the morse code at the bottom spelling
//out "BATTLESHIP BATTLESHIP"
void showSplash()
{
	cout << Text::white;
	cout << "      ____       _______ _______ _      ______  _____ _    _ _____ _____      \n";
	cout << "     |  _ \\   /\\|__   __|__   __| |    |  ____|/ ____| |  | |_   _|  __ \\  \n";
	cout << Text::white;
	cout << "     | |_) | /  \\  | |     | |  | |    | |__  | (___ | |__| | | | | |__) |   \n";
	cout << Text::d_white;
	cout << "     |  _ < / /\\ \\ | |     | |  | |    |  __|  \\___ \\|  __  | | | |  ___/ \n";
	cout << "     | |_) / ____ \\| |     | |  | |____| |____ ____) | |  | |_| |_| |        \n";
	cout << Text::gray;
	cout << "     |____/_/    \\_\\_|     |_|  |______|______|_____/|_|  |_|_____|_|	   \n\n";
	cout << Text::white;
	cout << "    -... .- - - .-.. . ... .... .. .--. -... .- - - .-.. . ... .... .. .--.\n\n";
//	ccl::setToDefaultColors();
}



//account settings function
void accountSettings(fstream &userData, UserAcct player[], char & mnChoice, int & count)
{
	//declare local variables
	char acctChoice;

	system("cls");
	showSplash();
	
	//displaying the account menn and getting the choice from the user
	acctChoice = showAcctMenu();

		while (acctChoice != '5')
		{
			switch (acctChoice)
			{
				case '1':
					createNewAcct(userData, player, acctChoice, count);
					break;
				case '2':
					editAcct(userData, player, acctChoice, count);
					break;
				case '3':
					deleteAcct(userData, player, acctChoice, count);
					break;
				case '4':
					showUsers(player, acctChoice, count);
					break;
				default :
					system("cls");
					showSplash();
					
					cout << Text::red << "\t\t      You have entered an invalid choice." << Text::white;
					cout << "\a";
						
					acctChoice = showAcctMenu();

			}
			

		}

		system("cls");
		showSplash();
		mnChoice = showMainMenu();
}


//one-player game function
void onePlayerGame(fstream &userData, char & choice, int count, UserAcct player[])
{
	//declare variables
	string   uName1,						//holds the userName for Player	1
		     pWord1,						//holds the password for Player 1
		     fName1,						//holds the firstName for Player 1
		     uName2,						//holds the userName for Player 2
		     pWord2,						//holds the password for Player 2
		     fName2;						//holds the firstName for Player 2
	bool     found = false,					//set to true if player data is found
		     valid1 = false,				//set to true if player 1 data is validated
		     valid2 = false,				//set to true if player 2 data is validated
		     hidePlayer1 = true,			//sets the visibility of player 1's game board
		     hidePlayer2 = true,			//sets the visibility of player 2's game board
			 player1Wins = false,
			 player2Wins = false;
	int      index = 1,						//counter used to find players in the player array
		     player1,						//reference to player 1's position in the player array
		     player2,						//reference to player 2's position in the player array
		     p1Board[ROWS][COLS],			//player 1's board array
		     p2Board[ROWS][COLS],			//player 2's board array
			 currentPlayer,
		     p1Carr = 0,			//player 1's Aircraft Carrier
		     p2Carr = 0,			//player 2's Aircraft Carrier
		     p1Batt = 0,		//player 1's Battleship
		     p2Batt = 0,		//player 2's Battleship
		     p1Sub = 0,			//player 1's Submarine
		     p2Sub = 0,			//player 2's Submarine
		     p1Des = 0,			//player 1's Destroyer
		     p2Des = 0,			//player 2's Destroyer
		     p1Pat = 0,		//player 1's Patrol Boat
		     p2Pat = 0,		//player 2's Patrol Boat
		     rowArray,
		     colArray;
	char     playGame = 'Y',				//controls whether a game will be played or not
			 rowCoord,
			 colCoord;
	unsigned seed = time(0);				//getting the system time
	
	
	//seeding the random number generator
	srand(seed);

	system("cls");
	showSplash();

	//prompting for a user name
	cout << "\nPlease enter a user name for Player 1: ";
	cin >> uName1;

	//searching the player array for the entered user name
	while (index < count && found == false)
	{
		if (uName1 == player[index].userName)
		{
			found = true;
		}
		else
		{
			index++;
		}
	}
	
	//if the user entered a user name from the player array,
	//prompt for a matching password
	if (found)
	{
		system("cls");
		showSplash();
		cout << "\nPlease enter your password:";
		pWord1 = getPass(PASSWORD_WIDTH);
		
		//if the password matches, the player is validated
		if (pWord1 == player[index].password)
		{
			valid1 = true;
			
			//player 1 is announced and his position in the
			//player array is recorded in the player1 variable
			system("cls");
			showSplash();
			player1 = index;
			fName1 = player[index].firstName;
			cout << "\nPlayer 1 is " << fName1 << endl << endl;
			Sleep(1000);
		}
		
		//or the password didn't match and user is given three more attempts
		else
		{
			system("cls");
			showSplash();
			int attempts = 1;
			while (attempts <= 3 && !(valid1))
			{
				cout << Text::red << "The password was incorrect.\n" << Text::white << "Please re-enter password: ";
				cout << "\a";
				pWord1 = getPass(PASSWORD_WIDTH);
				attempts++;
				if(pWord1 == player[index].password)
				{
					valid1 = true;
				
					system("cls");
					showSplash();
					player1 = index;
					fName1 = player[index].firstName;
					cout << "\nPlayer 1 is " << fName1 << endl << endl;
					Sleep(1000);
				}

				//if the password can't be matched to the user account
				//the player is not validated and the function returns
				//to the main menu
				else
				{
					system("cls");
					showSplash();

					if (attempts > 3)
					{
						system("cls");
						showSplash();
						cout << Text::red << "\nPlayer 1 could not be validated.\n\n" << Text::white;
						cout << "\a";
						Sleep(1000);
					}
				}
			}
		}

	}

	//if the entered user name is not in the player array
	//the function returns to the main menu
	else
	{
		cout << Text::red << "\nThat user name cannot be found!\n" << Text::white;
		cout << "\a";
		Sleep(1000);
	}

	//if player 1 is validated, start the process over for player 2
	if (valid1)
	{
		system("cls");
		showSplash();

		//Player 2 is automatically validated as the computer user
		//which is the account in the first element of the player array
		uName2 = player[0].userName;

		pWord2 = player[0].password;

		if (pWord2 == player[0].password)
		{
			valid2 = true;
			
			system("cls");
			showSplash();

			player2 = 0;
			fName2 = player[0].firstName;
			cout << "\nPlayer 2 is the " << fName2 << endl << endl;
			Sleep(1000);
		}
	}

	//if player 1 and player 2 have been validated, the main game loop is entered
	if (valid1 && valid2)
	{
		while (toupper(playGame) == 'Y')
		{
			system("cls");
			showSplash();
			cout << Text::green << "\n\t\t\t\tThe game is on!" << Text::white << "\n\n\t\tPlayer 1: " 
				<< fName1 << "\t\t\tPlayer 2: " << fName2;
			Sleep(3000);

			//initialize game board arrays to 0
			for (int r = 0; r < ROWS; r++)
			{
				for (int c = 0; c < COLS; c++)
				{
					p1Board [r][c] = 0;
					p2Board [r][c] = 0;
				}
			}

			//Player 1 places ships on the board
			hidePlayer1 = false;				//player 1 can see his ships
			hidePlayer2 = true;					//player 2's board is blank

			//this refreshes the screen and shows the game boards -- it is used
			//extensively during the ship placement and game play portions of the program
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//player 1 starts out placing ships on the player 1 board
			cout << endl << fName1 << "\'s turn to place ships on the board.\n";
			system("pause");
			
			//placing Carrier
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Carrier", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						
			//placing Battleship
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Battleship", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//placing Submarine
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Submarine", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Destroyer
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Destroyer", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Patrol Boat
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Patrol Boat", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				
			
			//Player 2 places ships on the board
			hidePlayer1 = true;					//player 1's board is blank
			hidePlayer2 = true;					//player 2's board is blank

			//game board refresh
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			cout << endl << fName2 << "\'s turn to place ships on the board.\n";
			Sleep(2000);
			
			/*
			//temporary placement of computer's ships -- needs to be replaced with random function
			for (index = 0; index < 5; index++)
				p2Board[0][index] += 1;
			for (index = 2; index < 6; index++)
				p2Board[2][index] += 2;
			for (index = 4; index < 7; index++)
				p2Board[4][index] += 3;
			for (index = 3; index < 6; index++)
				p2Board[6][index] += 4;
			for (index = 7; index < 9; index++)
				p2Board[8][index] =+ 5;
			*/

			//placing Carrier
			autoPlaceShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Carrier", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				
			//placing Battleship
			autoPlaceShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Battleship", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//placing Submarine
			autoPlaceShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Submarine", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Destroyer
			autoPlaceShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Destroyer", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Patrol Boat
			autoPlaceShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Patrol Boat", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				

            //start of actual game play
			hidePlayer1 = true;					//player 1's board is blank
			hidePlayer2 = true;					//player 2's board is blank
			
			//game board refresh
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//randomly choosing which player to go first
			currentPlayer = 1 + rand() % 2;

			//first player to start is announced
			if (currentPlayer == 1)
				cout << endl << fName1 << " has randomly been chosen to go first.\n\n";
			else
				cout << endl << fName2 << " has randomly been chosen to go first.\n\n";
			system("pause");

			//start game loop -- continues until a winner is determined
			while (!(player1Wins || player2Wins))
			{
				if (currentPlayer == 1)
				{
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//getting the coordinates from the player to bomb
					cout << endl << fName1 << ", ";
					rowCoord = getRowCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
					cout << endl << "Now, ";
					colCoord = getColCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//converting the coordinates to integers for use with the gameboard
					rowArray = characterToInteger(rowCoord) - 97;
					colArray = characterToInteger(colCoord);

					//checking to see if chosen location has already been chosen
					while (p2Board[rowArray][colArray] >= 100)
					{
						system("cls");
						showSplash();
						showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						cout << Text::red << "Those coordinates have already been chosen.  Please choose again." << Text::white;
						cout << "\a";

						//getting the coordinates from the player to bomb
						cout << endl << fName1 << ", ";
						rowCoord = getRowCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						cout << endl << "Now, ";
						colCoord = getColCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

						//converting the coordinates to integers for use with the gameboard
						rowArray = characterToInteger(rowCoord) - 97;
						colArray = characterToInteger(colCoord);
					}

					//adding 100 to the chosen coordintes indicating a bomb has landed there
					p2Board[rowArray][colArray] += 100;

					//zeroing out the ship variables
					
					p2Carr = 0;
					p2Batt = 0;
					p2Sub = 0;
					p2Des = 0;
					p2Pat = 0;

					//scanning the players boards
					for (int row = 0; row < ROWS; row++)
					{
						for (int col = 0; col < COLS; col++)
						{
							if (p2Board[row][col] == 101)
								p2Carr++;
							if (p2Board[row][col] == 102)
								p2Batt++;
							if (p2Board[row][col] == 103)
								p2Sub++;
							if (p2Board[row][col] == 104)
								p2Des++;
							if (p2Board[row][col] == 105)
								p2Pat++;
						}
					}

					//annoucing hit or miss
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					showHitMiss(rowCoord, colCoord, rowArray, colArray, currentPlayer, p1Board, p2Board);

					//looking for a winner
					if (p2Carr == 5 && p2Batt == 4 && p2Sub == 3 && p2Des == 3 && p2Pat == 2)
						player1Wins = true;
					else					//changing current player
					currentPlayer = 2;
				}
				else						//current player is Player 2
				{
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//getting the coordinates from the player to bomb
					rowArray = rand() % 9;	//characterToInteger(rowCoord) - 97;
					colArray = rand() % 9;	//characterToInteger(colCoord);

					//checking to see if chosen location has already been chosen
					while (p1Board[rowArray][colArray] >= 100)
					{
						rowArray = rand() % 9;	//characterToInteger(rowCoord) - 97;
						colArray = rand() % 9;	//characterToInteger(colCoord);
					}

					//adding 100 to the chosen coordintes indicating a bomb has landed there
					p1Board[rowArray][colArray] += 100;

					//converting the array coordinates to their corresponding character names
					rowCoord = rowIntegerToCharacter(rowArray);
					colCoord = colIntegerToCharacter(colArray);

					//zeroing out the ship variables
					p1Carr = 0;
					p1Batt = 0;
					p1Sub = 0;
					p1Des = 0;
					p1Pat = 0;

					//scanning the player board
					for (int row = 0; row < ROWS; row++)
					{
						for (int col = 0; col < COLS; col++)
						{
							if (p1Board[row][col] == 101)
								p1Carr++;
							if (p1Board[row][col] == 102)
								p1Batt++;
							if (p1Board[row][col] == 103)
								p1Sub++;
							if (p1Board[row][col] == 104)
								p1Des++;
							if (p1Board[row][col] == 105)
								p1Pat++;
						}
					}

					//annoucing hit or miss
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
					
					cout << "\nThe computer has chosen " << rowCoord << ":" <<colCoord;
					showHitMiss(rowCoord, colCoord, rowArray, colArray, currentPlayer, p1Board, p2Board);
					
					//looking for a winner
					if (p1Carr == 5 && p1Batt == 4 && p1Sub == 3 && p1Des == 3 && p1Pat == 2)
						player2Wins = true;
					else					//changing current player
					currentPlayer = 1;
				}

			}

			//if all of player 2's ships are sunk
			if (player1Wins)
			{
				//setting the board visibility
				hidePlayer1 = false;					//player 1's board is visible
				hidePlayer2 = false;					//player 2's board is visible

				//announce player 1 is the winner
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << endl << fName1 << " has won the game!\n\n";

				//update stats for player one and player two here
				player[player1].wins++;
				player[player1].winPercent = ((double)player[player1].wins/(player[player1].wins + player[player1].losses)) * 100.0;

				player[player2].losses++;
				player[player2].winPercent = ((double)player[player2].wins/(player[player2].wins + player[player2].losses)) * 100.0;
				
				//opening the userData file in output mode
				userData.open("playerdata.dat", ios::out);

				//writing updated player stats to the userData file
				for (index = 0; index < count; index++)
				{

					//write the updated player array to the userData file
					userData << player[index].userName << "\n";
					userData << player[index].password << "\n";
					userData << player[index].firstName << "\n";
					userData << player[index].wins << "\n";
					userData << player[index].losses << "\n";
					userData << player[index].winPercent << "\n";

				}

				//closing the data file
				userData.close();

				system("pause");
			}
			else      
			{
				//setting the board visibility
				hidePlayer1 = false;					//player 1's board is visible
				hidePlayer2 = false;					//player 2's board is visible

				//announce player 2 is the winner
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << endl << fName2 << " has won the game!\n\n";

				//update stats for player one and player two here
				player[player2].wins++;
				player[player2].winPercent = ((double)player[player2].wins/(player[player2].wins + player[player2].losses)) * 100.0;

				player[player1].losses++;
				player[player1].winPercent = ((double)player[player1].wins/(player[player1].wins + player[player1].losses)) * 100.0;
				
				//opening the userData file in output mode
				userData.open("playerdata.dat", ios::out);

				//writing updated player stats to the userData file
				for (index = 0; index < count; index++)
				{

					//write the updated player array to the userData file
					userData << player[index].userName << "\n";
					userData << player[index].password << "\n";
					userData << player[index].firstName << "\n";
					userData << player[index].wins << "\n";
					userData << player[index].losses << "\n";
					userData << player[index].winPercent << "\n";

				}

				//closing the data file
				userData.close();

				system("pause");
			}
			
			//after the game is finished, prompt for a re-match
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << "\nWould you like a re-match? (\"Y\" or \"N\"): ";
			playGame = _getch();

			while (toupper(playGame) != 'Y' && toupper(playGame) != 'N')
			{
				//user must choose Y or N to proceed
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "Invalid selection!  You must enter only \"Y\" or \"N\"." << Text::white;
				cout << "\nWould you like a re-match? (\"Y\" or \"N\"): ";
				cout << "\a";
				playGame = _getch();
			}
			
			//resetting the game variables
			player1Wins = false;
			player2Wins = false;
			p1Carr = 0;
			p1Batt = 0;
			p1Sub = 0;
			p1Des = 0;
			p1Pat = 0;
			p2Carr = 0;
			p2Batt = 0;
			p2Sub = 0;
			p2Des = 0;
			p2Pat = 0;
			
		}
	}
	else
	{
		cout << Text::red << "\n\nYou need a validated player to play a game.\n\n" << Text::white;
		cout << "\a";
		system ("pause");
		system("cls");
	}
	
	
	system("cls");
	showSplash();
	choice = showMainMenu();
}


//two-player game function
void twoPlayerGame(fstream &userData, char & choice, int count, UserAcct player[])
{
	//declare variables
	string   uName1,						//holds the userName for Player	1
		     pWord1,						//holds the password for Player 1
		     fName1,						//holds the firstName for Player 1
		     uName2,						//holds the userName for Player 2
		     pWord2,						//holds the password for Player 2
		     fName2;						//holds the firstName for Player 2
	bool     found = false,					//set to true if player data is found
		     valid1 = false,				//set to true if player 1 data is validated
		     valid2 = false,				//set to true if player 2 data is validated
		     hidePlayer1 = true,			//sets the visibility of player 1's game board
		     hidePlayer2 = true,			//sets the visibility of player 2's game board
			 player1Wins = false,			//determines if Player 1 wins the game
			 player2Wins = false;			//determines if Player 2 wins the game
	int      index = 1,						//counter used to find players in the player array
		     player1,						//reference to player 1's position in the player array
		     player2,						//reference to player 2's position in the player array
		     p1Board[ROWS][COLS],			//player 1's board array
		     p2Board[ROWS][COLS],			//player 2's board array
			 currentPlayer,					//player who's turn it is to choose a coordinate to bomb
		     p1Carr = 0,					//player 1's Aircraft Carrier
		     p2Carr = 0,					//player 2's Aircraft Carrier
		     p1Batt = 0,					//player 1's Battleship
		     p2Batt = 0,					//player 2's Battleship
		     p1Sub = 0,						//player 1's Submarine
		     p2Sub = 0,						//player 2's Submarine
		     p1Des = 0,						//player 1's Destroyer
		     p2Des = 0,						//player 2's Destroyer
		     p1Pat = 0,						//player 1's Patrol Boat
		     p2Pat = 0,						//player 2's Patrol Boat
			 rowArray,						//row location in the board array
			 colArray;						//column location in the board array
	char     playGame = 'Y',				//controls whether a game will be played or not
			 rowCoord,						//row coordinate to bomb
			 colCoord;						//column coordinate to bomb
	unsigned seed = time(0);				//getting the system time
	
	
	//seeding the random number generator
	srand(seed);
	

	system("cls");
	showSplash();

	//prompting for a user name
	cout << "\nPlease enter a user name for Player 1: ";
	cin >> uName1;

	//searching the player array for the entered user name
	while (index < count && found == false)
	{
		if (uName1 == player[index].userName)
		{
			found = true;
		}
		else
		{
			index++;
		}
	}
	
	//if the user entered a user name from the player array,
	//prompt for a matching password
	if (found)
	{
		system("cls");
		showSplash();
		cout << "\nPlease enter your password:";
		pWord1 = getPass(PASSWORD_WIDTH);
		
		//if the password matches, the player is validated
		if (pWord1 == player[index].password)
		{
			valid1 = true;
			
			//player 1 is announced and his position in the
			//player array is recorded in the player1 variable
			system("cls");
			showSplash();
			player1 = index;
			fName1 = player[index].firstName;
			cout << "\nPlayer 1 is " << fName1 << endl << endl;
			Sleep(1000);
		}

		//or the password didn't match and user is given three more attempts
		else
		{
			system("cls");
			showSplash();
			int attempts = 1;
			while (attempts <= 3 && !(valid1))
			{
				cout << Text::red << "The password was incorrect." << Text::white <<"\nPlease re-enter password: ";
				cout << "\a";
				pWord1 = getPass(PASSWORD_WIDTH);
				attempts++;
				if(pWord1 == player[index].password)
				{
					valid1 = true;
				
					system("cls");
					showSplash();
					player1 = index;
					fName1 = player[index].firstName;
					cout << "\nPlayer 1 is " << fName1 << endl << endl;
					Sleep(1000);
				}

				//if the password can't be matched to the user account
				//the player is not validated and the function returns
				//to the main menu
				else
				{
					system("cls");
					showSplash();

					if (attempts > 3)
					{
						system("cls");
						showSplash();
						cout << Text::red << "\nPlayer 1 could not be validated.\n\n" << Text::white;
						cout << "\a";
						Sleep(1000);
					}
				}
			}
		}

	}

	//if the entered user name is not in the player array
	//the function returns to the main menu
	else
	{
		cout << Text::red << "\nThat user name cannot be found!\n" << Text::white;
		cout << "\a";
		Sleep(1000);
	}

	//if player 1 is validated, start the process over for player 2
	if (valid1)
	{
		//resetting the search variables
		found = false;
		index = 1;
	
		//prompt for a user name
		system("cls");
		showSplash();
		cout << "\nPlease enter a user name for Player 2: ";
		cin >> uName2;

		//search the player array for the entered user name
		while (index < count && found == false)
		{
			if (uName2 == player[index].userName)
			{
				found = true;
			}
			else
			{
				index++;
			}
		}
	
		//if the user name is found, prompt for a password
		if (found)
		{
			system("cls");
			showSplash();
			cout << "\nPlease enter your password:";
			pWord2 = getPass(PASSWORD_WIDTH);

			//if the password matches the associated password for the username
			//player 2 has been validated
			if (pWord2 == player[index].password)
			{
				valid2 = true;

				//player 2 is announced
				system("cls");
				showSplash();
				player2 = index;
				fName2 = player[index].firstName;
				cout << "\nPlayer 2 is " << fName2 << endl << endl;
				Sleep(1000);
			}

			//if the password doesn't match, the user is given three
			//more chances
			else
			{
				system("cls");
				showSplash();
				int attempts = 1;
				while (attempts <= 3 && !(valid2))
				{
					cout << Text::red << "The password was incorrect." << Text::white << "\nPlease re-enter password: ";
					cout << "\a";
					pWord2 = getPass(PASSWORD_WIDTH);
					attempts++;

					//if the password matches, player 2 is validated
					if(pWord2 == player[index].password)
					{
						valid2 = true;

						//player 2 is announced and his position in the 
						//player array is recorded in the player2 variable
						system("cls");
						showSplash();
						player2 = index;
						fName2 = player[index].firstName;
						cout << "\nPlayer 2 is " << fName2 << endl << endl;
						Sleep(1000);
					}

					//or the player could not be validated and the function returns
					//to the main menu
					else
					{
						system("cls");
						showSplash();

						if (attempts > 3)
						{
							system("cls");
							showSplash();
							cout << Text::red << "\nPlayer 2 could not be validated.\n\n" << Text::white;
							cout << "\a";
							Sleep(1000);
						}
					}
				}
			}

		}

		//if the user name entered for player 2 is not in the player array
		//that announcement is made and the function returns to the main menu
		else
		{
			cout << Text::red << "\nThat user name cannot be found!\n" << Text::white;
			cout << "\a";
			Sleep(1000);
		}
	}

	//if player 1 and player 2 have been validated, the main game loop is entered
	if (valid1 && valid2)
	{
		while (toupper(playGame) == 'Y')
		{
			system("cls");
			showSplash();
			cout << Text::green << "\n\t\t\t\tThe game is on!" << Text::white << "\n\n\t\tPlayer 1: "
				<< fName1 << "\t\t\tPlayer 2: " << fName2;
			Sleep(3000);

			//initialize game board arrays to 0
			for (int r = 0; r < ROWS; r++)
			{
				for (int c = 0; c < COLS; c++)
				{
					p1Board [r][c] = 0;
					p2Board [r][c] = 0;
				}
			}

			//Player 1 places ships on the board
			hidePlayer1 = false;				//player 1 can see his ships
			hidePlayer2 = true;					//player 2's board is blank

			//this refreshes the screen and shows the game boards -- it is used
			//extensively during the ship placement and game play portions of the program
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//player 1 starts out placing ships on the player 1 board
			cout << endl << fName1 << "\'s turn to place ships on the board.\n";
			system("pause");

			//placing Carrier
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Carrier", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Battleship
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Battleship", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//placing Submarine
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Submarine", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Destroyer
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Destroyer", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Patrol Boat
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Patrol Boat", 1,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				
			
			//Player 2 places ships on the board
			hidePlayer1 = true;					//player 1's board is blank
			hidePlayer2 = false;				//player 2 can see his ships

			//game board refresh
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//player 2 now places ships on the player 2 board
			cout << endl << fName2 << "\'s turn to place ships on the board.\n";
			system("pause");
			
			//placing Carrier
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Carrier", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//placing Battleship
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Battleship", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			
			//placing Submarine
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Submarine", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Destroyer
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Destroyer", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//placing Patrol Boat
			placeShip(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, "Patrol Boat", 2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				

            //start of actual game play
			hidePlayer1 = true;					//player 1's board is blank
			hidePlayer2 = true;					//player 2's board is blank
			
			//game board refresh
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

			//randomly choosing which player to go first
			currentPlayer = 1 + rand() % 2;

			//first player to start is announced
			if (currentPlayer == 1)
				cout << endl << fName1 << " has randomly been chosen to go first.\n\n";
			else
				cout << endl << fName2 << " has randomly been chosen to go first.\n\n";
			system("pause");

			//start game loop -- continues until a winner is determined
			while (!(player1Wins || player2Wins))
			{
				if (currentPlayer == 1)
				{
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//getting the coordinates from the player to bomb
					cout << endl << fName1 << ", ";
					rowCoord = getRowCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
					cout << endl << "Now, ";
					colCoord = getColCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//converting the coordinates to integers for use with the gameboard
					rowArray = characterToInteger(rowCoord) - 97;
					colArray = characterToInteger(colCoord);

					//checking to see if chosen location has already been chosen
					while (p2Board[rowArray][colArray] >= 100)
					{
						system("cls");
						showSplash();
						showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						cout << Text::red << "Those coordinates have already been chosen.  Please choose again." << Text::white;
						cout << "\a";

						//getting the coordinates from the player to bomb
						cout << endl << fName1 << ", ";
						rowCoord = getRowCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						cout << endl << "Now, ";
						colCoord = getColCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

						//converting the coordinates to integers for use with the gameboard
						rowArray = characterToInteger(rowCoord) - 97;
						colArray = characterToInteger(colCoord);
					}

					//adding 100 to the chosen coordintes indicating a bomb has landed there
					p2Board[rowArray][colArray] += 100;

					//zeroing out the ship variables
					
					p2Carr = 0;
					p2Batt = 0;
					p2Sub = 0;
					p2Des = 0;
					p2Pat = 0;

					//scanning the players boards
					for (int row = 0; row < ROWS; row++)
					{
						for (int col = 0; col < COLS; col++)
						{
							if (p2Board[row][col] == 101)
								p2Carr++;
							if (p2Board[row][col] == 102)
								p2Batt++;
							if (p2Board[row][col] == 103)
								p2Sub++;
							if (p2Board[row][col] == 104)
								p2Des++;
							if (p2Board[row][col] == 105)
								p2Pat++;
						}
					}

					//annoucing hit or miss
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					showHitMiss(rowCoord, colCoord, rowArray, colArray, currentPlayer, p1Board, p2Board);

					//looking for a winner
					if (p2Carr == 5 && p2Batt == 4 && p2Sub == 3 && p2Des == 3 && p2Pat == 2)
						player1Wins = true;
					else					//changing current player
					currentPlayer = 2;
				}
				else						//current player is Player 2
				{
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//getting the coordinates from the player to bomb
					cout << endl << fName2 << ", ";
					rowCoord = getRowCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
					cout << endl << "Now, ";
					colCoord = getColCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					//converting the coordinates to integers for use with the gameboard
					rowArray = characterToInteger(rowCoord) - 97;
					colArray = characterToInteger(colCoord);

					//checking to see if chosen location has already been chosen
					while (p1Board[rowArray][colArray] >= 100)
					{
						system("cls");
						showSplash();
						showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						cout << Text::red << "Those coordinates have already been chosen.  Please choose again." << Text::white;
						cout << "\a";

						//getting the coordinates from the player to bomb
						cout << endl << fName2 << ", ";
						rowCoord = getRowCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
						cout << endl << "Now, ";
						colCoord = getColCoord(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
							p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

						//converting the coordinates to integers for use with the gameboard
						rowArray = characterToInteger(rowCoord) - 97;
						colArray = characterToInteger(colCoord);
					}

					//adding 100 to the chosen coordintes indicating a bomb has landed there
					p1Board[rowArray][colArray] += 100;

					//zeroing out the ship variables
					p1Carr = 0;
					p1Batt = 0;
					p1Sub = 0;
					p1Des = 0;
					p1Pat = 0;

					//scanning the player board
					for (int row = 0; row < ROWS; row++)
					{
						for (int col = 0; col < COLS; col++)
						{
							if (p1Board[row][col] == 101)
								p1Carr++;
							if (p1Board[row][col] == 102)
								p1Batt++;
							if (p1Board[row][col] == 103)
								p1Sub++;
							if (p1Board[row][col] == 104)
								p1Des++;
							if (p1Board[row][col] == 105)
								p1Pat++;
						}
					}

					//annoucing hit or miss
					system("cls");
					showSplash();
					showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
						p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

					showHitMiss(rowCoord, colCoord, rowArray, colArray, currentPlayer, p1Board, p2Board);

					//looking for a winner
					if (p1Carr == 5 && p1Batt == 4 && p1Sub == 3 && p1Des == 3 && p1Pat == 2)
						player2Wins = true;
					else					//changing current player
					currentPlayer = 1;
				}

			}

			//if all of player 2's ships are sunk
			if (player1Wins)
			{
				//setting the board visibility
				hidePlayer1 = false;					//player 1's board is visible
				hidePlayer2 = false;					//player 2's board is visible

				//announce player 1 is the winner
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << endl << fName1 << " has won the game!\n\n";

				//update stats for player one and player two here
				player[player1].wins++;
				player[player1].winPercent = ((double)player[player1].wins/(player[player1].wins + player[player1].losses)) * 100.0;

				player[player2].losses++;
				player[player2].winPercent = ((double)player[player2].wins/(player[player2].wins + player[player2].losses)) * 100.0;
				
				//opening the userData file in output mode
				userData.open("playerdata.dat", ios::out);

				//writing updated player stats to the userData file
				for (index = 0; index < count; index++)
				{

					//write the updated player array to the userData file
					userData << player[index].userName << "\n";
					userData << player[index].password << "\n";
					userData << player[index].firstName << "\n";
					userData << player[index].wins << "\n";
					userData << player[index].losses << "\n";
					userData << player[index].winPercent << "\n";

				}

				//closing the data file
				userData.close();

				system("pause");
			}
			else      
			{
				//setting the board visibility
				hidePlayer1 = false;					//player 1's board is visible
				hidePlayer2 = false;					//player 2's board is visible

				//announce player 2 is the winner
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << endl << fName2 << " has won the game!\n\n";

				//update stats for player one and player two here
				player[player2].wins++;
				player[player2].winPercent = ((double)player[player2].wins/(player[player2].wins + player[player2].losses)) * 100.0;

				player[player1].losses++;
				player[player1].winPercent = ((double)player[player1].wins/(player[player1].wins + player[player1].losses)) * 100.0;
				
				//opening the userData file in output mode
				userData.open("playerdata.dat", ios::out);

				//writing updated player stats to the userData file
				for (index = 0; index < count; index++)
				{

					//write the updated player array to the userData file
					userData << player[index].userName << "\n";
					userData << player[index].password << "\n";
					userData << player[index].firstName << "\n";
					userData << player[index].wins << "\n";
					userData << player[index].losses << "\n";
					userData << player[index].winPercent << "\n";

				}

				//closing the data file
				userData.close();

				system("pause");
			}
			
			//after the game is finished, prompt for a re-match
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << "\nWould you like a re-match? (\"Y\" or \"N\"): ";
			playGame = _getch();

			while (toupper(playGame) != 'Y' && toupper(playGame) != 'N')
			{
				//user must choose Y or N to proceed
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"Y\" or \"N\"." << Text::white;
				cout << "\nWould you like a re-match? (\"Y\" or \"N\"): ";
				cout << "\a";
				playGame = _getch();
			}
			
			//resetting the game variables
			player1Wins = false;
			player2Wins = false;
			p1Carr = 0;
			p1Batt = 0;
			p1Sub = 0;
			p1Des = 0;
			p1Pat = 0;
			p2Carr = 0;
			p2Batt = 0;
			p2Sub = 0;
			p2Des = 0;
			p2Pat = 0;
			
		}
	}
	//this will execute if one player or the other are not validated
	else
	{
		cout << Text::red << "\n\nYou need two validated players to play a game.\n\n" << Text::white;
		cout << "\a";
		system ("pause");
		system("cls");
	}

	
	//this displays the main menu
	system("cls");
	showSplash();
	choice = showMainMenu();
}


//this function shows the game board
void showBoard(int p1Board [][COLS], int p2Board[][COLS], bool hidePlayer1, bool hidePlayer2, string fName1, string fName2,
			   int c1, int b1, int s1, int d1, int p1, int c2, int b2, int s2, int d2, int p2)
{
	cout << "\t   " << fName1 << "\'s Field\t\t\t\t      " << fName2 << "\'s Field\n";
	cout << Text::d_white << "\t   1 2 3 4 5 6 7 8 9                           1 2 3 4 5 6 7 8 9\n" << Text::white;
	cout << "\t  -------------------                        -------------------\n";
	for (int r = 0; r < ROWS; r++)
	{
		cout << Text::d_white << "\t" << showRowLetter(r) << Text::white << "| ";
		for (int c = 0; c < COLS; c++)
		{
			if (hidePlayer1)
				cout << showHidden(p1Board [r][c]) << " " << Text::white;
			else
				cout << showVisible(p1Board [r][c]) << " " << Text::white;
		}
		cout << showNeutralGround(r, c1, b1, s1, d1, p1, c2, b2, s2, d2, p2) << 
			Text::d_white << showRowLetter(r) << Text::white << "| ";

		for (int c = 0; c < ROWS; c++)
		{
			if (hidePlayer2)
				cout << showHidden(p2Board [r][c]) << " " << Text::white;
			else
				cout << showVisible(p2Board [r][c]) << " " << Text::white;
		}
		cout << endl;
	}
/*	cout << "\t    Player 1 Field                             Player 2 Field\n";
	cout << "\t   1 2 3 4 5 6 7 8 9                         1 2 3 4 5 6 7 8 9\n";
	cout << "\t  -------------------                       -------------------\n";
	cout << "\tA| - - - - O - - - -        SHIPS:        A| X - - - - - - - -\n";
	cout << "\tB| - - - - - - - - -                      B| X - - - - O - - -\n";
	cout << "\tC| - O - - - - - - -       CARRIER        C| O - - - - - - - -\n";
	cout << "\tD| - - - - O - - O -      BATTLESHIP      D| - - O - - - - - -\n";
	cout << "\tE| - - - - - - - - -       SUBMARINE  X   E| - - - - O - - - -\n";
	cout << "\tF| - - - - - - - - -       DESTROYER      F| - - - - - - O - -\n";
	cout << "\tG| - O - O - - - - -    X PATROL BOAT     G| - - - - - - - - -\n";
	cout << "\tH| O X O - - - O - -                      H| - - O X X X - - -\n";
	cout << "\tI| - X - - - - - - -                      I| - - - - - - - - -\n\n";*/
}


//This function shows the user account menu and returns the account menu choice
char showAcctMenu()
{
	char choice = ' ';
	
	cout << Text::white;	
	cout << "\n\n\t\t\t     ACCOUNT SETTINGS MENU\n\n";
	cout << "\t\t\t    1 -- Create New Account\n";
	cout << "\t\t\t    2 -- Edit Existing Account\n";
	cout << "\t\t\t    3 -- Delete Existing Account\n";
	cout << "\t\t\t    4 -- Show Existing Accounts\n";
	cout << "\t\t\t    5 -- Return to Main Menu\n\n";
	cout << "\t\t\t      Enter 1, 2, 3, 4 or 5 ";
	ccl::setToDefaultColors();

	choice = _getch();
	 if (choice == 0)
	 {
		 while (choice == 0)
		{

			system("cls");
			showSplash();
			cout << Text::red << "\t\t      WARNING!!!  Num Lock is turned off." << Text::white;
			cout << "\a";
			Sleep(2000);
		 
			cout << Text::white;
			cout << "\n\n\t\t\t     ACCOUNT SETTINGS MENU\n\n";
			cout << "\t\t\t    1 -- Create New Account\n";
			cout << "\t\t\t    2 -- Edit Existing Account\n";
			cout << "\t\t\t    3 -- Delete Existing Account\n";
			cout << "\t\t\t    4 -- Show Existing Accounts\n";
			cout << "\t\t\t    5 -- Return to Main Menu\n\n";
			cout << "\t\t\t      Enter 1, 2, 3, 4 or 5 ";
			ccl::setToDefaultColors();
			choice = _getch();
		}
	 }

	if (choice == '1')
		return choice;
	if (choice == '2')
		return choice;
	if (choice == '3')
		return choice;
	if (choice == '4')
		return choice;
	if (choice == '5')
		return choice;

}


//this function creates a new entry in the userData file
void createNewAcct(fstream &userData, UserAcct player[], char & choice, int & count)
{
	//variable declarations
	string uName, pWord1, pWord2, fName;
	char writeNow;							//variable to trigger writing new account to file
	bool userExists;						//variable to check if a newly entered user already exists
	int spaceFound = -1;

	system("cls");
	showSplash();
	if (count < ARRAY_SIZE)
	{
		//opening the userData file in input/output/append mode
		userData.open("playerdata.dat", ios::in | ios::out | ios::app);

		//prompting for a new user name
		cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
		cout << "\nEnter your user name: ";
		getline(cin, uName);

		//checking for spaces in the user name
		spaceFound = uName.find(" " , 0);

		while (spaceFound != -1)
		{
			system("cls");
			showSplash();
			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
			cout << Text::red << "\nYou cannot have spaces in your user name." << Text::white;
			cout << "\nEnter your user name: ";
			cout << "\a";
			getline(cin, uName);
			spaceFound = -1;
			spaceFound = uName.find(" ", 0);
		}

		system("cls");
		showSplash();
		userExists = userCheck(player, uName, count);

		//if the user name that was entered is already in use by aother
		//user, give the user another chance to enter a new user name
		while (userExists)
		{
			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
			cout << Text::red << "\nThat user name already exists, please choose another name." << Text::white;
			cout << "\nEnter your user name: ";
			cout << "\a";
			getline(cin, uName);

			//checking for spaces in the user name
			spaceFound = uName.find(" " , 0);

			while (spaceFound != -1)
			{
				system("cls");
				showSplash();
				cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
				cout << Text::red << "\nYou cannot have spaces in your user name." << Text::white;
				cout << "\nEnter your user name: ";
				getline(cin, uName);
				spaceFound = -1;
				spaceFound = uName.find(" ", 0);
			}
			system("cls");
			showSplash();
			userExists = userCheck(player, uName, count);
		}

		//prompting for a password
		cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
		cout << "\nEnter your password (15 characters or less): ";
		pWord1 = getPass(PASSWORD_WIDTH);

		
		//checking for spaces in the password
		spaceFound = pWord1.find(" " , 0);

		while (spaceFound != -1)
		{
				system("cls");
				showSplash();
				cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
				cout << Text::red << "\nYou cannot have spaces in your password." << Text::white;
				cout << "\nEnter your password (15 characters or less): ";
				cout << "\a";
				pWord1 = getPass(PASSWORD_WIDTH);
				spaceFound = -1;
				spaceFound = pWord1.find(" ", 0);
		}

		//verifying password
		system("cls");
		showSplash();
		cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
		cout << "\nPlease re-enter your password: ";
		pWord2 = getPass(PASSWORD_WIDTH);
		system("cls");
		showSplash();

		//if the password was not entered the same between the first and
		//second time it was entered, give the user another chance to
		//correctly enter a password
		while (pWord1 != pWord2)
		{
			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
			cout << Text::red << "\nPasswords did not match!" << Text::white;
			cout << "\nPlease enter your password (15 characters of less): ";
			cout << "\a";
			pWord1 = getPass(PASSWORD_WIDTH);
			//checking for spaces in the user name
			spaceFound = pWord1.find(" " , 0);

			while (spaceFound != -1)
			{
					system("cls");
					showSplash();
					cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
					cout << Text::red << "\nYou cannot have spaces in your password." << Text::white;
					cout << "\nEnter your password (15 characters or less): ";
					cout << "\a";
					pWord1 = getPass(PASSWORD_WIDTH);
					spaceFound = -1;
					spaceFound = pWord1.find(" ", 0);
			}

			//verifying password
			system("cls");
			showSplash();
			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
			cout << "\nPlease re-enter your password: ";
			pWord2 = getPass(PASSWORD_WIDTH);
			system("cls");
			showSplash();
		}

		//prompting for a first name -- this will be the name displayed
		//on the game screens announcing things like who's turn it is
		//or who won the game
		cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
		cout << "\nEnter your display name: ";
		getline(cin, fName);

		//checking for spaces in the user name
		spaceFound = fName.find(" " , 0);

		while (spaceFound != -1)
		{
			system("cls");
			showSplash();
			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n";
			cout << Text::red << "\nYou cannot have spaces in your display name." << Text::white;
			cout << "\nEnter your display name: ";
			cout << "\a";
			getline(cin, fName);
			spaceFound = -1;
			spaceFound = fName.find(" ", 0);
		}
				
		system("cls");
		showSplash();

		//loading user info into player array
		player[count].userName = uName;
		player[count].password = pWord1;
		player[count].firstName = fName;
		player[count].wins = 0;
		player[count].losses = 0;
		if (player[count].wins == 0 && player[count].losses == 0)
			player[count].winPercent = 0.0;
		else
			player[count].winPercent = player[count].wins / ((double)player[count].wins + player[count].losses) * 100.0;

		//display the entered data (hiding the password)
		cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
		cout << "\nNew player information entered...\n\n";
		cout << "Username:\t" << player[count].userName << endl;
		cout << "Password:\t[***************]" << endl;
		cout << "First name:\t" << player[count].firstName;
	
		//write the new account to file or abandon changes to file
		cout << "\n\n\nDo you want to save this account to file?\n";
		cout << "Enter \"Y\" for Yes or \"N\" for No: ";
		writeNow = _getch();

		while (toupper(writeNow) != 'Y' && toupper(writeNow) != 'N')
		{			
			//display the entered data (hiding the password), again
			system("cls");
			showSplash();

			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
			cout << "\nNew player information entered...\n\n";
			cout << "Username:\t" << player[count].userName << endl;
			cout << "Password:\t[***************]" << endl;
			cout << "First name:\t" << player[count].firstName;

			//write the new account to file or abandon changes to file, again
			cout << Text::red << "\n\nInvalid selection!  You must enter only \"Y\" or \"N\"." << Text::white;
			cout << "\nDo you want to save this account to file?\n";
			cout << "Enter \"Y\" for Yes or \"N\" for No: ";
			cout << "\a";
			writeNow = _getch();
		}

		if (toupper(writeNow) == 'Y')
		{
			//write the computer user to the userData file
			userData << player[count].userName << "\n";
			userData << player[count].password << "\n";
			userData << player[count].firstName << "\n";
			userData << player[count].wins << "\n";
			userData << player[count].losses << "\n";
			userData << player[count].winPercent << "\n";

			//incrementing the count variable
			count++;

			//account written to file
			system("cls");
			showSplash();

			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
			cout << "\nNew account written to file.\n";
			cout << "\nReturning to Account Settings Menu.";
		}
		else
		{
			//account abandonded
			system("cls");
			showSplash();

			cout << "\n\n\t\t\t    CREATE NEW ACCOUNT\n\n";
			cout << "\nNew account " << Text::red << "NOT" << Text::white << " written to file.\n";
			cout << "\nReturning to Account Settings Menu.";
		}

		Sleep(3000);
		userData.close();

		system("cls");
		showSplash();

		choice = showAcctMenu();
	}
	else
	{
		cout << "\n\n\n\n\n      I'm sorry, the maximum number of player accounts has been reached.\n";
		cout << "\t\t    Please delete a player to add another.\n\n\n\n\n\n";
		cout << "\a";
		system("pause");

		system("cls");
		showSplash();

		choice = showAcctMenu();
	}
}


void editAcct(fstream & userData, UserAcct player[], char & choice, int & count)
{
	//declare variables
	int	   index = 0,
		   user,
		   spaceFound = -1;
	bool   valid = false,
		   found = false;
	string uName,
		   pWord,
		   fName,
		   pWord1,
		   pWord2,
		   confirm = "NO";
	char   editChoice;

	system("cls");
	showSplash();
	cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
	cout << "\nPlease enter a user name that you want to edit: ";
	cin >> uName;

	for (index; index < count; index++)
	{
		if (uName == player[index].userName)
		{
			found = true;
			user = index;
		}
	}
	
	if (found)
	{
		system("cls");
		showSplash();
		cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
		cout << "\nPlease enter your password:";
		pWord = getPass(PASSWORD_WIDTH);
		
		if (pWord == player[user].password)
		{
			valid = true;
			
			system("cls");
			showSplash();
			fName = player[user].firstName;

			//display the entered data (hiding the password)
			cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
			cout << "User found.\n\n";
			cout << "Username:\t" << player[user].userName << endl;
			cout << "Password:\t[***************]" << endl;
			cout << "First name:\t" << player[user].firstName << endl <<endl;
			cout << "Enter \"1\" to change the password for this account.\n";
			cout << "Enter \"2\" to return to the account settings menu.\n";
			editChoice = _getch();

			while (editChoice != '2')
			{
				switch (editChoice)
				{
					case '1':	//edit password
						system("cls");
						showSplash();
					
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "Editing password.\n\n";
						cout << "\nEnter your password (15 characters or less): ";
						pWord1 = getPass(PASSWORD_WIDTH);

		
						//checking for spaces in the password
						spaceFound = pWord1.find(" " , 0);

						while (spaceFound != -1)
						{
								system("cls");
								showSplash();
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n";
								cout << Text::red << "\nYou cannot have spaces in your password." << Text::white;
								cout << "\nEnter your password (15 characters or less): ";
								cout << "\a";
								pWord1 = getPass(PASSWORD_WIDTH);
								spaceFound = -1;
								spaceFound = pWord1.find(" ", 0);
						}

						//verifying password
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "\nPlease re-enter your password: ";
						pWord2 = getPass(PASSWORD_WIDTH);
						system("cls");
						showSplash();

						//if the password was not entered the same between the first and
						//second time it was entered, give the user another chance to
						//correctly enter a password
						while (pWord1 != pWord2)
						{
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n";
							cout << Text::red << "\nPasswords did not match!" << Text::white;
							cout << "\nPlease enter your password (15 characters of less): ";
							cout << "\a";
							pWord1 = getPass(PASSWORD_WIDTH);
							//checking for spaces in the user name
							spaceFound = pWord1.find(" " , 0);

							while (spaceFound != -1)
							{
									system("cls");
									showSplash();
									cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n";
									cout << Text::red << "\nYou cannot have spaces in your password." << Text::white;
									cout << "\nEnter your password (15 characters or less): ";
									cout << "\a";
									pWord1 = getPass(PASSWORD_WIDTH);
									spaceFound = -1;
									spaceFound = pWord1.find(" ", 0);
							}

							//verifying password
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
							cout << "\nPlease re-enter your password: ";
							pWord2 = getPass(PASSWORD_WIDTH);
							system("cls");
							showSplash();

							//confirm change to password
							cout << "Are you sure you want to change this password?\n\n";
							cout << "Enter \"" << Text::red << "YES" << Text::white 
								<< "\" to confirm or anything else to keep user.\n";
							cin >> confirm;

			
							if (confirm == "YES")
							{
								system("cls");
								showSplash();
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
								cout << Text::red << "Editing user account...\n\n" << Text::white;
								cout << "Username:\t" << player[user].userName << endl;
								cout << "Password:\t[***************]" << endl;
								cout << "First name:\t" << player[user].firstName << endl <<endl;
							
								//opening the userData file in output mode
								userData.open("playerdata.dat", ios::out);
		
								//writing player array before the element to be edited to the file
								for (int index = 0; index < user; index++)
								{

									//write the computer user to the userData file
									userData << player[index].userName << "\n";
									userData << player[index].password << "\n";
									userData << player[index].firstName << "\n";
									userData << player[index].wins << "\n";
									userData << player[index].losses << "\n";
									userData << player[index].winPercent << "\n";
	
								}
	
								//writing player to be edited to the file
								userData << player[user].userName << "\n";
								userData << pWord1 << "\n";
								userData << player[user].firstName << "\n";
								userData << player[user].wins << "\n";
								userData << player[user].losses << "\n";
								userData << player[user].winPercent << "\n";
	
			
								//writing player array after the element to be edited to the file
								for (int index = user + 1; index < count; index++)
								{
				
									//write the computer user to the userData file
									userData << player[index].userName << "\n";
									userData << player[index].password << "\n";
									userData << player[index].firstName << "\n";
									userData << player[index].wins << "\n";
									userData << player[index].losses << "\n";
									userData << player[index].winPercent << "\n";
								}
			
								//closing the data file
								userData.close();
	
								//reopen the data file to reload the player array
								userData.open("playerdata.dat", ios::in);
	
								//reloading the userData file into the player array
								count = 0;					//resetting the count variable
	
								//read first player into player array
								getline(userData, player[count].userName);
								getline(userData, player[count].password);
								getline(userData, player[count].firstName);
								userData >> player[count].wins;
								userData >> player[count].losses;
								userData >> player[count].winPercent;
	
								count++;
	
								//while more players exist in the userData file,
								//continue reading players into the player array
								while (userData >> player[count].userName)
								{
									userData >> player[count].password;
									userData >> player[count].firstName;
									userData >> player[count].wins;
									userData >> player[count].losses;
									userData >> player[count].winPercent;
	
									count++;
			
								}
			
								//closing the data file
								userData.close();

								//call back to edit menu
								Sleep(2000);
								
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
								cout << "User found.\n\n";
								cout << "Username:\t" << player[user].userName << endl;
								cout << "Password:\t[***************]" << endl;
								cout << "First name:\t" << player[user].firstName << endl <<endl;
								cout << "Enter \"1\" to change the password for this account.\n";
								cout << "Enter \"2\" to return to the account settings menu.\n";
								editChoice = _getch();
								
	
							}
							else	
							{
								if (user > 0)
								{
									system("cls");
									showSplash();
									cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
									cout << Text::red << "NOT" << Text::white << " editing user account.\n\n";										cout << "Username:\t" << player[user].userName << endl;
									cout << "Password:\t[***************]" << endl;
									cout << "First name:\t" << player[user].firstName << endl <<endl;
								}

								//call back to edit menu
								Sleep(2000);
								
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
								cout << "User found.\n\n";
								cout << "Username:\t" << player[user].userName << endl;
								cout << "Password:\t[***************]" << endl;
								cout << "First name:\t" << player[user].firstName << endl <<endl;
								cout << "Enter \"1\" to change the password for this account.\n";
								cout << "Enter \"2\" to return to the account settings menu.\n";
								editChoice = _getch();
								
							}
							
						break;
					default :
						system("cls");
						showSplash();
					
						cout << Text::red << "\t\t      You have entered an invalid choice.\n" << Text::white;
						cout << "\a";
						cout << "\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "User found.\n\n";
						cout << "Username:\t" << player[user].userName << endl;
						cout << "Password:\t[***************]" << endl;
						cout << "First name:\t" << player[user].firstName << endl <<endl;
						cout << "Enter \"1\" to change the password for this account.\n";
						cout << "Enter \"2\" to return to the account settings menu.\n";
						editChoice = _getch();

					}
				}
	
					//confirm change to password
					cout << "Are you sure you want to change this password?\n\n";
					cout << "Enter \"" << Text::red << "YES" << Text::white 
						<< "\" to confirm or anything else to keep user.\n";
					cin >> confirm;

				
					if (confirm == "YES")
					{
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << Text::red << "Editing user account...\n\n" << Text::white;
						cout << "Username:\t" << player[user].userName << endl;
						cout << "Password:\t[***************]" << endl;
						cout << "First name:\t" << player[user].firstName << endl <<endl;
								
						//opening the userData file in output mode
						userData.open("playerdata.dat", ios::out);
						//writing player array before the element to be edited to the file
						for (int index = 0; index < user; index++)
						{
							//write the computer user to the userData file
							userData << player[index].userName << "\n";
							userData << player[index].password << "\n";
							userData << player[index].firstName << "\n";
							userData << player[index].wins << "\n";
							userData << player[index].losses << "\n";
							userData << player[index].winPercent << "\n";

						}
		
						//writing player to be edited to the file
						userData << player[user].userName << "\n";
						userData << pWord1 << "\n";
						userData << player[user].firstName << "\n";
						userData << player[user].wins << "\n";
						userData << player[user].losses << "\n";
						userData << player[user].winPercent << "\n";
	
			
						//writing player array after the element to be edited to the file
						for (int index = user + 1; index < count; index++)
						{
				
							//write the computer user to the userData file
							userData << player[index].userName << "\n";
							userData << player[index].password << "\n";
							userData << player[index].firstName << "\n";
							userData << player[index].wins << "\n";
							userData << player[index].losses << "\n";
							userData << player[index].winPercent << "\n";
						}
			
						//closing the data file
						userData.close();
	
						//reopen the data file to reload the player array
						userData.open("playerdata.dat", ios::in);
	
						//reloading the userData file into the player array
						count = 0;					//resetting the count variable

						//read first player into player array
						getline(userData, player[count].userName);
						getline(userData, player[count].password);
						getline(userData, player[count].firstName);
						userData >> player[count].wins;
						userData >> player[count].losses;
						userData >> player[count].winPercent;
	
						count++;
	
						//while more players exist in the userData file,
						//continue reading players into the player array
						while (userData >> player[count].userName)
						{
							userData >> player[count].password;
							userData >> player[count].firstName;
							userData >> player[count].wins;
							userData >> player[count].losses;
							userData >> player[count].winPercent;

							count++;
			
						}	
			
						//closing the data file
						userData.close();

						//call back to edit menu
						Sleep(2000);
						
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "User found.\n\n";
						cout << "Username:\t" << player[user].userName << endl;
						cout << "Password:\t[***************]" << endl;
						cout << "First name:\t" << player[user].firstName << endl <<endl;
						cout << "Enter \"1\" to change the password for this account.\n";
						cout << "Enter \"2\" to return to the account settings menu.\n";
						editChoice = _getch();
							
	
					}
					else	
					{
						if (user > 0)
						{
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
							cout << Text::red << "NOT" << Text::white << " editing user account.\n\n";										cout << "Username:\t" << player[user].userName << endl;
							cout << "Password:\t[***************]" << endl;
							cout << "First name:\t" << player[user].firstName << endl <<endl;
						}

							//call back to edit menu
							Sleep(2000);
								
						
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
							cout << "User found.\n\n";
							cout << "Username:\t" << player[user].userName << endl;
							cout << "Password:\t[***************]" << endl;
							cout << "First name:\t" << player[user].firstName << endl <<endl;
							cout << "Enter \"1\" to change the password for this account.\n";
							cout << "Enter \"2\" to return to the account settings menu.\n";
							editChoice = _getch();
									
					}

				}

					
		}
		else
		{
			system("cls");
			showSplash();
			int attempts = 1;
			while (attempts <= 3 && !(valid))
			{
				cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
				cout << Text::red << "The password was incorrect.\n" << Text::white << "Please re-enter password: ";
				cout << "\a";
				pWord = getPass(PASSWORD_WIDTH);
				attempts++;
		if (pWord == player[user].password)
		{
			valid = true;
			
			system("cls");
			showSplash();
			fName = player[user].firstName;

			//display the entered data (hiding the password)
			cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
			cout << "User found.\n\n";
			cout << "Username:\t" << player[user].userName << endl;
			cout << "Password:\t[***************]" << endl;
			cout << "First name:\t" << player[user].firstName << endl <<endl;
			cout << "Enter \"1\" to change the password for this account.\n";
			cout << "Enter \"2\" to return to the account settings menu.\n";
			editChoice = _getch();

			while (editChoice != '2')
			{
				switch (editChoice)
				{
					case '1':	//edit password
						system("cls");
						showSplash();
					
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "Editing password.\n\n";
						cout << "\nEnter your password (15 characters or less): ";
						pWord1 = getPass(PASSWORD_WIDTH);

		
						//checking for spaces in the password
						spaceFound = pWord1.find(" " , 0);

						while (spaceFound != -1)
						{
								system("cls");
								showSplash();
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n";
								cout << Text::red << "\nYou cannot have spaces in your password." << Text::white;
								cout << "\nEnter your password (15 characters or less): ";
								cout << "\a";
								pWord1 = getPass(PASSWORD_WIDTH);
								spaceFound = -1;
								spaceFound = pWord1.find(" ", 0);
						}

						//verifying password
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "\nPlease re-enter your password: ";
						pWord2 = getPass(PASSWORD_WIDTH);
						system("cls");
						showSplash();

						//if the password was not entered the same between the first and
						//second time it was entered, give the user another chance to
						//correctly enter a password
						while (pWord1 != pWord2)
						{
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n";
							cout << Text::red << "\nPasswords did not match!" << Text::white;
							cout << "\nPlease enter your password (15 characters of less): ";
							cout << "\a";
							pWord1 = getPass(PASSWORD_WIDTH);
							//checking for spaces in the user name
							spaceFound = pWord1.find(" " , 0);

							while (spaceFound != -1)
							{
									system("cls");
									showSplash();
									cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n";
									cout << Text::red << "\nYou cannot have spaces in your password." << Text::white;
									cout << "\nEnter your password (15 characters or less): ";
									cout << "\a";
									pWord1 = getPass(PASSWORD_WIDTH);
									spaceFound = -1;
									spaceFound = pWord1.find(" ", 0);
							}

							//verifying password
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
							cout << "\nPlease re-enter your password: ";
							pWord2 = getPass(PASSWORD_WIDTH);
							system("cls");
							showSplash();

							//confirm change to password
							cout << "Are you sure you want to change this password?\n\n";
							cout << "Enter \"" << Text::red << "YES" << Text::white 
								<< "\" to confirm or anything else to keep user.\n";
							cin >> confirm;

			
							if (confirm == "YES")
							{
								system("cls");
								showSplash();
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
								cout << Text::red << "Editing user account...\n\n" << Text::white;
								cout << "Username:\t" << player[user].userName << endl;
								cout << "Password:\t[***************]" << endl;
								cout << "First name:\t" << player[user].firstName << endl <<endl;
							
								//opening the userData file in output mode
								userData.open("playerdata.dat", ios::out);
		
								//writing player array before the element to be edited to the file
								for (int index = 0; index < user; index++)
								{

									//write the computer user to the userData file
									userData << player[index].userName << "\n";
									userData << player[index].password << "\n";
									userData << player[index].firstName << "\n";
									userData << player[index].wins << "\n";
									userData << player[index].losses << "\n";
									userData << player[index].winPercent << "\n";
	
								}
	
								//writing player to be edited to the file
								userData << player[user].userName << "\n";
								userData << pWord1 << "\n";
								userData << player[user].firstName << "\n";
								userData << player[user].wins << "\n";
								userData << player[user].losses << "\n";
								userData << player[user].winPercent << "\n";
	
			
								//writing player array after the element to be edited to the file
								for (int index = user + 1; index < count; index++)
								{
				
									//write the computer user to the userData file
									userData << player[index].userName << "\n";
									userData << player[index].password << "\n";
									userData << player[index].firstName << "\n";
									userData << player[index].wins << "\n";
									userData << player[index].losses << "\n";
									userData << player[index].winPercent << "\n";
								}
			
								//closing the data file
								userData.close();
	
								//reopen the data file to reload the player array
								userData.open("playerdata.dat", ios::in);
	
								//reloading the userData file into the player array
								count = 0;					//resetting the count variable
	
								//read first player into player array
								getline(userData, player[count].userName);
								getline(userData, player[count].password);
								getline(userData, player[count].firstName);
								userData >> player[count].wins;
								userData >> player[count].losses;
								userData >> player[count].winPercent;
	
								count++;
	
								//while more players exist in the userData file,
								//continue reading players into the player array
								while (userData >> player[count].userName)
								{
									userData >> player[count].password;
									userData >> player[count].firstName;
									userData >> player[count].wins;
									userData >> player[count].losses;
									userData >> player[count].winPercent;
	
									count++;
			
								}
			
								//closing the data file
								userData.close();

								//call back to edit menu
								Sleep(2000);
								
								system("cls");
								showSplash();
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
								cout << "User found.\n\n";
								cout << "Username:\t" << player[user].userName << endl;
								cout << "Password:\t[***************]" << endl;
								cout << "First name:\t" << player[user].firstName << endl <<endl;
								cout << "Enter \"1\" to change the password for this account.\n";
								cout << "Enter \"2\" to return to the account settings menu.\n";
								editChoice = _getch();
								
	
							}
							else	
							{
								if (user > 0)
								{
									system("cls");
									showSplash();
									cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
									cout << Text::red << "NOT" << Text::white << " editing user account.\n\n";										cout << "Username:\t" << player[user].userName << endl;
									cout << "Password:\t[***************]" << endl;
									cout << "First name:\t" << player[user].firstName << endl <<endl;
								}

								//call back to edit menu
								Sleep(2000);
								
								system("cls");
								showSplash();
								cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
								cout << "User found.\n\n";
								cout << "Username:\t" << player[user].userName << endl;
								cout << "Password:\t[***************]" << endl;
								cout << "First name:\t" << player[user].firstName << endl <<endl;
								cout << "Enter \"1\" to change the password for this account.\n";
								cout << "Enter \"2\" to return to the account settings menu.\n";
								editChoice = _getch();
								
							}
							
//						}

						break;
					default :
						system("cls");
						showSplash();
					
						cout << Text::red << "\t\t      You have entered an invalid choice.\n" << Text::white;
						cout << "\a";
						cout << "\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "User found.\n\n";
						cout << "Username:\t" << player[user].userName << endl;
						cout << "Password:\t[***************]" << endl;
						cout << "First name:\t" << player[user].firstName << endl <<endl;
						cout << "Enter \"1\" to change the password for this account.\n";
						cout << "Enter \"2\" to return to the account settings menu.\n";
						editChoice = _getch();

					}
				}
	
					//confirm change to password
					cout << "Are you sure you want to change this password?\n\n";
					cout << "Enter \"" << Text::red << "YES" << Text::white 
						<< "\" to confirm or anything else to keep user.\n";
					cin >> confirm;

				
					if (confirm == "YES")
					{
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << Text::red << "Editing user account...\n\n" << Text::white;
						cout << "Username:\t" << player[user].userName << endl;
						cout << "Password:\t[***************]" << endl;
						cout << "First name:\t" << player[user].firstName << endl <<endl;
								
						//opening the userData file in output mode
						userData.open("playerdata.dat", ios::out);
						//writing player array before the element to be edited to the file
						for (int index = 0; index < user; index++)
						{
							//write the computer user to the userData file
							userData << player[index].userName << "\n";
							userData << player[index].password << "\n";
							userData << player[index].firstName << "\n";
							userData << player[index].wins << "\n";
							userData << player[index].losses << "\n";
							userData << player[index].winPercent << "\n";

						}
		
						//writing player to be edited to the file
						userData << player[user].userName << "\n";
						userData << pWord1 << "\n";
						userData << player[user].firstName << "\n";
						userData << player[user].wins << "\n";
						userData << player[user].losses << "\n";
						userData << player[user].winPercent << "\n";
	
			
						//writing player array after the element to be edited to the file
						for (int index = user + 1; index < count; index++)
						{
				
							//write the computer user to the userData file
							userData << player[index].userName << "\n";
							userData << player[index].password << "\n";
							userData << player[index].firstName << "\n";
							userData << player[index].wins << "\n";
							userData << player[index].losses << "\n";
							userData << player[index].winPercent << "\n";
						}
			
						//closing the data file
						userData.close();
	
						//reopen the data file to reload the player array
						userData.open("playerdata.dat", ios::in);
	
						//reloading the userData file into the player array
						count = 0;					//resetting the count variable

						//read first player into player array
						getline(userData, player[count].userName);
						getline(userData, player[count].password);
						getline(userData, player[count].firstName);
						userData >> player[count].wins;
						userData >> player[count].losses;
						userData >> player[count].winPercent;
	
						count++;
	
						//while more players exist in the userData file,
						//continue reading players into the player array
						while (userData >> player[count].userName)
						{
							userData >> player[count].password;
							userData >> player[count].firstName;
							userData >> player[count].wins;
							userData >> player[count].losses;
							userData >> player[count].winPercent;

							count++;
			
						}	
			
						//closing the data file
						userData.close();

						//call back to edit menu
						Sleep(2000);
						
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << "User found.\n\n";
						cout << "Username:\t" << player[user].userName << endl;
						cout << "Password:\t[***************]" << endl;
						cout << "First name:\t" << player[user].firstName << endl <<endl;
						cout << "Enter \"1\" to change the password for this account.\n";
						cout << "Enter \"2\" to return to the account settings menu.\n";
						editChoice = _getch();
							
	
					}
					else	
					{
						if (user > 0)
						{
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
							cout << Text::red << "NOT" << Text::white << " editing user account.\n\n";										cout << "Username:\t" << player[user].userName << endl;
							cout << "Password:\t[***************]" << endl;
							cout << "First name:\t" << player[user].firstName << endl <<endl;
						}

							//call back to edit menu
							Sleep(2000);
								
						
							system("cls");
							showSplash();
							cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
							cout << "User found.\n\n";
							cout << "Username:\t" << player[user].userName << endl;
							cout << "Password:\t[***************]" << endl;
							cout << "First name:\t" << player[user].firstName << endl <<endl;
							cout << "Enter \"1\" to change the password for this account.\n";
							cout << "Enter \"2\" to return to the account settings menu.\n";
							editChoice = _getch();
									
					}

				}

					
		}
				else
				{
					system("cls");
					showSplash();

					if (attempts > 3)
					{
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
						cout << Text::red << "\nUser could not be validated.\n\n" << Text::white;
						cout << "\a";
						user = -1;
						Sleep(1000);
					}
				}
			}
		}

	}
	else
	{
		system("cls");
		showSplash();
		cout << "\n\n\t\t\t    EDIT EXISTING ACCOUNT\n\n";
		cout << Text::red << "\nThat user name cannot be found!\n\n" << Text::white;
		cout << "\a";
		user = -1;
	}


	cout << "Returning to Account Settings Menu....";
	Sleep(3000);
	system("cls");
	showSplash();

	choice = showAcctMenu();
}


void deleteAcct(fstream &userData, UserAcct player[], char & choice, int & count)
{
	//declare variables
	int	   index = 0,
		   user;
	bool   valid = false,
		   found = false;
	string uName,
		   pWord,
		   fName,
		   confirm = "NO";

	system("cls");
	showSplash();
	cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
	cout << "\nPlease enter a user name that you want to delete: ";
	cin >> uName;

	for (index; index < count; index++)
	{
		if (uName == player[index].userName)
		{
			found = true;
			user = index;
		}
	}
	
	if (found)
	{
		system("cls");
		showSplash();
		cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
		cout << "\nPlease enter your password:";
		pWord = getPass(PASSWORD_WIDTH);
		
		if (pWord == player[user].password)
		{
			valid = true;
			
			system("cls");
			showSplash();
			fName = player[user].firstName;

			//display the entered data (hiding the password)
			cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
			cout << "User found.\n\n";
			cout << "Username:\t" << player[user].userName << endl;
			cout << "Password:\t[***************]" << endl;
			cout << "First name:\t" << player[user].firstName << endl <<endl;
			cout << "Are you sure you want to delete this user?\n";
			cout << "This cannot be undone.\n\n";
			cout << "Enter \"" << Text::red << "YES" << Text::white 
				<< "\" to confirm or anything else to keep user.\n";
			cin >> confirm;
		}
		else
		{
			system("cls");
			showSplash();
			int attempts = 1;
			while (attempts <= 3 && !(valid))
			{
				cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
				cout << Text::red << "The password was incorrect.\nPlease re-enter password: " << Text::white;
				cout << "\a";
				pWord = getPass(PASSWORD_WIDTH);
				attempts++;
				if(pWord == player[user].password)
				{
					valid = true;
					system("cls");
					showSplash();
					fName = player[user].firstName;

					//display the entered data (hiding the password)
					cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
					cout << "User found.\n\n";
					cout << "Username:\t" << player[user].userName << endl;
					cout << "Password:\t[***************]" << endl;
					cout << "First name:\t" << player[user].firstName << endl <<endl;
					cout << "Are you sure you want to delete this user?\n";
					cout << "This cannot be undone.\n\n";
					cout << "Enter \"" << Text::red << "YES" << Text::white 
						<< "\" to confirm or anything else to keep user.\n";
					cin >> confirm;
				}
				else
				{
					system("cls");
					showSplash();

					if (attempts > 3)
					{
						system("cls");
						showSplash();
						cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
						cout << Text::red << "\nUser could not be validated.\n\n" << Text::white;
						cout << "\a";
						user = -1;
						Sleep(1000);
					}
				}
			}
		}

	}
	else
	{
		system("cls");
		showSplash();
		cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
		cout << Text::red << "\nThat user name cannot be found!\n\n" << Text::white;
		cout << "\a";
		user = -1;
	}

	if (confirm == "YES")
	{
		system("cls");
		showSplash();
		cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
		cout << Text::red << "Deleting user account...\n\n" << Text::white;
		cout << "Username:\t" << player[user].userName << endl;
		cout << "Password:\t[***************]" << endl;
		cout << "First name:\t" << player[user].firstName << endl <<endl;

		//trying something different
		player[user].userName = "qwertyuioplmnbvcxzasdfghjk";
		
		//opening the userData file in output mode
		userData.open("playerdata.dat", ios::out);
		
		//writing player array before the element to be deleted to the file
		for (int index = 0; index < user; index++)
		{

			//write the computer user to the userData file
			userData << player[index].userName << "\n";
			userData << player[index].password << "\n";
			userData << player[index].firstName << "\n";
			userData << player[index].wins << "\n";
			userData << player[index].losses << "\n";
			userData << player[index].winPercent << "\n";

		}
		
		//writing player array after the element to be deleted to the file
		for (int index = user + 1; index < count; index++)
		{
			
			//write the computer user to the userData file
			userData << player[index].userName << "\n";
			userData << player[index].password << "\n";
			userData << player[index].firstName << "\n";
			userData << player[index].wins << "\n";
			userData << player[index].losses << "\n";
			userData << player[index].winPercent << "\n";
		}
		
		//closing the data file
		userData.close();

		//reopen the data file to reload the player array
		userData.open("playerdata.dat", ios::in);

		//reloading the userData file into the player array
		count = 0;					//resetting the count variable

		//read first player into player array
		getline(userData, player[count].userName);
		getline(userData, player[count].password);
		getline(userData, player[count].firstName);
		userData >> player[count].wins;
		userData >> player[count].losses;
		userData >> player[count].winPercent;

		count++;

		//while more players exist in the userData file,
		//continue reading players into the player array
		while (userData >> player[count].userName)
		{
			userData >> player[count].password;
			userData >> player[count].firstName;
			userData >> player[count].wins;
			userData >> player[count].losses;
			userData >> player[count].winPercent;

			count++;
		
		}
		
		//closing the data file
		userData.close();

	}
	else	
	{
		if (user > 0)
		{
		system("cls");
		showSplash();
			cout << "\n\n\t\t\t   DELETE EXISTING ACCOUNT\n\n";
			cout << Text::red << "NOT" << Text::white << " deleting user account.\n\n";
			cout << "Username:\t" << player[user].userName << endl;
			cout << "Password:\t[***************]" << endl;
			cout << "First name:\t" << player[user].firstName << endl <<endl;
		}
	}
	
	cout << "Returning to Account Settings Menu....";
	Sleep(3000);
	system("cls");
	showSplash();

	choice = showAcctMenu();
}


//function - password handler
//This function accepts an integer for the password length and
//returns a string value.  The screen output has been hidden so
//you cannot see the characters being typed into the string value.
//	Adapted from code written by "Celtc" on
//	http://www.cplusplus.com/forum/general/33781/
string getPass(int width)
{
	cout << "[";
	for (int i =0; i < width - 1; i++)
		cout << "_";
	cout << "]";
	for (int i = 0; i < width; i++)
		cout << "\b";
	char letra, pass[PASSWORD_WIDTH];
	int i = 0;
	do
	{
		do
		{
			letra = _getch();
			if (letra != 8 && letra != 13 && i < width - 1)
			{
				pass[i] = letra;
				cout << "*";
				i++;
			}
			else if (letra == 8 && i > 0)
			{
				cout << "\b" << "_" << "\b";
				i--;
			}
		}
		while (letra != 13 && i < width - 1);
		if (letra != 13)
		{
			letra = _getch();
			if (letra == 8 && i > 0)
			{
				cout << "\b" << "_" << "\b";
				i--;
			}
		}
	}
	while (letra != 13);
	pass[i] = '\0';
	return pass;
}


//this function takes an integer value from the show board function
//and returns a row designation (A - I) to display on the board
char showRowLetter(int rowNumber)
{
	if (rowNumber == 0)
		return 'A';
	if (rowNumber == 1)
		return 'B';
	if (rowNumber == 2)
		return 'C';
	if (rowNumber == 3)
		return 'D';
	if (rowNumber == 4)
		return 'E';
	if (rowNumber == 5)
		return 'F';
	if (rowNumber == 6)
		return 'G';
	if (rowNumber == 7)
		return 'H';
	if (rowNumber == 8)
		return 'I';
}


//this function takes an integer value from the show board function
//and returns a string that shows the status of the players' ships
// between the game boards
string showNeutralGround(int rowNumber, int c1, int b1, int s1, int d1, int p1,
						 int c2, int b2, int s2, int d2, int p2)
{
	if (rowNumber == 0)
		return "        SHIPS:         ";
	if (rowNumber == 1)
		return "                       ";
	if (rowNumber == 2 && c1 == CARRIER_SIZE && c2 == CARRIER_SIZE)
		return "    X  CARRIER    X    ";
	if (rowNumber == 2 && c1 == CARRIER_SIZE && c2 < CARRIER_SIZE)
		return "    X  CARRIER         ";
	if (rowNumber == 2 && c1 < CARRIER_SIZE && c2 == CARRIER_SIZE)
		return "       CARRIER    X    ";
	if (rowNumber == 2 && c1 < CARRIER_SIZE && c2 < CARRIER_SIZE)
		return "       CARRIER         ";
	if (rowNumber == 3 && b1 == BATTLESHIP_SIZE && b2 == BATTLESHIP_SIZE)
		return "    X BATTLESHIP  X    ";
	if (rowNumber == 3 && b1 == BATTLESHIP_SIZE && b2 < BATTLESHIP_SIZE)
		return "    X BATTLESHIP       ";
	if (rowNumber == 3 && b1 < BATTLESHIP_SIZE && b2 == BATTLESHIP_SIZE)
		return "      BATTLESHIP  X    ";
	if (rowNumber == 3 && b1 < BATTLESHIP_SIZE && b2 < BATTLESHIP_SIZE)
		return "      BATTLESHIP       ";
	if (rowNumber == 4 && s1 == SUBMARINE_SIZE && s2 == SUBMARINE_SIZE)
		return "    X  SUBMARINE  X    ";
	if (rowNumber == 4 && s1 == SUBMARINE_SIZE && s2 < SUBMARINE_SIZE)
		return "    X  SUBMARINE       ";
	if (rowNumber == 4 && s1 < SUBMARINE_SIZE && s2 == SUBMARINE_SIZE)
		return "       SUBMARINE  X    ";
	if (rowNumber == 4 && s1 < SUBMARINE_SIZE && s2 < SUBMARINE_SIZE)
		return "       SUBMARINE       ";
	if (rowNumber == 5 && d1 == DESTROYER_SIZE && d2 == DESTROYER_SIZE)
		return "    X  DESTROYER  X    ";
	if (rowNumber == 5 && d1 == DESTROYER_SIZE && d2 < DESTROYER_SIZE)
		return "    X  DESTROYER       ";
	if (rowNumber == 5 && d1 < DESTROYER_SIZE && d2 == DESTROYER_SIZE)
		return "       DESTROYER  X    ";
	if (rowNumber == 5 && d1 < DESTROYER_SIZE && d2 < DESTROYER_SIZE)
		return "       DESTROYER       ";
	if (rowNumber == 6 && p1 == PATROLBOAT_SIZE && p2 == PATROLBOAT_SIZE)
		return "    X PATROL BOAT X    ";
	if (rowNumber == 6 && p1 == PATROLBOAT_SIZE && p2 < PATROLBOAT_SIZE)
		return "    X PATROL BOAT      ";
	if (rowNumber == 6 && p1 < PATROLBOAT_SIZE && p2 == PATROLBOAT_SIZE)
		return "      PATROL BOAT X    ";
	if (rowNumber == 6 && p1 < PATROLBOAT_SIZE && p2 < PATROLBOAT_SIZE)
		return "      PATROL BOAT      ";
	if (rowNumber == 7)
		return "                       ";
	if (rowNumber == 8)
		return "                       ";
}


//this function receives an integer value from the player's game board array
//and returns the character to display on the game board while placing the 
//ships
char showVisible(int coordinate)
{
	if (coordinate == 0)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 1)
	{
		cout << Text::gray;
		return 'C';
	}
	if (coordinate == 2)
	{
		cout << Text::gray;
		return 'B';
	}
	if (coordinate == 3)
	{
		cout << Text::gray;
		return 'S';
	}
	if (coordinate == 4)
	{
		cout << Text::gray;
		return 'D';
	}
	if (coordinate == 5)
	{
		cout << Text::gray;
		return 'P';
	}
	if (coordinate == 100)
	{
		cout << Text::white;
		return 'O';
	}
	if (coordinate > 100)
	{
		cout << Text::red;
		return 'X';
	}
}


//this function receives an integer value from the player's game board array
//and returns the character to display on the game board during gameplay
char showHidden(int coordinate)
{
	if (coordinate == 0)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 1)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 2)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 3)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 4)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 5)
	{
		cout << Text::blue;
		return '-';
	}
	if (coordinate == 100)
	{
		cout << Text::white;
		return 'O';
	}
	if (coordinate == 101)
	{
		cout << Text::red;
		return 'X';
	}
	if (coordinate == 102)
	{
		cout << Text::red;
		return 'X';
	}
	if (coordinate == 103)
	{
		cout << Text::red;
		return 'X';
	}
	if (coordinate == 104)
	{
		cout << Text::red;
		return 'X';
	}
	if (coordinate == 105)
	{
		cout << Text::red;
		return 'X';
	}
}



//this function will be used to place ships on the player's game board
void placeShip(int p1Board [][COLS], int p2Board[][COLS],
			   bool hidePlayer1, bool hidePlayer2, string fName1, string fName2,
			   string shipName, int currentPlayer, int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
						 int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	char orientation,
		 rowLocation,
		 colLocation;
	int rCoord,			        //integer varaible to hold array locations
		cCoord;			        //integer variable to hold array locations
    bool overlap;               //variable to search for overlapping ships

    do
	{
		orientation = getOrientation(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, shipName,
			p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
	    rowLocation = getRowLocation(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, shipName, orientation,
			p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
	    colLocation = getColLocation(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2, shipName, orientation,
			p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

        //checking for overlapping ships
		rCoord = characterToInteger(rowLocation);           //converting coordinates to integers for comparison
		cCoord = characterToInteger(colLocation);           //with array elements

        if (currentPlayer == 1)
		{
            if (orientation == 'H')
			{
				overlap = overlappingHShips(p1Board, rCoord, cCoord, shipName);
			}
			else
			{
                overlap = overlappingVShips(p1Board, rCoord, cCoord, shipName);
			}
		}

        if (currentPlayer == 2)
        {
            if (orientation == 'H')
			{
				overlap = overlappingHShips(p2Board, rCoord, cCoord, shipName);
			}
			else
			{
                overlap = overlappingVShips(p2Board, rCoord, cCoord, shipName);
			}
		}

		if (overlap)
        {
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << Text::red << "You cannot have overlapping ships." << Text::white 
				<< "\nPlease re-enter your coordinates for your " << shipName << ".\n";
			cout << "\a";
            system("pause");
		}
           
	} while (overlap);
	
	system("cls");
	showSplash();
	showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
		p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

	//checking for ships that don't fit on the map and correcting locations before
	//entering coordinates on the player's game board
	if (shipName == "Carrier")
	{
		if (orientation == 'H')     //for horizontally placed Carriers
		{
			if (colLocation > '5')
			{
				colLocation = '5';
				cout << "\nYour " << shipName << " didn't fit at those coordinates, so I moved it over to "
					<< rowLocation << ":" << colLocation;
				Sleep(3000);
			}
			
            //converting chars to ints for ship placement in board array
			cCoord = characterToInteger(colLocation);
			rCoord = characterToInteger(rowLocation);
            
            //placing ship in board array
			for (int shipStart = cCoord; shipStart < cCoord + 5; shipStart++)
			{
				if (currentPlayer == 1)
				{
					p1Board [rCoord][shipStart] = 1;
				}
				if (currentPlayer == 2)
				{ 
					p2Board [rCoord][shipStart] = 1;
				}
			}
		}
		else                        //for vertically placed Carriers
		{
			if (rowLocation > 'E')
			{
				rowLocation = 'E';
				cout << "\nYour " << shipName << " didn't fit at those coordinates, so I moved it up to "
					<< rowLocation << ":" << colLocation;
				Sleep(3000);
			}
            //converting chars to ints for ship placement in board array
			cCoord = characterToInteger(colLocation);
			rCoord = characterToInteger(rowLocation);
			
            //placing ship in board array
			for (int shipStart = rCoord; shipStart < rCoord + 5; shipStart++)
			{
				if (currentPlayer == 1)
				{
					p1Board [shipStart][cCoord] = 1;
				}
				if (currentPlayer == 2)
				{
					p2Board [shipStart][cCoord] = 1;
				}
			}
		}
	}
	else if (shipName == "Battleship")
	{
		if (orientation == 'H')     //for horizontally placed Battleships
		{
			//converting chars to ints for ship placement in board array
	    	cCoord = characterToInteger(colLocation);
	    	rCoord = characterToInteger(rowLocation);
                
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 4; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 2;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 2;
			    }
			}
		}
		else                        //for vertically placed Battleships
		{
			//converting chars to ints for ship placement in board array
			cCoord = characterToInteger(colLocation);
			rCoord = characterToInteger(rowLocation);
			
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 4; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 2;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 2;
				}

			}
		}

	}
	else if (shipName == "Submarine")
	{
		if (orientation == 'H')     //for horizontally placed Submarines
		{
			//converting chars to ints for ship placement in board array
	    	cCoord = characterToInteger(colLocation);
	    	rCoord = characterToInteger(rowLocation);
                
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 3; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 3;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 3;
			    }
			}
		}
		else                        //for vertically placed Submarines
		{
			//converting chars to ints for ship placement in board array
			cCoord = characterToInteger(colLocation);
			rCoord = characterToInteger(rowLocation);
			
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 3; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 3;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 3;
				}

			}
		}

	}
	else if (shipName == "Destroyer")
	{
		if (orientation == 'H')     //for horizontally placed Destroyers
		{
			//converting chars to ints for ship placement in board array
	    	cCoord = characterToInteger(colLocation);
	    	rCoord = characterToInteger(rowLocation);
                
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 3; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 4;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 4;
			    }
			}
		}
		else                        //for vertically placed Destroyers
		{
			//converting chars to ints for ship placement in board array
			cCoord = characterToInteger(colLocation);
			rCoord = characterToInteger(rowLocation);
			
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 3; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 4;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 4;
				}

			}
		}

	}
	else  //the only ship left is Patrol Boat
	{
		if (orientation == 'H')     //for horizontally placed Patrol Boats
		{
			//converting chars to ints for ship placement in board array
	    	cCoord = characterToInteger(colLocation);
	    	rCoord = characterToInteger(rowLocation);
                
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 2; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 5;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 5;
			    }
			}
		}
		else                        //for vertically placed Patrol Boats
		{
			//converting chars to ints for ship placement in board array
			cCoord = characterToInteger(colLocation);
			rCoord = characterToInteger(rowLocation);
			
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 2; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 5;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 5;
				}

			}
		}

	}

	system("cls");
	showSplash();
	showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
		p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

	cout << "\nShip orientation: ";
	if (orientation == 'H')
		cout << "Horizontal\n";
	else
		cout <<"Vertical\n";
	cout << "Ship coordinates: " << rowLocation << ":" << colLocation << endl;
	system("pause");
}


//this function gets the ship orientation
char getOrientation(int p1Board [][COLS], int p2Board[][COLS], bool hidePlayer1, bool hidePlayer2, 
					string fName1, string fName2, string shipName, int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
						 int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	char orientation;
	
	system("cls");
	showSplash();
	showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
		p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
	cout << endl << "To place your " << shipName << " on the board.\n";
	cout << "Choose \"H\" for Horizontal or \"V\" for vertical: ";
	orientation = _getch();

	while (toupper(orientation) != 'H' && toupper(orientation) != 'V')
	{
		system("cls");
		showSplash();
		showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
			p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
		cout << Text::red << "\nInvalid selection!  You must enter only \"H\" or \"V\"." << Text::white;
		cout << "\nTo place your " << shipName << " on the board,\n";
		cout << "Choose \"H\" for Horizontal or \"V\" for vertical: ";
		cout << "\a";
		orientation = _getch();
	}

	orientation = toupper(orientation);
	return orientation;
}


//this function gets the ship's beginning row location
char getRowLocation(int p1Board [][COLS], int p2Board[][COLS], bool hidePlayer1, bool hidePlayer2, 
					string fName1, string fName2, string shipName, char orient,
					int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
					int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	char rowLocation;
	
	if (shipName == "Carrier")
	{
		system("cls");
		showSplash();
		showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
			p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
		cout << endl << "To place your " << shipName << " on the board.\n";
		cout << "Choose a letter between \"A\" and \"I\": ";
		rowLocation = _getch();

		while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
			 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
			 && toupper(rowLocation) != 'G' && toupper(rowLocation) != 'H' && toupper(rowLocation) != 'I')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"I\"." << Text::white;
			cout << "\nTo place your " << shipName << " on the board,\n";
			cout << "Choose a letter between \"A\" and \"I\": ";
			cout << "\a";
			rowLocation = _getch();
		}
	
		rowLocation = toupper(rowLocation);
		return rowLocation;
	}
	
	if (shipName == "Battleship")
	{
		if (orient == 'H')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"I\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G' && toupper(rowLocation) != 'H' && toupper(rowLocation) != 'I')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"I\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"I\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"F\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				&& toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"F\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"F\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
	}

	if (shipName == "Submarine")
	{
		if (orient == 'H')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"I\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G' && toupper(rowLocation) != 'H' && toupper(rowLocation) != 'I')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"I\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"I\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"G\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"G\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"G\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
	}

	if (shipName == "Destroyer")
	{
		if (orient == 'H')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"I\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G' && toupper(rowLocation) != 'H' && toupper(rowLocation) != 'I')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"I\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"I\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"G\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"G\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"G\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
	}

	if (shipName == "Patrol Boat")
	{
		if (orient == 'H')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"I\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G' && toupper(rowLocation) != 'H' && toupper(rowLocation) != 'I')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"I\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"I\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a letter between \"A\" and \"H\": ";
			rowLocation = _getch();

			while (toupper(rowLocation) != 'A' && toupper(rowLocation) != 'B' && toupper(rowLocation) != 'C'
				 && toupper(rowLocation) != 'D' && toupper(rowLocation) != 'E' && toupper(rowLocation) != 'F'
				 && toupper(rowLocation) != 'G' && toupper(rowLocation) != 'H')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"H\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a letter between \"A\" and \"H\": ";
				cout << "\a";
				rowLocation = _getch();
			}
	
			rowLocation = toupper(rowLocation);
			return rowLocation;
		}
	}
}


//this function gets the ship's beginning row location
char getColLocation(int p1Board [][COLS], int p2Board[][COLS], bool hidePlayer1, bool hidePlayer2, 
					string fName1, string fName2, string shipName, char orient,
					int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
					int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	char colLocation;
	
	if (shipName == "Carrier")
	{
		system("cls");
		showSplash();
		showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
			p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
		cout << endl << "To place your " << shipName << " on the board.\n";
		cout << "Choose a number between \"1\" and \"9\": ";
		colLocation = _getch();

		while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7' && colLocation != '8' && colLocation != '9')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"9\"." << Text::white;
			cout << "\nTo place your " << shipName << " on the board,\n";
			cout << "Choose a number between \"1\" and \"9\": ";
			cout << "\a";
			colLocation = _getch();
		}

		return colLocation;
	}

	if (shipName == "Battleship")
	{
		if (orient == 'V')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"9\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7' && colLocation != '8' && colLocation != '9')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"9\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"9\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"6\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"6\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"6\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
	}
	
	if (shipName == "Submarine")
	{
		if (orient == 'V')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"9\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7' && colLocation != '8' && colLocation != '9')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"9\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"9\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"7\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"7\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"7\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
	}

	if (shipName == "Destroyer")
	{
		if (orient == 'V')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"9\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7' && colLocation != '8' && colLocation != '9')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"9\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"9\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"7\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"7\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"7\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
	}

	if (shipName == "Patrol Boat")
	{
		if (orient == 'V')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"9\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7' && colLocation != '8' && colLocation != '9')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"9\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"9\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
		else
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << endl << "To place your " << shipName << " on the board.\n";
			cout << "Choose a number between \"1\" and \"8\": ";
			colLocation = _getch();

			while (colLocation != '1' && colLocation != '2' && colLocation != '3'
			&& colLocation != '4' && colLocation != '5' && colLocation != '6'
			&& colLocation != '7' && colLocation != '8')
			{
				system("cls");
				showSplash();
				showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
					p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
				cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"8\"." << Text::white;
				cout << "\nTo place your " << shipName << " on the board,\n";
				cout << "Choose a number between \"1\" and \"8\": ";
				cout << "\a";
				colLocation = _getch();
			}

			return colLocation;
		}
	}
}


//this function accepts char variables and converts them to ints to enable
//entering the ship coordinates into a 2-dimensional array of ints
int characterToInteger(char ch)
{
	if (ch == 'A')
		return 0;
	if (ch == 'B')
		return 1;
	if (ch == 'C')
		return 2;
	if (ch == 'D')
		return 3;
	if (ch == 'E')
		return 4;
	if (ch == 'F')
		return 5;
	if (ch == 'G')
		return 6;
	if (ch == 'H')
		return 7;
	if (ch == 'I')
		return 8;
	if (ch == '1')
		return 0;
	if (ch == '2')
		return 1;
	if (ch == '3')
		return 2;
	if (ch == '4')
		return 3;
	if (ch == '5')
		return 4;
	if (ch == '6')
		return 5;
	if (ch == '7')
		return 6;
	if (ch == '8')
		return 7;
	if (ch == '9')
		return 8;
}


//this function accepts int variables and converts them to chars to enable
//displaying the computer's chosen row coordinates during game play
char rowIntegerToCharacter(int num)
{
	if (num == 0)
		return 'A';
	if (num == 1)
		return 'B';
	if (num == 2)
		return 'C';
	if (num == 3)
		return 'D';
	if (num == 4)
		return 'E';
	if (num == 5)
		return 'F';
	if (num == 6)
		return 'G';
	if (num == 7)
		return 'H';
	if (num == 8)
		return 'I';
}


//this function accepts int variables and converts them to chars to enable
//displaying the computer's chosen column coordinates during game play
char colIntegerToCharacter(int num)
{
	if (num == 0)
		return '1';
	if (num == 1)
		return '2';
	if (num == 2)
		return '3';
	if (num == 3)
		return '4';
	if (num == 4)
		return '5';
	if (num == 5)
		return '6';
	if (num == 6)
		return '7';
	if (num == 7)
		return '8';
	if (num == 8)
		return '9';
}



//this function compares newly placed horizontal ships to the ships
//that have already been placed in the player's board.  If the 
//ships overlap, it returns true.
bool overlappingHShips(int board[][COLS], int rCoord, int cCoord, string shipName)
{
    int spaces;

    if (shipName == "Carrier")
        return false;
    if (shipName == "Battleship")
        spaces = 4;
    if (shipName == "Submarine")
        spaces = 3;
    if (shipName == "Destroyer")
        spaces = 3;
    if (shipName == "Patrol Boat")
        spaces = 2;

    for (int shipStart = cCoord; shipStart < cCoord + spaces; shipStart++)
    {
		if (board[rCoord][shipStart] != 0)
			return true;
    }
    return false;
}



//this function compares newly placed vertical ships to the ships
//that have already been placed in the player's board.  If the 
//ships overlap, it returns true.
bool overlappingVShips(int board[][COLS], int rCoord, int cCoord, string shipName)
{
    int spaces;

    if (shipName == "Carrier")
        return false;
    if (shipName == "Battleship")
        spaces = 4;
    if (shipName == "Submarine")
        spaces = 3;
    if (shipName == "Destroyer")
        spaces = 3;
    if (shipName == "Patrol Boat")
        spaces = 2;

    for (int shipStart = rCoord; shipStart < rCoord + spaces; shipStart++)
    {
		if (board[shipStart][cCoord] != 0)
			return true;
    }
	return false;
}


//this function check for the existance of a user name before adding a new
//user name to the player system
bool userCheck(UserAcct player[], string uName, int count)
{
	for (int index = 0; index < count; index++)
	{
		if (player[index].userName == uName)
			return true;
	}
	return false;
}


//this function lists all user names in the player array
void showUsers(UserAcct player[], char & choice, int count)
{
	system("cls");
	cout << Text::white;
	cout << "\n\n\t\t\t     EXISTING USER ACCOUNTS\n\n";
	cout << "\n";
	cout << "\t\t\tWin Percent\t\tUser Name\n\n";
	cout << setprecision(2) << fixed;

	for (int index = 1; index < count; index++)
	{
		cout << "\t\t\t  " << setw(5) << player[index].winPercent << "%\t\t" << player[index].userName << endl;
	}
	
	cout << endl;
	system("pause");

	system("cls");
	showSplash();

	choice = showAcctMenu();
}



//this function gets a row coordinate from the players during gameplay
char getRowCoord(int p1Board [][COLS], int p2Board[][COLS], bool hidePlayer1, bool hidePlayer2, 
				string fName1, string fName2, int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
						int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	//declare local variable
	char coordinate;

	cout << "choose a letter coordinate between \"A\" and \"I\": "; 
	coordinate = _getch();

//	coordinate = toupper(coordinate);	//I was trying to get the row coordinate to display in CAPITALS, but
										//whatever I did broke the game... I'll come back to this later
//		while (coordinate != 'A' && coordinate != 'B' && coordinate != 'C'
//				&& coordinate != 'D' && coordinate != 'E' && coordinate != 'F'
//				&& coordinate != 'G' && coordinate != 'H' && coordinate != 'I')
		while (toupper(coordinate) != 'A' && toupper(coordinate) != 'B' && toupper(coordinate) != 'C'
				&& toupper(coordinate) != 'D' && toupper(coordinate) != 'E' && toupper(coordinate) != 'F'
				&& toupper(coordinate) != 'G' && toupper(coordinate) != 'H' && toupper(coordinate) != 'I')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << Text::red << "\nInvalid selection!  You must enter only \"A\" through \"I\".\n" << Text::white;
			cout << "Choose a letter coordinate between \"A\" and \"I\": ";
			cout << "\a";
			coordinate = _getch();

//			coordinate = toupper(coordinate);
		}
	return coordinate;	
}



//this function gets a row coordinate from the players during gameplay
char getColCoord(int p1Board [][COLS], int p2Board[][COLS], bool hidePlayer1, bool hidePlayer2, 
				string fName1, string fName2, int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
						int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	char coordinate;

	cout << "choose a number coordinate between \"1\" and \"9\": "; 
	coordinate = _getch();

		while (coordinate != '1' && coordinate != '2' && coordinate != '3'
				&& coordinate != '4' && coordinate != '5' && coordinate != '6'
				&& coordinate != '7' && coordinate != '8' && coordinate != '9')
		{
			system("cls");
			showSplash();
			showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
				p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);
			cout << Text::red << "\nInvalid selection!  You must enter only \"1\" through \"9\".\n" << Text::white;
			cout << "Choose a number coordinate between \"1\" and \"9\": ";
			cout << "\a";
			coordinate = _getch();
		}
	return coordinate;
}


//this function will give feedback during game play whether
//a chosen coordinate is a hit or a miss
void showHitMiss(char rowCoord, char colCoord, int rowArray, int colArray,
	int currentPlayer, int p1Board[][COLS], int p2Board[][COLS])
{
	cout << endl << rowCoord << ":" << colCoord << " is a ";
	{
		if (currentPlayer == 1)
		{
			if (p2Board[rowArray][colArray] == 100)
				cout << Text::red << "MISS!" << Text::white;
			else
				cout << Text::red << "HIT!" << Text::white;
		}
		
		//current player is Player 2
		else
		{
			if (p1Board[rowArray][colArray] == 100)
				cout << Text::red << "MISS!" << Text::white;
			else
				cout << Text::red << "HIT!" << Text::white;
		}
	}

	Sleep(2000);
}


//this function will be used to automatically place ships on the player's game board
void autoPlaceShip(int p1Board [][COLS], int p2Board[][COLS],
			   bool hidePlayer1, bool hidePlayer2, string fName1, string fName2,
			   string shipName, int currentPlayer, int p1Carr, int p1Batt, int p1Sub, int p1Des, int p1Pat,
						 int p2Carr, int p2Batt, int p2Sub, int p2Des, int p2Pat)
{
	int  rCoord,		        //integer varaible to hold array locations
		 cCoord,		        //integer variable to hold array locations
		 randomOrientation;		//integer to hold orientation (0 = horizontal and 1 = vertical)
	char orientation;			//some functions require a char variable for orientation (H or V)
    bool overlap;               //variable to search for overlapping ships

    do
	{
		//getting random orientation for each ship
		randomOrientation = rand() % 2;
		if (randomOrientation == 0)
			orientation = 'H';
		else
			orientation = 'V';

		//getting the random starting point for each ship
		if (shipName == "Carrier" && orientation == 'H')
		{
			rCoord = rand() % 9;
			cCoord = rand() % 4;
		}
		else if (shipName == "Carrier" && orientation == 'V')
		{
			rCoord = rand() % 4;
			cCoord = rand() % 9;
		}
		else if (shipName == "Battleship" && orientation == 'H')
		{
			rCoord = rand() % 9;
			cCoord = rand() % 5;
		}
		else if (shipName == "Battleship" && orientation == 'V')
		{
			rCoord = rand() % 5;
			cCoord = rand() % 9;
		}
		else if (shipName == "Submarine" && orientation == 'H')
		{
			rCoord = rand() % 9;
			cCoord = rand() % 6;
		}
		else if (shipName == "Submarine" && orientation == 'V')
		{
			rCoord = rand() % 6;
			cCoord = rand() % 9;
		}
		else if (shipName == "Destroyer" && orientation == 'H')
		{
			rCoord = rand() % 9;
			cCoord = rand() % 6;
		}
		else if (shipName == "Destroyer" && orientation == 'V')
		{
			rCoord = rand() % 6;
			cCoord = rand() % 9;
		}
		else if (shipName == "Patrol Boat" && orientation == 'H')
		{
			rCoord = rand() % 9;
			cCoord = rand() % 7;
		}
		else  //only option left is a Patrol Boat in a vertical orientation
		{
			rCoord = rand() % 7;
			cCoord = rand() % 9;
		}

        //checking for overlapping ships
        if (currentPlayer == 1)
		{
            if (orientation == 'H')
			{
				overlap = overlappingHShips(p1Board, rCoord, cCoord, shipName);
			}
			else
			{
                overlap = overlappingVShips(p1Board, rCoord, cCoord, shipName);
			}
		}

        if (currentPlayer == 2)
        {
            if (orientation == 'H')
			{
				overlap = overlappingHShips(p2Board, rCoord, cCoord, shipName);
			}
			else
			{
                overlap = overlappingVShips(p2Board, rCoord, cCoord, shipName);
			}
		}

	} while (overlap);
	
	system("cls");
	showSplash();
	showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
		p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

	if (shipName == "Carrier")
	{
		if (orientation == 'H')     //for horizontally placed carriers
		{
            //placing ship in board array
			for (int shipStart = cCoord; shipStart < cCoord + 5; shipStart++)
			{
				if (currentPlayer == 1)
				{
					p1Board [rCoord][shipStart] = 1;
				}
				if (currentPlayer == 2)
				{ 
					p2Board [rCoord][shipStart] = 1;
				}
			}
		}
		else                        //for vertically placed carriers
		{
			for (int shipStart = rCoord; shipStart < rCoord + 5; shipStart++)
			{
				if (currentPlayer == 1)
				{
					p1Board [shipStart][cCoord] = 1;
				}
				if (currentPlayer == 2)
				{
					p2Board [shipStart][cCoord] = 1;
				}
			}
		}
	}
	else if (shipName == "Battleship")
	{
		if (orientation == 'H')     //for horizontally placed battleships
		{
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 4; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 2;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 2;
			    }
			}
		}
		else                        //for vertically placed battleships
		{
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 4; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 2;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 2;
				}

			}
		}

	}
	else if (shipName == "Submarine")
	{
		if (orientation == 'H')     //for horizontally placed Submarines
		{
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 3; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 3;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 3;
			    }
			}
		}
		else                        //for vertically placed Submarines
		{
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 3; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 3;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 3;
				}

			}
		}

	}
	else if (shipName == "Destroyer")
	{
		if (orientation == 'H')     //for horizontally placed Destroyers
		{
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 3; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 4;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 4;
			    }
			}
		}
		else                        //for vertically placed Destroyers
		{
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 3; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 4;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 4;
				}

			}
		}

	}
	else  //the only ship left is Patrol Boat
	{
		if (orientation == 'H')     //for horizontally placed Patrol Boats
		{
            //placing ship in board array
	    	for (int shipStart = cCoord; shipStart < cCoord + 2; shipStart++)
			{
			    if (currentPlayer == 1)
			    {
			    	p1Board [rCoord][shipStart] = 5;
			    }
			    if (currentPlayer == 2)
			    { 
			    	p2Board [rCoord][shipStart] = 5;
			    }
			}
		}
		else                        //for vertically placed Patrol boats
		{
            //placing ship in board array
		    for (int shipStart = rCoord; shipStart < rCoord + 2; shipStart++)
			{
				if (currentPlayer == 1)
	    		{
		    		p1Board [shipStart][cCoord] = 5;
			    }
				if (currentPlayer == 2)
				{
                    p2Board [shipStart][cCoord] = 5;
				}

			}
		}

	}

	system("cls");
	showSplash();
	showBoard(p1Board, p2Board, hidePlayer1, hidePlayer2, fName1, fName2,
		p1Carr, p1Batt, p1Sub, p1Des, p1Pat, p2Carr, p2Batt, p2Sub, p2Des, p2Pat);

	cout << endl << shipName << " has been randomly placed on the game board.";
}
