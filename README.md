battleship
==========

Name: Matthew Stewart<br>
Date: December 4, 2013<br>
Program: C++ I Final Project - Battleship<br>
Description:<br>
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
of-bounds or in overlapping positions, will be rejected.  After both players
have placed their	ships on the board, one of the players is randomly chosen
to go first. Each player takes turns to choose coordinates on the opposing game
board.  When a ship has been hit it is indicated with an "X" on the 
game board and a miss is indicated with an "O".  When a ship is sunk,
an "X" appears next to that player's ship, shown between the two game
boards.  Once one player has all of his/her ships sunk, the other player
is announced as the winner and the game stats are updated in the
playerdata file.  After the game is over, the players are prompted for
a rematch or to quit the game.  In a single-player game, all of the 
player 2 functions are carried out through the use of the random function.

Please note that this program was written before we covered object-oriented
programming and as a result the source code is long and full of functions.
However, despite the clunky nature of the code, it works as intended.
Special thanks to a classmate, Jason Weeks, for letting us use his utilities.cpp
include to add a little color to our programs.

Also note that due to the windows-specific includes, this program probably will
not work properly on a Mac. Sorry about that.
