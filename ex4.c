/******************
Name: Konstantin Ostashenko
ID: 345548929
Assignment: ex4
*******************/
#include <stdio.h>
#include <string.h>

#define TASK_CHOICE_ROBOT_PATHS 1
#define TASK_CHOICE_HUMAN_PYRAMID 2
#define TASK_CHOICE_PARENTHESIS_VALIDATION 3
#define TASK_CHOICE_QUEENS_BATTLE 4
#define TASK_CHOICE_CROSSWORD_GENERATOR 5
#define TASK_CHOICE_EXIT 6

void printMenu();

// --------------------

unsigned long long int recursiveCombinations(int n, int k);
void robotPaths();

// --------------------

#define NUMBER_OF_PYRAMID_ROWS 5
#define NUMBER_OF_PYRAMID_COLUMNS 5
#define TOTAL_WEIGHT_MULTIPLIER 0.5

float recursiveTotalWeight(int row, int column,
                           float cheerleaderWeights[NUMBER_OF_PYRAMID_ROWS][NUMBER_OF_PYRAMID_COLUMNS]);
void humanPyramid();

// --------------------

#define NUMBER_OF_PARENTHESES_PAIRS 4
#define ENDING_CHAR '\n'
char *parenthesesPairs[NUMBER_OF_PARENTHESES_PAIRS] = {"()", "[]", "{}", "<>"};

int isOpeningParenthesis(char c, int index);
int isClosingParenthesis(char c, int index);
int isPairedParentheses(char opening, char closing, int index);
int isEndingChar(char c);
char recursiveParenthesisSearch();
void parenthesisBalanceValidation();

// --------------------

#define MAX_QUEENS_SIZE 20
#define ASCII_SIZE 256
#define EMPTY_QUEEN_CHAR '*'
#define QUEEN_CHAR 'X'


int isEmpty(int row, int column, int gridSize, char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]);
int isFreeCell(int row, int column, unsigned char zone, int gridSize,
               int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
               char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]);
void placeQueen(int row, int column, unsigned char zone,
                int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
                char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]);
void removeQueen(int row, int column, unsigned char zone,
                int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
                char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]);
int recursiveQueensPlacement(int row, int column, int gridSize,
                             int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
                             unsigned char zones_grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE],
                             char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]);
void queensBattle();

// --------------------

#define MAX_CROSSWORD_SIZE 30
#define MAX_SLOTS_NUMBER 100
#define MAX_WORDS_NUMBER 100
// 15 + 1 for '\0'
#define MAX_WORD_LENGTH 16
#define HORIZONTAL_SLOT_DIRECTION 'H'

typedef struct CrosswordSlot {
    // starting point coordinates
    int row, column;
    unsigned long int length;
    // 1 if horizontal, 0 if vertical
    int horizontal;
} CrosswordSlot;

int checkSlotLetters(int letterIndex, char word[MAX_WORD_LENGTH], CrosswordSlot slot,
                     char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]);
int wordFitsSlot(char word[MAX_WORD_LENGTH], CrosswordSlot slot,
                 char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]);
void placeWord(int letterIndex, char word[MAX_WORD_LENGTH], CrosswordSlot  slot,
                char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]);
void removeWord(int letterIndex, char word[MAX_WORD_LENGTH], CrosswordSlot  slot,
                char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]);
int recursiveWordPlacement(int wordIndex, int wordsNumber, int slotIndex, int slotsNumber,
                                char words[MAX_WORDS_NUMBER][MAX_WORD_LENGTH], int usedWords[MAX_WORDS_NUMBER],
                                CrosswordSlot slots[MAX_SLOTS_NUMBER],
                                char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]);
void crosswordGenerator();

// --------------------

int main() {
    int chosenTask = 0;
    while (chosenTask != TASK_CHOICE_EXIT) {
        printMenu();
        scanf("%d", &chosenTask);
        switch (chosenTask) {
            case TASK_CHOICE_ROBOT_PATHS:
                robotPaths();
                break;
            case TASK_CHOICE_HUMAN_PYRAMID:
                humanPyramid();
                break;
            case TASK_CHOICE_PARENTHESIS_VALIDATION:
                parenthesisBalanceValidation();
                break;
            case TASK_CHOICE_QUEENS_BATTLE:
                queensBattle();
                break;
            case TASK_CHOICE_CROSSWORD_GENERATOR:
                crosswordGenerator();
                break;
            case TASK_CHOICE_EXIT:
                printf("Goodbye!\n");
                break;
            default:
                printf("Please choose a task number from the list.\n");
                break;
        }
    }
    return 0;
}

/*
 * Print main menu
 * */
void printMenu() {
    printf("Choose an option:\n"
           "1. Robot Paths\n"
           "2. The Human Pyramid\n"
           "3. Parenthesis Validation\n"
           "4. Queens Battle\n"
           "5. Crossword Generator\n"
           "6. Exit\n");
}

/*
 * Recursive function to calculate C(n, k)
 * C(n, k) = n if k = 1
 * C(n, k) = C(n-1,k-1) * n / k otherwise
 * Using unsigned long long int just as a pre-caution against big enough n and k
 * Works fine for X+Y <= 62
 * */
unsigned long long int recursiveCombinations(int n, int k) {
    if (k == 0) {
        return 1;
    }
    return recursiveCombinations(n - 1, k - 1) * n / k;
}

/*
 * Task 1, finding the number of unique robot paths
 * */
void robotPaths() {
    /*
     * The simplest recursion in this task would be:
     * -------------------------------------------
     * int paths(int x, int y) {
     *   if (x == 0 || y == 0) {
     *     return 1;
     *   }
     *   return paths(x - 1, y) + paths(x, y - 1);
     * }
     * -------------------------------------------
     * Which is literally calling the function from each possible cell robot can get to
     * until we hit the "wall"
     * Hitting the "wall" gives us exactly one path - all the way left or all the way down
     * This algorithm is valid, but on large X and Y it is going to be hugely inefficient - O(2^(X+Y))
     * And unfortunately storing previous calls is not an option as we have no known limits for X and Y
     * */
    /*
     * Note that in all cases, in order to get to the end, the robot needs to make exactly
     * X steps left and Y steps down, no matter in which order he actually does them
     * This means that the total number of steps is constant and is X+Y
     * Let the step left be RED and the step-down be GREEN
     * Imagine X+Y balls as our steps, we must color them as exactly X RED ones and Y GREEN ones
     * Note that coloring X RED ones automatically makes all other balls GREEN
     * This means that the number of unique paths is equal to the number of ways how we can color
     * X balls RED from the total of X+Y balls
     * This number is equal to the number of combinations, which is: C(X+Y, X)
     * C(n, k) = n!/(k! * (n-k)!) = n/k * (n-1)!/((k-1)! * (n-k)!) = n/k * C(n-1, k-1)
     * The recursive formula for C(n, k) would then be:
     * C(n, k) = n if k = 1
     * C(n, k) = C(n-1,k-1) * n/k otherwise
     * This is a recursive formula which we will implement
     * It has the difficulty of O(k) as it has exactly k recursive calls
     * */
    printf("Please enter the coordinates of the robot (column, row):\n");
    int x = 0;
    int y = 0;
    scanf("%d %d", &x, &y);
    unsigned long long int totalPaths = 0;
    // C(X+Y, X) = C(X+Y, Y) and smaller second argument means less recursive calls
    if (x < 0 || y < 0) {
        totalPaths = 0;
    } else if (x < y) {
        totalPaths = recursiveCombinations(x + y, x);
    } else {
        totalPaths = recursiveCombinations(x + y, y);
    }
    printf("The total number of paths the robot can take to reach home is: %llu\n", totalPaths);
}

/*
 * Calculating total weight recursively
 * */
float recursiveTotalWeight(int row, int column,
                           float cheerleaderWeights[NUMBER_OF_PYRAMID_ROWS][NUMBER_OF_PYRAMID_COLUMNS]) {
    if (column < 0 || column > row) {
        return 0;
    } else if (row == 0) {
        return cheerleaderWeights[row][column];
    }
    return cheerleaderWeights[row][column] + TOTAL_WEIGHT_MULTIPLIER
                                             * (recursiveTotalWeight(row - 1, column - 1, cheerleaderWeights)
                                                + recursiveTotalWeight(row - 1, column, cheerleaderWeights));
}

/*
 * Task 2, calculating total weight for each person in the human pyramid
 * */
void humanPyramid() {
    /*
     * Weights of cheerleaders are stored the following way in a 2D-array:
     * [* 0 0 0 0]       [*]
     * [* * 0 0 0]      [* *]
     * [* * * 0 0] ->  [* * *]
     * [* * * * 0]    [* * * *]
     * [* * * * *]   [* * * * *]
     * */
    float cheerleaderWeights[NUMBER_OF_PYRAMID_ROWS][NUMBER_OF_PYRAMID_COLUMNS];
    printf("Please enter the weights of the cheerleaders:\n");
    for (int i = 0; i < NUMBER_OF_PYRAMID_ROWS; ++i) {
        for (int j = 0; j <= i; ++j) {
            scanf("%f", &cheerleaderWeights[i][j]);
            if (cheerleaderWeights[i][j] < 0) {
                printf("Negative weights are not supported.\n");
                return;
            }
        }
    }
    printf("The total weight on each cheerleader is:\n");
    for (int i = 0; i < NUMBER_OF_PYRAMID_ROWS; ++i) {
        for (int j = 0; j <= i; ++j) {
            printf("%.2f ", recursiveTotalWeight(i, j, cheerleaderWeights));
        }
        printf("\n");
    }
}

/*
 * Recursively check if the character is an opening parenthesis
 * */
int isOpeningParenthesis(char c, int index) {
    if (!(index >= 0 && index < NUMBER_OF_PARENTHESES_PAIRS)) {
        return 0;
    }
    if (c == parenthesesPairs[index][0]) {
        return 1;
    }
    return isOpeningParenthesis(c, index + 1);
}

/*
 * Recursively check if the character is a closing parenthesis
 * */
int isClosingParenthesis(char c, int index) {
    if (!(index >= 0 && index < NUMBER_OF_PARENTHESES_PAIRS)) {
        return 0;
    }
    if (c == parenthesesPairs[index][1]) {
        return 1;
    }
    return isClosingParenthesis(c, index + 1);
}

/*
 * Recursively check if two characters are a valid parentheses pair
 * */
int isPairedParentheses(char opening, char closing, int index) {
    if (!(index >= 0 && index < NUMBER_OF_PARENTHESES_PAIRS)) {
        return 0;
    }
    if (opening == parenthesesPairs[index][0] && closing == parenthesesPairs[index][1]) {
            return 1;
    }
    return isPairedParentheses(opening, closing, index + 1);
}

/*
 * Check if the character is an ending one (e.g. end of input)
 * */
int isEndingChar(char c) {
    return c == ENDING_CHAR;
}

/*
 * Recursively validate the input string for parentheses balance up until the ENDING_CHAR
 * This algorithm uses the recursion call stack as a place to "store" opening parentheses
 * */
char recursiveParenthesisSearch() {
    char current = '\0';
    /*
     * Reading chars one by one is also an option, it doesn't change the logic
     * Using a loop to skip "non-interesting" characters in the input
     * thus significantly reducing recursion depth
     * */
    while (!(isOpeningParenthesis(current, 0) || isClosingParenthesis(current, 0) || isEndingChar(current))) {
        scanf("%c", &current);
    }
    /*
     * At this point current can only be an opening/closing parenthesis or the ENDING_CHAR
     * Base cases: If current is ENDING_CHAR or a closing parenthesis - return it
     * Otherwise current is an opening parenthesis - start searching for a closingCandidate.
     * closingCandidate can either be ENDING_CHAR or a closing parenthesis
     * Check if current and closingCandidate are a valid parentheses pair
     * If not - return 0
     * If they are - proceed with the search as the input is not yet finished
     * */
    if (isEndingChar(current) || isClosingParenthesis(current, 0)) {
        return current;
    }
    char closingCandidate = recursiveParenthesisSearch();
    if (!isPairedParentheses(current, closingCandidate, 0)) {
        // unbalanced, read the leftovers if any
        if (closingCandidate != ENDING_CHAR) {
            scanf("%*[^\n]");
        }
        return '\0';
    }
    return recursiveParenthesisSearch();
}

/*
 * Task 3, validating a term for parentheses balance
 * */
void parenthesisBalanceValidation() {
    printf("Please enter a term for validation:\n");
    scanf(" ");
    if (recursiveParenthesisSearch() == ENDING_CHAR) {
        printf("The parentheses are balanced correctly.\n");
    } else {
        printf("The parentheses are not balanced correctly.\n");
    }
}

/*
 * Check if the grid cell is empty
 * */
int isEmpty(int row, int column, int gridSize, char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]) {
    // If row or column is invalid - cell is considered empty
    return !(row >= 0 && row < gridSize) || !(column >= 0 && column < gridSize)
           || grid[row][column] == EMPTY_QUEEN_CHAR;
}

/*
 * Check if the queen can be placed at (row, column)
 * */
int isFreeCell(int row, int column, unsigned char zone, int gridSize,
               int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
               char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]) {
    /*
     * Queen can be placed if and only if:
     * Row is free
     * Column is free (column is always free because of the algorithm)
     * Zone is free
     * Upper-left corner (x-1, y-1) is free
     * Bottom-left corner (x+1, y-1) is free
     * Checking the right side corners is not necessary, as the queens are placed strictly from left to right
     * */
    return rows[row] == 0 && zones[zone] == 0
           && isEmpty(row - 1, column - 1, gridSize, grid)
           && isEmpty(row + 1, column - 1, gridSize, grid);
}

/*
 * Place the queen on the grid and mark it in all checking points
 * */
void placeQueen(int row, int column, unsigned char zone,
                int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
                char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]) {
    grid[row][column] = QUEEN_CHAR;
    rows[row] = 1;
    zones[zone] = 1;
}

/*
 * Remove the queen from the grid and unmark it in all checking points */
void removeQueen(int row, int column, unsigned char zone,
                int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
                char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]) {
    grid[row][column] = EMPTY_QUEEN_CHAR;
    rows[row] = 0;
    zones[zone] = 0;
}

/*
 * Recursively place N queens on a NxN colored grid
 * */
int recursiveQueensPlacement(int row, int column, int gridSize,
                             int rows[MAX_QUEENS_SIZE], int zones[ASCII_SIZE],
                             unsigned char zones_grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE],
                             char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE]) {
    /*
     * Base cases are:
     * Row exceeds the grid size - no solution
     * All N queens are placed - solution is found
     * Queens are placed strictly from left to right, so the number of placed queens is equal to column
     * */
    if (row >= gridSize) {
        return 0;
    }
    if (column >= gridSize) {
        return 1;
    }
    unsigned char zone = zones_grid[row][column];
    // If the queen can be placed at (row, column)
    if (isFreeCell(row, column, zone, gridSize, rows, zones, grid)) {
        // Place the queen at (row, colum) and continue recursively to the next column
        placeQueen(row, column, zone, rows, zones, grid);
        if (recursiveQueensPlacement(0, column + 1, gridSize, rows, zones, zones_grid, grid)) {
            return 1;
        }
        // Backtracking, if there is no solution after placing queen at (row, column) - remove it
        removeQueen(row, column, zone, rows, zones, grid);
    }
    // The queen cannot be placed at (row, column), continue recursively to the next row
    return recursiveQueensPlacement(row + 1, column, gridSize, rows, zones, zones_grid, grid);
}

/*
 * Task 4, placing N queens on a NxN colored grid
 * */
void queensBattle() {
    int gridSize = 0;
    // Any ASCII character can be represented as an unsigned char
    unsigned char grid[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE];
    char result[MAX_QUEENS_SIZE][MAX_QUEENS_SIZE];
    printf("Please enter the board dimensions:\n");
    scanf("%d", &gridSize);
    printf("Please enter a %d*%d puzzle board:\n", gridSize, gridSize);
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            scanf(" %c", &grid[i][j]);
            result[i][j] = EMPTY_QUEEN_CHAR;
        }
    }
    // Each row is an index from 0 to MAX_QUEENS_SIZE, each zone is an ASCII character
    int rows[MAX_QUEENS_SIZE] = {0};
    int zones[ASCII_SIZE] = {0};
    if (recursiveQueensPlacement(0, 0, gridSize, rows, zones, grid, result)) {
        printf("Solution:\n");
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                printf("%c ", result[i][j]);
            }
            printf("\n");
        }
    } else {
        printf("This puzzle cannot be solved.\n");
    }
}

// --------------------

/*
 * Recursively check if word fits the letters that are already on the grid in the given slot
 * */
int checkSlotLetters(int letterIndex, char word[MAX_WORD_LENGTH], CrosswordSlot slot,
                     char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]) {
    if (word[letterIndex] == '\0') {
        return 1;
    }
    int row = slot.row;
    int column = slot.column;
    if (slot.horizontal) {
        column += letterIndex;
    } else {
        row += letterIndex;
    }
    return (grid[row][column] == '\0' || word[letterIndex] == grid[row][column])
           && checkSlotLetters(letterIndex + 1, word, slot, grid);
}

/*
 * Check if word fits the given slot
 * */
int wordFitsSlot(char word[MAX_WORD_LENGTH], CrosswordSlot slot,
                 char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]) {
    // If the word length is 
    return strlen(word) == slot.length && checkSlotLetters(0, word, slot, grid);
}

/*
 * Place word into the grid in the given slot
 * */
void placeWord(int letterIndex, char word[MAX_WORD_LENGTH], CrosswordSlot  slot,
               char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]) {
    if (word[letterIndex] == '\0') {
        return;
    }
    int row = slot.row;
    int column = slot.column;
    if (slot.horizontal) {
        column += letterIndex;
    } else {
        row += letterIndex;
    }
    grid[row][column] = word[letterIndex];
    placeWord(letterIndex + 1, word, slot, grid);
}

/*
 * Remove word from the grid in the given slot
 * */
void removeWord(int letterIndex, char word[MAX_WORD_LENGTH], CrosswordSlot  slot,
               char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]) {
    if (word[letterIndex] == '\0') {
        return;
    }
    int row = slot.row;
    int column = slot.column;
    if (slot.horizontal) {
        column += letterIndex;
    } else {
        row += letterIndex;
    }
    grid[row][column] = '\0';
    removeWord(letterIndex + 1, word, slot, grid);
}

/*
 * Recursively place words in a crossword skeleton
 * */
int recursiveWordPlacement(int wordIndex, int wordsNumber, int slotIndex, int slotsNumber,
                                char words[MAX_WORDS_NUMBER][MAX_WORD_LENGTH], int usedWords[MAX_WORDS_NUMBER],
                                CrosswordSlot slots[MAX_SLOTS_NUMBER],
                                char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE]) {
    /*
     * Base cases are:
     * WordIndex exceeds the dictionary size - no solution
     * All N slots are filled - solution is found
     * Slots are filled strictly from first to last as per the input order
     * */
    if (wordIndex >= wordsNumber) {
        return 0;
    }
    if (slotIndex >= slotsNumber) {
        return 1;
    }
    // If the word is not yet used and can be placed in the given slot
    if (!usedWords[wordIndex] && wordFitsSlot(words[wordIndex], slots[slotIndex], grid)) {
        // Place the word into the slot and continue recursively to the next slot
        usedWords[wordIndex] = 1;
        placeWord(0, words[wordIndex], slots[slotIndex], grid);
        if (recursiveWordPlacement(0, wordsNumber, slotIndex + 1, slotsNumber, words, usedWords, slots, grid)) {
            return 1;
        }
        // Backtracking, if there is no solution after placing word into the slot - remove it
        usedWords[wordIndex] = 0;
        removeWord(0, words[wordIndex], slots[slotIndex], grid);
    }
    // The word cannot be placed into this slot, continue recursively to the next word
    return recursiveWordPlacement(wordIndex + 1, wordsNumber, slotIndex, slotsNumber, words, usedWords, slots, grid);
}

/*
 * Task 5, placing words in a crossword skeleton
 * */
void crosswordGenerator() {
    int gridSize = 0;
    int slotsNumber = 0;
    int wordsNumber = 0;
    CrosswordSlot slots[MAX_SLOTS_NUMBER];
    char words[MAX_WORDS_NUMBER][MAX_WORD_LENGTH];
    int usedWords[MAX_WORDS_NUMBER] = {0};
    printf("Please enter the dimensions of the crossword grid:\n");
    scanf("%d", &gridSize);
    printf("Please enter the number of slots in the crossword:\n");
    scanf("%d", &slotsNumber);
    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
    for (int i = 0; i < slotsNumber; ++i) {
        char temp;
        scanf("%d %d %lu %c", &slots[i].row, &slots[i].column, &slots[i].length, &temp);
        if (temp == HORIZONTAL_SLOT_DIRECTION) {
            slots[i].horizontal = 1;
        } else {
            slots[i].horizontal = 0;
        }
    }
    printf("Please enter the number of words in the dictionary:\n");
    scanf("%d", &wordsNumber);
    while (wordsNumber < slotsNumber) {
        printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n",
               slotsNumber);
        scanf("%d", &wordsNumber);
    }
    printf("Please enter the words for the dictionary:\n");
    for (int i = 0; i < wordsNumber; ++i) {
        scanf("%s", words[i]);
    }
    char grid[MAX_CROSSWORD_SIZE][MAX_CROSSWORD_SIZE] = {0};
    if (recursiveWordPlacement(0, wordsNumber, 0, slotsNumber, words, usedWords, slots, grid)) {
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                if (grid[i][j]) {
                    printf("| %c ", grid[i][j]);
                } else {
                    printf("| # ");
                }
            }
            printf("|\n");
        }
    } else {
        printf("This crossword cannot be solved.\n");
    }
}
