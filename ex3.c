/******************
Name: Konstantin Ostashenko
ID: 345548929
Assignment: ex3
*******************/

#include <stdio.h>

#define NUM_OF_BRANDS 5
#define BRANDS_NAMES 15
#define NUM_OF_TYPES 4
#define TYPES_NAMES 10
#define DAYS_IN_YEAR 365

#define ADD_ONE  1
#define ADD_ALL  2
#define STATS  3
#define PRINT  4
#define INSIGHTS  5
#define DELTAS  6
#define DONE  7

char brands[NUM_OF_BRANDS][BRANDS_NAMES] = {"Toyoga", "HyunNight", "Mazduh", "FolksVegan", "Key-Yuh"};
char types[NUM_OF_TYPES][TYPES_NAMES] = {"SUV", "Sedan", "Coupe", "GT"};

int findMaxInArray(int array[], int size);
void printMenu();

void printDailyDataPrompt(int filledBrands[NUM_OF_BRANDS]);
void recordDailyBrandData(int currentDay, int filledBrands[NUM_OF_BRANDS],
                     int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);

void calculateTotalDailyStats(int startingDay, int lastDay, int totalDailySales[DAYS_IN_YEAR],
                              int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);
void calculateBrandStats(int startingDay, int lastDay, int brandSales[NUM_OF_BRANDS],
                         int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);
void calculateCarTypeStats(int startingDay, int lastDay, int carTypeSales[NUM_OF_TYPES],
                           int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);

void provideDailyStats(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);
void printDataCube(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);
void provideOverallInsights(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);
void provideAverageDeltaMetrics(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]);

int main() {
    int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES];
    // initializing the cube with (-1)s
    for (int i = 0; i < DAYS_IN_YEAR; ++i) {
        for (int j = 0; j < NUM_OF_BRANDS; ++j) {
            for (int k = 0; k < NUM_OF_TYPES; ++k) {
                cube[i][j][k] = -1;
            }
        }
    }
    // variables for daily data input
    int filledBrands[NUM_OF_BRANDS] = {0};
    int dataIsIncomplete;

    // day counter
    int latestDay = 0;

    int choice = 0;
    while(choice != DONE) {
        printMenu();
        scanf("%d", &choice);
        switch(choice) {
            case ADD_ONE:
                for (int i = 0; i < NUM_OF_BRANDS; ++i) {
                    filledBrands[i] = 0;
                }
                recordDailyBrandData(latestDay, filledBrands, cube);
                break;
            case ADD_ALL:
                for (int i = 0; i < NUM_OF_BRANDS; ++i) {
                    filledBrands[i] = 0;
                }
                dataIsIncomplete = 1;
                // while data is incomplete
                while (dataIsIncomplete) {
                    printDailyDataPrompt(filledBrands);
                    recordDailyBrandData(latestDay, filledBrands, cube);
                    // assume data to be complete
                    dataIsIncomplete = 0;
                    // if any brand is not yet filled for the current day, data is incomplete
                    for (int i = 0; i < NUM_OF_BRANDS; ++i) {
                        if (!filledBrands[i]) {
                            dataIsIncomplete = 1;
                            break;
                        }
                    }
                }
                latestDay += 1;
                break;
            case STATS:
                provideDailyStats(latestDay, cube);
                break;
            case PRINT:
                printDataCube(latestDay, cube);
                break;
            case INSIGHTS:
                provideOverallInsights(latestDay, cube);
                break;
            case DELTAS:
                provideAverageDeltaMetrics(latestDay, cube);
                break;
            case DONE:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid input\n");
        }
    }
    return 0;
}

/*
 * Find max element in array
 * Arguments: array - int[], should not be changed
 * Arguments: size of array - int
 * Return: index of a max element in the given array
 * */
int findMaxInArray(int array[], int size) {
    int maxIndex = 0;
    for (int i = 0; i < size; ++i) {
        if (array[i] > array[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

/*
 * Print main menu
 * Arguments: None
 * Return: None
 * */
void printMenu() {
    printf("Welcome to the Cars Data Cube! What would you like to do?\n"
           "1.Enter Daily Data For A Brand\n"
           "2.Populate A Day Of Sales For All Brands\n"
           "3.Provide Daily Stats\n"
           "4.Print All Data\n"
           "5.Provide Overall (simple) Insights\n"
           "6.Provide Average Delta Metrics\n"
           "7.exit\n");
}

/*
 * Print the daily data prompt for part 2
 * Arguments: filled brands - int[NUM_OF_BRANDS]
 * Return: None
 * */
void printDailyDataPrompt(int filledBrands[NUM_OF_BRANDS]) {
    printf("No data for brands");
    for (int i = 0; i < NUM_OF_BRANDS; ++i) {
        if (!filledBrands[i]) {
            printf(" %s", brands[i]);
        }
    }
    printf("\n"
           "Please complete the data\n");
}

/*
 * Record daily data for part 1 or part 2
 * Arguments: current day - int,
 * Arguments: filled brands - int[NUM_OF_BRANDS]
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void recordDailyBrandData(int currentDay, int filledBrands[NUM_OF_BRANDS],
                    int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int brandIndex = -1;
    // Input and validate brand
    scanf("%d", &brandIndex);
    if (!(brandIndex >= 0 && brandIndex < NUM_OF_BRANDS && !filledBrands[brandIndex])) {
        printf("This brand is not valid\n");
    } else {
        // Input daily data and save it in the cube
        for (int i = 0; i < NUM_OF_TYPES; ++i) {
            scanf("%d", &cube[currentDay][brandIndex][i]);
        }
    }
    filledBrands[brandIndex] = 1;
}

/*
 * Calculate total daily stats from startingDay till lastDay including
 * Arguments: starting and latest days - int[DAYS_IN_YEAR]
 * Arguments: array of integers to store sales stats into
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void calculateTotalDailyStats(int startingDay, int lastDay, int totalDailySales[DAYS_IN_YEAR],
                              int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    for (int i = startingDay; i <= lastDay; ++i) {
        for (int j = 0; j < NUM_OF_BRANDS; ++j) {
            for (int k = 0; k < NUM_OF_TYPES; ++k) {
                totalDailySales[i] += cube[i][j][k];
            }
        }
    }
}

/*
 * Calculate brand stats from startingDay till lastDay including
 * Arguments: starting and latest days - int[NUM_OF_BRANDS]
 * Arguments: array of integers to store sales stats into
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void calculateBrandStats(int startingDay, int lastDay, int brandSales[NUM_OF_BRANDS],
                              int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    for (int i = startingDay; i <= lastDay; ++i) {
        for (int j = 0; j < NUM_OF_BRANDS; ++j) {
            for (int k = 0; k < NUM_OF_TYPES; ++k) {
                brandSales[j] += cube[i][j][k];
            }
        }
    }
}

/*
 * Calculate car type stats from startingDay till lastDay including
 * Arguments: starting and latest days - int[NUM_OF_TYPES]
 * Arguments: array of integers to store sales stats into
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void calculateCarTypeStats(int startingDay, int lastDay, int carTypeSales[NUM_OF_TYPES],
                         int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    for (int i = startingDay; i <= lastDay; ++i) {
        for (int j = 0; j < NUM_OF_BRANDS; ++j) {
            for (int k = 0; k < NUM_OF_TYPES; ++k) {
                carTypeSales[k] += cube[i][j][k];
            }
        }
    }
}

/*
 * Provide daily stats
 * Arguments: latest recorded day - int,
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void provideDailyStats(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int chosenDay = -1;
    printf("What day would you like to analyze?\n");
    while (!(scanf("%d", &chosenDay) == 1 && chosenDay >= 1 && chosenDay <= latestDay)) {
        printf("Please enter a valid day.\n");
        printf("What day would you like to analyze?\n");
    }
    // Input is 1-365 instead of 0-364, so we need to subtract 1
    chosenDay -= 1;

    int totalDailySales[DAYS_IN_YEAR] = {0};
    int brandSales[NUM_OF_BRANDS] = {0};
    int carTypeSales[NUM_OF_TYPES] = {0};
    calculateTotalDailyStats(chosenDay, chosenDay, totalDailySales, cube);
    calculateBrandStats(chosenDay, chosenDay, brandSales, cube);
    calculateCarTypeStats(chosenDay, chosenDay, carTypeSales, cube);
    int maxBrandSalesIndex = findMaxInArray(brandSales, NUM_OF_BRANDS);
    int maxCarTypeSalesIndex = findMaxInArray(carTypeSales, NUM_OF_TYPES);
    // Output day is still 1-365, not 0-364
    printf("In day number %d:\n", chosenDay + 1);
    printf("The sales total was %d\n", totalDailySales[chosenDay]);
    printf("The best sold brand with %d sales was %s\n",
           brandSales[maxBrandSalesIndex], brands[maxBrandSalesIndex]);
    printf("The best sold type with %d sales was %s\n",
           carTypeSales[maxCarTypeSalesIndex], types[maxCarTypeSalesIndex]);
}

/*
 * Print the data cube
 * Arguments: latest recorded day - int,
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void printDataCube(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    printf("*****************************************\n"
           "\n");
    for (int i = 0; i < NUM_OF_BRANDS; ++i) {
        printf("Sales for %s:\n", brands[i]);
        for (int j = 0; j < latestDay; ++j) {
            printf("Day %d-", j + 1);
            for (int k = 0; k < NUM_OF_TYPES; ++k) {
                printf(" %s: %d", types[k], cube[j][i][k]);
            }
            printf("\n");
        }
    }
    printf("\n"
           "*****************************************\n");
}

/*
 * Provide overall insights
 * Arguments: latest recorded day - int,
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void provideOverallInsights(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    int totalDailySales[DAYS_IN_YEAR] = {0};
    int brandSales[NUM_OF_BRANDS] = {0};
    int carTypeSales[NUM_OF_TYPES] = {0};
    calculateTotalDailyStats(0, latestDay - 1, totalDailySales, cube);
    calculateBrandStats(0, latestDay - 1, brandSales, cube);
    calculateCarTypeStats(0, latestDay - 1, carTypeSales, cube);
    int maxDailySalesIndex = findMaxInArray(totalDailySales, latestDay);
    int maxBrandSalesIndex = findMaxInArray(brandSales, NUM_OF_BRANDS);
    int maxCarTypeSalesIndex = findMaxInArray(carTypeSales, NUM_OF_TYPES);
    printf("The best-selling brand overall is %s: %d$\n",
           brands[maxBrandSalesIndex], brandSales[maxBrandSalesIndex]);
    printf("The best-selling type of car is %s: %d$\n",
           types[maxCarTypeSalesIndex], carTypeSales[maxCarTypeSalesIndex]);
    printf("The most profitable day was day number %d: %d$\n",
           maxDailySalesIndex + 1, totalDailySales[maxDailySalesIndex]);
}

/*
 * Provide average delta metrics
 * Arguments: latest recorded day - int,
 * Arguments: data cube - int[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]
 * Return: None
 * */
void provideAverageDeltaMetrics(int latestDay, int cube[DAYS_IN_YEAR][NUM_OF_BRANDS][NUM_OF_TYPES]) {
    if (latestDay <= 1) {
        for (int i = 0; i < NUM_OF_BRANDS; ++i) {
            // printing -0.0 according to how the provided executable works
            printf("Brand: %s, Average Delta: %f\n", brands[i], -0.0);
        }
        return;
    }
    int brandDailyDeltas[DAYS_IN_YEAR][NUM_OF_BRANDS];
    for (int i = 0; i < latestDay; ++i) {
        for (int j = 0; j < NUM_OF_BRANDS; ++j) {
            brandDailyDeltas[i][j] = 0;
        }
        calculateBrandStats(i, i, brandDailyDeltas[i], cube);
    }
    for (int i = 0; i < latestDay - 1; ++i) {
        for (int j = 0; j < NUM_OF_BRANDS; ++j) {
            /*
             * Calculate deltas in-place
             * e.g. A = [1, 4, 2, 9]
             * -> [4-1=3, 4, 2, 9] -> [3, 2-4=-2, 2, 9] -> [3, -2, 9-2=7, 9]
             * Resulting array will store N-1 deltas starting from index 0
             * */
            brandDailyDeltas[i][j] = brandDailyDeltas[i + 1][j] - brandDailyDeltas[i][j];
        }
    }
    for (int i = 0; i < NUM_OF_BRANDS; ++i) {
        double sum = 0;
        for (int j = 0; j < latestDay - 1; ++j) {
            sum += brandDailyDeltas[j][i];
        }
        printf("Brand: %s, Average Delta: %f\n", brands[i], sum / (latestDay - 1));
    }
}
