#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0
#define HORIZONTAL 1
#define VERTICAL 2
#define DIAGONALDESC 3
#define DIAGONALASC 4
#define NORMAL_COLOUR 7

typedef struct 
{
	int letter;
	int colour;
	char* word;
	bool wordGuessed;
} point;

char **create2DArray(); //function prototype
void printArray(); //Prints 2d array on screen.
void placeHorizontal();
void placeVertical();
void placeDiagonal();
void fillArray();
void placeWords();
int RandRange();

int grid_size;

char *planets[] = { "MERCURY", "VENUS", "EARTH", "MARS", "JUPITER", "SATURN", "URANUS", "NEPTUNE" };
char *animals[] = { "TIGER", "ZEBRA", "EAGLE", "DUCK", "PENGUIN", "SWORDFISH", "SNAKE", "OCTOPUS" };
char *colours[] = { "RED", "GREEN", "BLUE", "YELLOW", "ORANGE", "SILVER", "INDIGO", "VIOLET", };
char *usedWords[9] = { " ", " ", " ", "DUCK", " ", " ", " ", " ", " " };
char *guessedWords[9] = { "","","","","","","","","" };
int categorySize = 9;

char** myArray; //global array for grid.
point points[50][50];

//Creates a 2D array of grid_size * grid_size and returns a pointer to it
char **create2DArray()
{
	int i, j;
	char **array = (char **)malloc(sizeof(char *) * grid_size);

	for (i = 0; i<grid_size; i++)
		array[i] = (char *)malloc(sizeof(char) * grid_size);

	for (i = 0; i<grid_size; i++)
		for (j = 0; j<grid_size; j++)
			array[i][j] = '.';
	return array;
}

void printArray(char** array)
{
	HANDLE  hConsole;
	int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int i, j;
	for (i = 0; i< grid_size; i++)
	{
		for (j = 0; j<grid_size; j++)
		{
			//int randColour = randRange(9, 15);
			bool wordGuessed = false;
			int count;
			for (count = 0; count < 9; count++)//Loop through all guessed words.
			{
				char* word = points[i][j].word;
				char* guessedWord = guessedWords[count];


				if (strcmp(points[i][j].word, guessedWords[count]) == 0)
				{
					wordGuessed = true;
					points[i][j].wordGuessed = true;
				}
				else k = NORMAL_COLOUR;
			}

			if (points[i][j].wordGuessed == true)
			{
				k = points[i][j].colour;
			}
			else k = 7;
			
			SetConsoleTextAttribute(hConsole, k);
			printf("%c ", array[i][j]);
			k = 7;
			SetConsoleTextAttribute(hConsole, k);
		}
		printf("\n");
	}
}

void placeWords(char** myArray, int choice, int maxWords)
{
	int i;
	for (i = 0; i < maxWords; i++)
	{
		if (choice == 1)
		{
			switch (rand() % 5)
			{
			case 0:
			case 1:
				placeHorizontal(myArray, planets[i]);
				break;
			case 2:
			case 3:
				placeVertical(myArray, planets[i]);
				break;
			case 4:
				placeDiagonal(myArray, planets[i]);
				break;
			}
			usedWords[i] = planets[i];
		}

		else if (choice == 2)
		{
			switch (rand() % 5)
			{
			case 0:
			case 1:
				placeHorizontal(myArray, animals[i]);
				break;
			case 2:
			case 3:
				placeVertical(myArray, animals[i]);
				break;
			case 4:
				placeDiagonal(myArray, animals[i]);
				break;
			}
			usedWords[i] = animals[i];
		}

		else if (choice == 3)
		{
			switch (rand() % 5)
			{
			case 0:
			case 1:
				placeHorizontal(myArray, colours[i]);
				break;
			case 2:
			case 3:
				placeVertical(myArray, colours[i]);
				break;
			case 4:
				placeDiagonal(myArray, colours[i]);
				break;
			}
			usedWords[i] = colours[i];
		}
	}
}

void fillArray(char** array)
{
	int i, j;
	for (i = 0; i<grid_size; i++)
	{
		for (j = 0; j<grid_size; j++)
		{
			if (array[i][j] == '.')
			{
				array[i][j] = 'A' + rand() % 26;
			}
		}
	}
}

int randRange(int min, int max)
{
	int diff = max - min;
	return (int)(((double)(diff + 1) / RAND_MAX) * rand() + min);
}

void placeHorizontal(char** array, char* word)
{
	bool overlap = false;
	do
	{
		overlap = false;
		//int randomIndex = rand() % (categorySize - 1);
		int i;
		int wordSize = strlen(word);
		int minDistanceFromEdge = grid_size - wordSize - 1;
		int startRow = rand() % grid_size;
		int startCol = rand() % minDistanceFromEdge;

		//Search ahead to check for any overlaps before inserting letters
		for (i = 0; i < wordSize; i++)//for every letter in the word.
		{
			if (array[startRow][startCol + i] != '.')// If there is overlap.
			{
				overlap = true;
			}
		}

		if (overlap == false)
		{
			int randColour = randRange(9, 15);
			for (i = 0; i < wordSize; i++)//for every letter in the word.
			{
				array[startRow][startCol + i] = word[i];// Place the letter in array.
				points[startRow][startCol + i].letter = word[i];
				points[startRow][startCol + i].word = word;
				points[startRow][startCol + i].colour = randColour;
			}
		}
	} while (overlap == true);
}

void placeVertical(char** array, char* word)
{
	bool overlap = false;
	do
	{
		overlap = false;
		int randomIndex = rand() % (categorySize - 1);
		int i;
		int wordSize = strlen(word);
		int minDistanceFromEdge = grid_size - wordSize - 1;
		int startRow = rand() % minDistanceFromEdge;
		int startCol = rand() % grid_size;

		//Search ahead to check for any overlaps before inserting letters
		for (i = 0; i < wordSize; i++)//for every letter in the word
		{
			if (array[startRow + i][startCol] != '.')
			{
				overlap = true;
			}
		}

		if (overlap == false)
		{
			int randColour = randRange(9, 15);
			for (i = 0; i < wordSize; i++)//for every letter in the word
			{
				array[startRow + i][startCol] = word[i];
				points[startRow + i][startCol].letter = word[i];
				points[startRow + i][startCol].word = word;
				points[startRow + i][startCol].colour = randColour;
			}
		}
	} while (overlap == true);
}

void placeDiagonal(char** array, char *word)
{
	bool overlap = false;
	do
	{
		overlap = false;
		int randomIndex = rand() % (categorySize - 1);
		int i;
		int wordSize = strlen(word);
		int minDistanceFromEdge;

		if (rand() % 2 == 1) //Place diagonal descending
		{
			minDistanceFromEdge = grid_size - wordSize - 1;
			int startRow = rand() % minDistanceFromEdge;
			int startCol = rand() % minDistanceFromEdge;

			//Search ahead to check for any overlaps before inserting letters
			for (i = 0; i < wordSize; i++)//for every letter in the word
			{
				if (array[startRow + i][startCol + i] != '.')
				{
					overlap = true;
				}
			}

			if (overlap == false)
			{
				int randColour = randRange(9, 15);
				for (i = 0; i < wordSize; i++)//for every letter in the word
				{
					array[startRow + i][startCol + i] = word[i];
					points[startRow + i][startCol + i].letter = word[i];
					points[startRow + i][startCol + i].word = word;
					points[startRow + i][startCol + i].colour = randColour;
				}
			}
		}
		else //Place diagonal ascending
		{
			minDistanceFromEdge = wordSize + 1;
			int startRow = randRange(minDistanceFromEdge, (grid_size - 1));
			if (startRow == grid_size)
				startRow = grid_size - 1;
			int startCol = randRange(0, grid_size - 1 - minDistanceFromEdge);

			//Search ahead to check for any overlaps before inserting letters
			for (i = 0; i < wordSize; i++)//for every letter in the word
			{
				if (array[startRow - i][startCol + i] != '.')//Crashes happening here.
				{
					overlap = true;
				}
			}

			if (overlap == false)
			{
				int randColour = randRange(9, 15);
				for (i = 0; i < wordSize; i++)//for every letter in the word
				{
					array[startRow - i][startCol + i] = word[i];
					points[startRow - i][startCol + i].letter = word[i];
					points[startRow - i][startCol + i].word = word;
					points[startRow - i][startCol + i].colour = randColour;
				}
			}
		}
	} while (overlap == true);
}

int checkGuess(char* guess)
{
	int i;
	for (i = 0; i < 9; i++)
	{
		char *word = usedWords[i];
		if (strcmp(guess, word) == 0)
		{
			return 1;
		}
	}
}

int main()
{
	int choice, i, j;
	printf("--------------------------------");
	printf("\nWelcome to the Word Search Game!\n");
	printf("--------------------------------\n");
	printf("Please select a category to play.\n1:> Planets\n2:> Animals\n3:> Colours\n");
	scanf("%d", &choice);
	printf("Enter the size of the grid to play.\n");
	scanf("%d", &grid_size);
	getchar();
	srand(time(NULL));
	myArray = create2DArray();

	// Allocate number of words to add to grid.
	int maxWords;
	if (grid_size <= 15)
	{
		maxWords = 6;
	}
	else if (grid_size < 20)
	{
		maxWords = 7;
	}
	else maxWords = 8;

	for (i = 0; i < grid_size; i++)
	{
		for (j = 0; j < grid_size; j++)
		{
			points[i][j].word = "blank";
			points[i][j].colour = NORMAL_COLOUR;
			points[i][j].wordGuessed = false;
		}
	}

	placeWords(myArray, choice, maxWords);
	fillArray(myArray);
	char guess[256];
	int correctGuesses = 0;
	clock_t start = clock();
	while (true)
	{
		clock_t end = clock();
		float seconds = (float)(end - start) / CLOCKS_PER_SEC;
		system("cls");
		printf("You have found %d out of %d words.", correctGuesses, maxWords);
		printf("\nYou have 2 minutes!\n");
		printArray(myArray);
		printf("You have %.2f seconds remaining\n", 120 - seconds);
		printf("Enter a word: ");
		fgets(guess, 256, stdin);

		guess[strlen(guess) - 1] = '\0';//Remove newline character from input.

		if (checkGuess(guess) == 1)
		{
			guessedWords[correctGuesses] = guess;
			correctGuesses++;			
		}
		end = clock();
		seconds = (float)(end - start) / CLOCKS_PER_SEC;
		if (correctGuesses == maxWords || 120.00 - seconds <= 0.00)
			break;
	}
	if (correctGuesses == maxWords)
	{
		system("cls");
		printArray(myArray);
		printf("CONGRATULATIONS!! YOU WIN!\n");
		clock_t end = clock();
		float seconds = (float)(end - start) / CLOCKS_PER_SEC;
		printf("You completed the wordsearch in %.2f seconds!\n", seconds);
	}
	else printf("YOU LOSE!!\n");

	//printf("\n");
	system("PAUSE");
	return 0;
}