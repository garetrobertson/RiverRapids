// ART INSTITUTE OF VANCOUVER:
// VGP 120: PROGAMMING IN C II FINAL ASSIGNMENT
// RIVER RAPIDS: PROGRAMMED BY JOSH CAMPBELL, GARET ROBERTSON
// THANKS FOR PLAYING! 


#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <stdbool.h>

#define consoleWidth 80

#define points 10

#define ROWS 25
#define COLS 80

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define LAND '|'
#define RIVER '~'
#define OBSTACLE '*'


// colors dude 
// make shit


#define BLACK			00
#define DARK_BLUE		01
#define DARK_GREEN		02
#define TEAL			03
#define DARK_RED		04
#define DARK_PINK		05
#define DARK_YELLOW		06
#define LIGHT_GREY		07
#define DARK_GREY		 8
#define BLUE			 9
#define LIGHT_GREEN		10
#define CYAN			11
#define RED				12
#define PINK			13
#define YELLOW			14
#define WHITE			15

void SetColorAndBackground(int ForgC, int BackC);
void drawLine(char* temp_board);
int randomSize(int smallest, int largest);
void playerMovement(int startIndex);
void setMap();
void drawGameBoard(int startIndex);
void printRow(int colIndex);
void UpdateMap(int colNum);
void playGame();
bool changeSize();
void creditScreen();

char game_board[ROWS][COLS];
int x = 0; // player x position
int y = 1; // player y position
int left = 20, right = 20, newL = 20, newR = 20; // land and river columns
int pointCounter = 0; // tracks points
int difficulty = 50; // difficulty is the frequency of the obstacles.  Difficulty of 50 = 1/50 = 2% Chance (goes down by 1 every 25 lines)
bool collision; // as long as collision is false, the game will continue to run, upon hitting an obstacle or wall collision becomes true



// START OF MAIN
void main()
{

	// Start of menu
	char choice = 'a';
	printf_s("\n\n\n\n\n\n\n\n\n\t\t\t   Welcome to River Rapids!\n");
	printf_s("\tAvoid all the obstacles. The further you get the higher the score.\n");
	printf_s("\t\t\tAre you ready to play? (Y/N): ");
	scanf_s(" %c", &choice, 1);
	
	while (toupper(choice) != 'Y' && toupper(choice) != 'N'){
		printf_s("\n\t\t\tInvalid input. (Y/N): ");
		scanf_s(" %c", &choice, 1);
	}

	if (toupper(choice) == 'Y') playGame();
	else{
		printf_s("\n\n\t\t\t\tPlay again soon!\n\t\t\t");
		system("pause");
		
	}
	
}


void playGame(){
	pointCounter = 0;
	x = 0;
	y = 1;
	difficulty = 50;
	system("cls");
	char choice = 'a';
	// Initialize the board to all 1's
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			game_board[i][j] = LAND;
		}
	}

	x = 40; // sets players start position to halfway across the first row
	collision = false; // player has not collided with a wall or obstacle. continue to run game as long as this value is false
	int startIndex = 0;
	setMap();

	while (!collision)
	{
		int tempCounter = pointCounter;

		if (tempCounter % 25 == 0 && difficulty >= 10)
		{
			difficulty--;
		}

		drawGameBoard(startIndex);

		UpdateMap(startIndex);

		playerMovement(startIndex);

		//Start of Delay Code
		clock_t begin = clock(); // clock that tracks time since start of frame
		for (int i = 0; 1; i++)
		{
			playerMovement(startIndex);

			double time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC; //Float value time_spent checks time since begin in seconds.
			if (time_spent >= .05)// check if time_spent is greater than time since begin (hard value sets delay time, lower # = faster refresh rate)
				break;
		}
		//End of Delay Code

		// Start of Fast Screen Clear
		COORD Position;
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		Position.X = 0;
		Position.Y = 0;
		SetConsoleCursorPosition(hout, Position);
		// End of Fast Screen Clear

		startIndex++;
		if (startIndex == ROWS)
		{
			startIndex = 0;
		}
		pointCounter++; // update points every frame
	}

	SetColorAndBackground(WHITE, BLACK);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			printf("%c", '0');
		}
	}
	system("cls");
	// Prompt user with score and ask to play again
	printf("\n\n\n\n\n\n\n\n\n\t\t\t\t    Gameover!"); 
	printf("\n\t\t\t\tYou got %d points!", pointCounter);
	printf_s("\n\n\t\t\t\tPlay again? (Y/N): ");
	scanf_s(" %c", &choice, 1);

	while (toupper(choice) != 'Y' && toupper(choice) != 'N'){
		printf_s("\n\t\t\tInvalid input. (Y/N): ");
		scanf_s(" %c", &choice, 1);
	}

	if (toupper(choice) == 'Y') playGame();
	else{
		creditScreen();
	}
	return;
}





void creditScreen(){
	system("cls");
	printf_s("\n\n\n\n\n\n\t\t\t\tDeveloped by: \n\n");
	printf_s("\t\t\t\tGaret Robertson\n\t\t\t\tJosh Campbell\n\n\n");
	printf_s("\n\n\t\t\t      Thanks for playing!\n\n\t\t\t");
	system("pause");
}


void SetColorAndBackground(int ForgC, int BackC) // easy to use colour function. First parameter sets foreground, second sets background 
//see #define's for key 
{
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	return;
}

int randomSize(int smallest, int largest)
{
	int result = (rand() % (smallest - largest)) + smallest;
	return result;
}

void UpdateMap(int colNum)
{
	int index = 0;
	if (left == newL) {
		if (changeSize())
			newL = randomSize(13, 30); // sets random range between 13 and 30
	}
	if (right == newR) {
		if (changeSize())
			newR = randomSize(13, 30); // sets random range between 13 and 30
	}
	if (left < newL)left++;
	if (left > newL)left--;
	if (right < newR)right++;
	if (right > newR)right--;
	int mid = 80 - (left + right);

	for (int i = 0; i < left; i++)
	{
		game_board[colNum][index] = LAND;

		index++;
	}
	for (int i = 0; i < mid; i++)
	{
		int obstacleLocation = randomSize(1, difficulty);
		if (obstacleLocation == 1)
		{
			game_board[colNum][index] = OBSTACLE;
		}
		else
		{
			game_board[colNum][index] = RIVER;
		}
		// Moment mod 
		// Create a engine 
		//)
		index++;
	}

	for (int i = 0; i < right; i++)
	{
		game_board[colNum][index] = LAND;
		index++;
	}
}

bool changeSize() {
	int result = rand() % 10;
	if (result > 7) {
		return true;
	}
	else {
		return false;
	}
}

void playerMovement(int startIndex)
{
	if (_kbhit())
	{
		int c = 0;
		switch ((c = _getch())) //getch_:  gets character from keyboard 
		{
		case KEY_UP: // capture up arrow press
			if (y > 0) //top boundary set to 0
			{
				y--; // move character up by subtracting from Y value.
			}
			break;
		case KEY_DOWN: // capture down arrow press
			if (y < (ROWS - 1)) // bottom boundary set to 23 (ROWS - 1)
			{
				++y; // move character down by adding to the Y value
			}
			break;
		case KEY_LEFT: // capture left arrow press
			if (x >= 0) // left boundary set to Column 0
			{
				--x; // move character left by subtracting from the X value
			}
			break;
		case KEY_RIGHT: // capture right arrow press
			if (x < (COLS - 1)) // right boundary set to Column 89 (COLS -1)
			{
				++x; // move character right by adding to the X value
			}
			break;
		default: // if anything other than up, down, left, or right, do nothing and exit switch
			break;
		}
	}
	int tempY = (y + startIndex) % (ROWS);

	if (game_board[tempY][x] == LAND
		|| game_board[tempY][x] == OBSTACLE)
	{
		collision = true;
	}
}

void setMap()
{
	for (int i = 0; i < ROWS; i++)
	{
		UpdateMap(i);
	}
}

void drawGameBoard(int startIndex)
{
	for (int rowsDrawn = 0; rowsDrawn < ROWS - 1; rowsDrawn++) // print rows until console screen is filled. (24 Rows)
	{
		printRow(rowsDrawn, startIndex);// print a row from the array (First Iteration: start position is Row 0: Prints Rows 0-23)
		startIndex++;					// increase array start position (First Iteration: start position is now Row 1: Prints Rows 1-23 and then Row 0)
		if (startIndex == ROWS)			// if you reach the end of the array (Row 23)...
			startIndex = 0;				//...reset array to zero and print a new Row 0 different from first Row 0 (value acquired through RandomSize() )
	}
}

void printRow(int rowsDrawn, int gameBoardRowIndex)
{
	int extraChars = 0;										// track extra characters in score board, accomodates for extra characters in console output
	int tempCounter = pointCounter;							// track points
	if (rowsDrawn == 5)  // print score on line 5
	{
		printf("%c%c%c%c%c", LAND, LAND, LAND, LAND, LAND); // print 5 LAND characters to indent the SCORE board.
		extraChars = 11;									// accomodate for extra characters in the console output.
		SetColorAndBackground(WHITE, BLACK);
		printf("SCORE:");
		printf("%d", pointCounter);							// print current score (updates each frame)
		extraChars++;														// Determine number of digits
		while (tempCounter >= 10)
		{
			tempCounter /= 10;								// divide by one digit
			extraChars++;									// one more digit to accomodate for in the console output.
		}
	}

	for (int i = extraChars; i < COLS; i++)
	{
		if (game_board[gameBoardRowIndex][i] == LAND)
			SetColorAndBackground(DARK_GREEN, LIGHT_GREEN);
		else if (game_board[gameBoardRowIndex][i] == RIVER)
			SetColorAndBackground(BLUE, DARK_BLUE);
		else if (game_board[gameBoardRowIndex][i] == OBSTACLE)
		{
			SetColorAndBackground(YELLOW, RED);
		}
		// This is the Row the character is currently on
		if (y == rowsDrawn && x == i)						// if the player's position matches the current row...
		{
			SetColorAndBackground(BLACK, YELLOW);			//...change foreground to black, background to yellow and...
			printf("%c", 1);								// ...print a yellow character at player's current index position.
		}
		else
		{
			printf("%c", game_board[gameBoardRowIndex][i]);
		}
	}

}