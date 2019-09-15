// sudoku.cpp : Defines the entry point for the console application.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
This program works with the AC-3 to eliminate any other options until one is left
When only one is left it returns the value
*/

int checkConstraints(int sudBoard[9][9], int i, int j)
{
	int index = sudBoard[i][j];
	int arrLeft[9];
	int k;
	int l;
	int indexToRemove;
	int count = 0;
	int max = 0;


	if (index != 0)                                 // If value where user needs an input is not empty, no need to solve
		return index;

	for (k = 0; k < 9; k++)
	{
		arrLeft[k] = (k + 1);
	}

	for (k = 0; k < 9; k++)                         // Checks horizontal and vertically to check any restrictions on that spot
	{
		if (sudBoard[i][k] != 0)
		{
			indexToRemove = sudBoard[i][k];
			arrLeft[indexToRemove - 1] = 0;
		}
		if (sudBoard[k][j] != 0)
		{
			indexToRemove = sudBoard[k][j];
			arrLeft[indexToRemove - 1] = 0;
		}
	}

	for (k = 0; k < 3; k++)                         // Checks in the box it is in to check for any restrictions
	{
		for (l = 0; l < 3; l++)
		{
			if (sudBoard[k + ((i / 3) * 3)][l + ((j / 3) * 3)] != 0)
			{
				indexToRemove = sudBoard[k + ((i / 3) * 3)][l + ((j / 3) * 3)];
				arrLeft[indexToRemove - 1] = 0;
			}
		}
	}
	for (k = 0; k < 9; k++)                         // If only one value can go in that spot then it uses AC-3 method to implement the value
	{
		if (arrLeft[k] != 0)
		{
			count++;
			if (arrLeft[k] > max)
				max = arrLeft[k];
		}
	}

	if (count > 1 || count == 0)
		return 0;

	return max;         // Returns the value

}

/*
Solves the sudoku puzzle without using AC-3 ALONE
It first uses AC-3 and then uses another algo I designed to solve or partially solve it.
Once partially solved, it uses backtracking to finish off the sudoku puzzle.
*/
int solveOther(int sudBoard[9][9])
{
    int i;
    int j;
    int k;
    int l;
    int x;
    int y;
    int z;
    int index;
    int indexToAdd;
    int change;
    int proxy;
    int arrLeft[9][1][3];
    int counter = 0;

    while (1 == 1)
    {
        while (1 == 1)
        {
            change = 0;
            for (i = 0; i < 9; i++)
            {
                for (j = 0; j < 9; j++)
                {
                    proxy = sudBoard[i][j];
                    index = checkConstraints(sudBoard, i, j);           // Solves as much as it can using the AC-3 method
                    sudBoard[i][j] = index;
                    if (proxy != index)
                        change = 1;
                }
            }
            if (change == 0)
                break;
        }

        for (k = 1; k < 10; k++)                // Uses other algo to mimic what a human would do when solving an sudoku puzzle
        {
            for (z = 0; z < 9; z++)
            {
                arrLeft[z][0][0] = 0;
                arrLeft[z][0][1] = 0;
                arrLeft[z][0][2] = 0;
            }

            i = k / 3 * 3;
            j = ((k + 2 ) % 3 ) * 3;

            for (x = i; x < i + 3; x++)
            {
                for (y = j; y < j + 3; y++)
                {
                    if (sudBoard[x][y] == 0)
                    {
                        for (l = 0; l < 9; l++)
                        {
                            if (checkPossible(sudBoard, (l + 1), x , y) == 1)
                            {
                                arrLeft[l][0][0] = arrLeft[l][0][0] + 1;
                                arrLeft[l][0][1] = x;
                                arrLeft[l][0][2] = y;
                            }
                        }
                    }
                }
            }
            for (l = 0; l < 9; l++)
            {
                //printf("%d times at %d,%d\n", arrLeft[l][0][0], arrLeft[l][0][1], arrLeft[l][0][2]);
                if (arrLeft[l][0][0] == 1)
                {
                    x = arrLeft[l][0][1];
                    y = arrLeft[l][0][2];
                    sudBoard[x][y] = l + 1;
                }
            }
        }

        change = 0;

        for (i = 0; i < 9; i++)
        {
            for (j = 0; j < 9; j++)
            {
                if (sudBoard[i][j] == 0)
                    change = 0;

                if (change != 0)
                    return 1;
            }
        }
        counter++;              // Does this in a loop 20 times before breaking out in which case the problem will be partially solved.
        if (counter == 20)
        {
            break;
        }
    }
    backTrackThree(sudBoard);           //Once partially solved, backtracking will finish the problem and solve it completely.
}

/*
This function tries to solve the sudoku using the AC-3 Method.
Returns 0 if not solved.
*/
int solveAcThree(int sudBoard[9][9])
{
	int i;
	int j;
	int index;
	int change;
	int proxy;

	while (1 == 1)
	{
		change = 0;
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				proxy = sudBoard[i][j];
				index = checkConstraints(sudBoard, i, j);        // Checks each spot and if one and only one value is possible, it implements it
				sudBoard[i][j] = index;
				if (proxy != index)
					change = 1;
			}
		}
		if (change == 0)
			break;
	}

	for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (sudBoard[i][j] == 0)
                return 0;
        }
    }

    return 1;
}

/*
Once AC-3 doesn't work
Function tries AC-3 and another algo to help partially solve the puzzle
Then it finishes it off with Backtracking
*/

int backTrackThree(int sudBoard[9][9])
{
    int i = 0;
    int j = 0;
    int k;
    int l = 0;
    int m;
    int arrPast[80][2];
    int index = 0;

    while (1==1)
    {
        if (sudBoard[i][j] == 0)
        {
            for (k = 1; k < 10; k++)
            {
                l = checkPossible(sudBoard, k, i, j);               // Tries to put any VALID input in a empty space
                if (l == 1)
                {
                    arrPast[index][0] = i;
                    arrPast[index][1] = j;
                    sudBoard[i][j] = k;
                    index++;
                    break;
                }
            }
            while (l == 0)                                           // If no valid input available then goes back to previous input and changes and tries again (Backtracks)
            {
                index--;
                i = arrPast[index][0];
                j = arrPast[index][1];

                m = sudBoard[i][j];

                sudBoard[i][j] = 0;

                for (k = m + 1; k < 10; k++)
                {
                    l = checkPossible(sudBoard, k, i ,j);
                    if (l == 1)
                    {
                        sudBoard[i][j] = k;
                        arrPast[index][0] = i;
                        arrPast[index][1] = j;
                        index++;
                        break;
                    }
                }
            }
        }
        j++;

        if (i == 8 && j == 9)           // Knows when the sudoku is fully solved (exit)
            break;

        if ((j % 3 == 0) && ((i+1) % 3 == 0) && (j < 7) && j != 0)
        {
            i = i - 2;
        }
        else if ((j % 3 == 0) &&((i + 1) % 3 != 0) && j != 0)
        {
            i++;
            j = j - 3;
        }
        else if ((j % 3 == 0) && ((i+1) % 3 == 0) && (j > 7) && j != 0)
        {
            i++;
            j = 0;
        }
    }
    printSud(sudBoard);
    return 1;
}

/*
Checks if a value (k) is valid on the board at the coodinates i and j
If k is valid on those coordinates, returns 1
or it returns 0 for not being valid
*/

int checkPossible(int sudBoard[9][9], int k, int i, int j)
{
    int l;
    int m;


    for (l = 0; l < 9; l++)
    {
        if (sudBoard[i][l] == k)
        return 0;

        if (sudBoard[l][j] == k)
        return 0;
    }

    for (l = 0; l < 3; l++)
    {
        for (m = 0; m < 3; m++)
        {
            if (sudBoard[l + ((i / 3) * 3)][m + ((j / 3) * 3)] == k)
			{
				return 0;
			}
        }
    }

//    printf("Change %d at %d,%d", k,i,j);
    return 1;
}

/*
Loads a sudoku txt file from file path into sudBoard array.
*/
void sudFromFile(char* file, int sudBoard[9][9]){
	FILE* fp;
	if ((fp = fopen(file, "r")) == NULL)
    {
		printf("Could not open file");
		exit(1);
	}

	int i;
	int lineNum = 0;
	char line[11];
	while (fgets(line, sizeof(line), fp) != NULL && lineNum < 10)
    {

		for (i = 0; i < 9; i++)
        {
			sudBoard[lineNum][i] = (line[i] - '0');
		}
		lineNum++;

	}

	fclose(fp);

}
/*
Saves a Sudoku puzzle as a txt file. Format:

	460509700
	902317400
	087000509
	700460090
	000008000
	014952307
	540270003
	070095600
	020143975

'0's act as empty spaces.
Sudoku files are saved and read from a folder named 'puzzles' at the program source location.
Files are labled as sudokuX.txt were 'X' is an incrementing value read from sudokuInc.txt
*/
void sudToFile(int sudBoard[9][9])
{

	//Read save file for the current number of saved Sudoku puzzles
	FILE* fpIncRead;

	if ((fpIncRead = fopen("sudokuInc.txt", "r")) == NULL)
    {
		printf("Could not read file");
		exit(1);
	}

	char line[8];
	int increment = 0;
	fgets(line, sizeof(line), fpIncRead);
	increment = atoi(line);
	fclose(fpIncRead);

	//Write to save file by incrementing the stored value
	FILE* fpIncWrite;
	if ((fpIncWrite = fopen("sudokuInc.txt", "w")) == NULL)
    {
		printf("Could not write file");
		exit(1);
	}
	fprintf(fpIncWrite, "%d", ++increment);
	fclose(fpIncWrite);


	//Open a new file to save the sudoku puzzle to
	FILE* fp;
	char fileName[32];
	snprintf(fileName, sizeof(char) * 32, "puzzles/sudoku%i.txt", increment);
	if ((fp = fopen(fileName, "w")) == NULL)
    {
		printf("Could not write file");
		exit(1);
	}

	int j;
	int i;
	//
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			fprintf(fp, "%d",sudBoard[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

int** generateSudBoard(){
	int sBoard[9][9];
	int j;
	int i;
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
		{
			sBoard[i][j] = 0;
		}
	}
	return 0;
}

/*
Prints the contents of the sudoku board to console.
*/
void printSud(int sudBoard[9][9]){
	int i;
	int j;
	for (i = 0; i < 9; i++)
	{
		printf("\n");

		if (i % 3 == 0)
        {
			printf("  -----------------\n");
		}
		for (j = 0; j < 9; j++)
		{
			if (j % 3 == 0)
            {
				printf(" | ");
			}
			printf("%d", sudBoard[i][j]);
		}
		printf(" |");
	}
	printf("\n  -----------------\n");
}

int main()
{
	int arrSudokuBoard[9][9];
	int sBoard[9][9] = {NULL};
	int solveable;
	int j;
	int i;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			arrSudokuBoard[i][j] = 0;
		}
	}

	char* path = "puzzles/sudoku1.txt"; //default path
	int option;
	printf("********* Sudoku Solver **********\n");
	printf("1. Load Sudoku \n");
	printf("2. Save Sudoku \n");
	printf("3. Solve Sudoku \n");
	printf("4. Print Sudoku \n");
	printf("5. Exit \n\n");

	while (1==1)
    {
        printf("Enter Option: ");
        option = getchar();

        switch (option)
        {
            case '1':
                sudFromFile(path, sBoard);
                printf("Loaded Sudoku.\n");
                break;
            case '2':
                sudToFile(sBoard);
                printf("Saving Sudoku.\n");
                break;
            case '3':

                printf("Solving Sudoku...\n");

                solveable = solveAcThree(sBoard);

                if (solveable == 0)
                {
                    printf("The sudoku problem is not solveable by AC-3 method.\n");
                    solveable = solveOther(sBoard);
                    printf("\nThe problem was solved using a number of methods which include backtracking, AC-3 and aditional functions!\n");
                }
                printf("\n Solvable: %d", solveable);
                break;
            case '4':
                if (sBoard[0][0] == NULL)
                {
                    printf("No sudoku loaded.");
                }
                else
                {
                    printSud(sBoard);
                }
            break;
            case '5':
                exit(1);
            break;
        }
        printf("\n");
        option = getchar();
    }
	return 0;
}
