/******************
Name: Konstantin Ostashenko
ID: 345548929
Assignment: ex2
*******************/

#include <stdio.h>

int main() {
    // general constants
    const int divisorForEvenOddCheck = 2;
    const int divisorForHalving = 2;
    /*
     * Leaving TA's comments as is, declaring variables near them
     * The main loop and switch case starts at line 119
     *
     * The amount of constants is probably overwhelming, and I am sorry for that
     * Although we never got the exact definition of a "magic number" and/or
     * any exceptions (I am counting 0 and 1 as obvious exceptions in some cases)
     * This amount of constants kinda lowers the overall readability of the code,
     * generally speaking so does the absence of functions
     * Hoping to be understood and being sorry for whoever has to go through all this...
     * */

	// Case 1: Draw Happy Face with given symbols for eyes, nose and mouse
	/* Example:
	* n = 3:
	* 0   0
	*   o
	* \___/
	*/
    // variables for case 1
    char eye;
    char nose;
    char mouth;
    int faceSize;

	// Case 2: determine whether the sum of all digits to the left of the middle digit(s)
	// and the sum of all digits to the right of the middle digit(s) are equal
	/* Examples:
	Balanced: 1533, 450810, 99
	Not blanced: 1552, 34
	Please notice: the number has to be bigger than 0.
	*/
    // constants for case 2
    const int balancedCountingBase = 10;
    // variables for case 2
    int digitsCount;
    long long int sumLeft;
    long long int sumRight;
    long long int balancedNumber;
    long long int numberCopy;

	// Case 3: determine whether the sum of the proper divisors (od an integer) is greater than the number itself
	/* Examples:
	Abudant: 12, 20, 24
	Not Abudant: 3, 7, 10
	Please notice: the number has to be bigger than 0.
	*/
    // constants for case 3
    const int nextDivisorStep = 1;
    const int properDivisorOfAnyNumber = 1;
    const int minProperDivisor = 2;
    // variables for case 3
    long long int generousNumber;
    long long int properDivisorSum;
    long long int currentDivisor;

	// Case 4: determine wether a number is a prime.
	/* Examples:
	This one brings joy: 3, 5, 11
	This one does not bring joy: 15, 8, 99
	Please notice: the number has to be bigger than 0.
	*/
    // constants for case 4
    const int minPrimeNumber = 2;
    const int minDivisor = 2;
    const int circleOfJoyCountingBase = 10;
    // variables for case 4
    int isPrime;
    long long int primeNumber;
    long long int reversedPrimeNumber;
    

	// Happy numbers: Print all the happy numbers between 1 to the given number.
	// Happy number is a number which eventually reaches 1 when replaced by the sum of the square of each digit
	/* Examples:
	Happy :) : 7, 10
	Not Happy :( : 5, 9
	Please notice: the number has to be bigger than 0.
	*/
    // constants for case 5
    const int minHappyNumber = 1;
    const int happyFinishNumber = 1;
    const int unhappyLoopNumber = 4;
    const int happyCountingBase = 10;
    // variables for case 5
    long long int maxHappyNumber;
    long long int currentHappyCandidate;
    long long int currentHappySum;

	// Festival of Laughter: Prints all the numbers between 1 the given number:
	// and replace with "Smile!" every number that divided by the given smile number
	// and replace with "Cheer!" every number that divided by the given cheer number
	// and replace with "Festival!" every number that divided by both of them
	/* Example:
	6, smile: 2, cheer: 3 : 1, Smile!, Cheer!, Smile!, 5, Festival!
	*/
    // constants for case 6
    const int numberOfSmileCheerArgs = 2;
    const int numberOfMaxFestivalArgs = 1;
    const int minFestivalNumber = 1;
    // variables for case 6
    long long int smileNumber;
    long long int cheerNumber;
    long long int maxFestivalNumber;

    // variable to store current chosen option
    int option = 0;
    /* enum for options
     * We haven't yet spoken about enums, but enum and define are the only options
     * to avoid using magic numbers for switch case, as they must be determined
     * during compile-time, const int doesn't work here
     * */
    enum MenuOption {
        HAPPY_FACE_MENU_OPTION = 1,
        BALANCED_NUMBER_MENU_OPTION = 2,
        GENEROUS_NUMBER_MENU_OPTION = 3,
        CIRCLE_OF_JOY_MENU_OPTION = 4,
        HAPPY_NUMBER_MENU_OPTION = 5,
        FESTIVAL_OF_LAUGHTER_MENU_OPTION = 6,
        EXIT_MENU_OPTION = 7
    };
    // main loop, running until user chooses to exit
    while (option != EXIT_MENU_OPTION) {
        // initial menu
        printf("Choose an option:\n"
               "\t1. Happy Face\n"
               "\t2. Balanced Number\n"
               "\t3. Generous Number\n"
               "\t4. Circle Of Joy\n"
               "\t5. Happy Numbers\n"
               "\t6. Festival Of Laughter\n"
               "\t7. Exit\n");
        // getting chosen option from user, leading space to skip any trailing whitespaces
        scanf(" %d", &option);
        if (!(option >= HAPPY_FACE_MENU_OPTION && option <= EXIT_MENU_OPTION)) {
            printf("This option is not available, please try again.\n");
        } else {
            // determining which task to perform
            switch (option) {
                case HAPPY_FACE_MENU_OPTION:
                    // initialize all variables
                    eye = 0;
                    nose = 0;
                    mouth = 0;
                    faceSize = 0;
                    printf("Enter symbols for the eyes, nose, and mouth:\n");
                    // leading space in the format to skip any trailing whitespaces
                    scanf(" %c %c %c", &eye, &nose, &mouth);
                    printf("Enter face size:\n");
                    scanf("%d", &faceSize);
                    while (!(faceSize > 0 && faceSize % divisorForEvenOddCheck)) {
                        printf("The face's size must be an odd and positive number, please try again:\n");
                        scanf("%d", &faceSize);
                    }
                    /*
                     * "*" is a width modifier, providing necessary padding with spaces
                     * For eyes we need faceSize spaces, so the width of a right eye is going to be
                     * faceSize + 1
                     * For nose we need (faceSize + 1)/2 space, so the width of a nose is going to
                     * be ((faceSize + 1)/2)+1
                     * For mouth we cannot use space padding, so we'll just use a for loop
                     * */
                    printf("%c%*c\n", eye, faceSize + 1, eye);
                    printf("%*c\n", ((faceSize + 1) / divisorForHalving) + 1, nose);
                    printf("\\");
                    for (int i = 0; i < faceSize; ++i) {
                        printf("%c", mouth);
                    }
                    printf("/\n");
                    break;
                case BALANCED_NUMBER_MENU_OPTION:
                    // initialize all variables
                    digitsCount = 0;
                    sumRight = 0;
                    sumLeft = 0;
                    balancedNumber = 0;
                    numberCopy = 0;
                    printf("Enter a number:\n");
                    scanf("%lld", &balancedNumber);
                    while (balancedNumber <= 0) {
                        printf("Only positive number is allowed, please try again:\n");
                        scanf("%lld", &balancedNumber);
                    }
                    /*
                     * To determine whether the number is balanced or not,
                     * we first need to determine how many digits it comprises */
                    numberCopy = balancedNumber;
                    while (numberCopy) {
                        digitsCount += 1;
                        numberCopy /= balancedCountingBase;
                    }
                    // Now to the calculation of the right sum
                    for (int i = 0; i < digitsCount / divisorForHalving; ++i) {
                        sumRight += balancedNumber % balancedCountingBase;
                        balancedNumber /= balancedCountingBase;
                    }
                    // If the number of digits is odd, skip the middle digit
                    if (digitsCount % divisorForEvenOddCheck) {
                        balancedNumber /= balancedCountingBase;
                    }
                    // Now to the calculation of the left sum
                    for (int i = 0; i < digitsCount / divisorForHalving; ++i) {
                        sumLeft += balancedNumber % balancedCountingBase;
                        balancedNumber /= balancedCountingBase;
                    }
                    if (sumLeft == sumRight) {
                        printf("This number is balanced and brings harmony!\n");
                    } else {
                        printf("This number isn't balanced and destroys harmony.\n");
                    }
                    break;
                case GENEROUS_NUMBER_MENU_OPTION:
                    // initialize all variables
                    generousNumber = 0;
                    properDivisorSum = 0;
                    currentDivisor = 0;
                    printf("Enter a number:\n");
                    scanf("%lld", &generousNumber);
                    while (generousNumber <= 0) {
                        printf("Only positive number is allowed, please try again:\n");
                        scanf("%lld", &generousNumber);
                    }
                    // 1 is a proper divisor of any number > 1, adding it to the sum
                    if (generousNumber > properDivisorOfAnyNumber) {
                        properDivisorSum += properDivisorOfAnyNumber;
                    }
                    /*
                     * Loop variable currentDivisor is going to iterate from 2 up until
                     * sqrt(generousNumber)
                     * Each iteration gives us two possible proper divisors:
                     * currentDivisor and (generousNumber / currentDivisor)
                     * */
                    currentDivisor = minProperDivisor;
                    while (currentDivisor * currentDivisor <= generousNumber) {
                        // currentDivisor is a proper divisor
                        if (!(generousNumber % currentDivisor)) {
                            // proper divisor: currentDivisor
                            properDivisorSum += currentDivisor;
                            // currentDivisor is not equal to sqrt(generousNumber)
                            if (currentDivisor * currentDivisor != generousNumber) {
                                /*
                                 * currentDivisor is not equal to (generousNumber / currentDivisor)
                                 * it can also be added to the sum
                                 * */
                                properDivisorSum += generousNumber / currentDivisor;
                            }
                        }
                        currentDivisor += nextDivisorStep;
                    }
                    if (properDivisorSum > generousNumber) {
                        printf("This number is generous!\n");
                    } else {
                        printf("This number does not share.\n");
                    }
                    break;
                case CIRCLE_OF_JOY_MENU_OPTION:
                    // initializing all variables
                    isPrime = 1;
                    primeNumber = 0;
                    reversedPrimeNumber = 0;
                    printf("Enter a number:\n");
                    scanf("%lld", &primeNumber);
                    while (primeNumber <= 0) {
                        printf("Only positive number is allowed, please try again:\n");
                        scanf("%lld", &primeNumber);
                    }
                    if (primeNumber < minPrimeNumber) {
                        isPrime = 0;
                    }
                    // if the number is still a candidate to be prime, proceed
                    if (isPrime) {
                        // Determine whether primeNumber is indeed prime (approach from task 3)
                        for (long long int i = minDivisor; i < primeNumber; ++i) {
                            if (!(primeNumber % i)) {
                                isPrime = 0;
                                break;
                            }
                        }
                    }
                    // if the number is still a candidate to be prime, proceed
                    if (isPrime) {
                        // reverse primeNumber and store it in reversedPrimeNumber
                        while (primeNumber) {
                            reversedPrimeNumber *= circleOfJoyCountingBase;
                            reversedPrimeNumber += primeNumber % circleOfJoyCountingBase;
                            primeNumber /= circleOfJoyCountingBase;
                        }
                        // Determine whether reversedPrimeNumber is also prime (approach from task 3)
                        for (long long int i = minDivisor; i < reversedPrimeNumber; ++i) {
                            if (!(reversedPrimeNumber % i)) {
                                isPrime = 0;
                                break;
                            }
                        }
                    }
                    if (isPrime) {
                        printf("This number completes the circle of joy!\n");
                    } else {
                        printf("The circle remains incomplete.\n");
                    }
                    break;
                case HAPPY_NUMBER_MENU_OPTION:
                    // initializing all variables
                    maxHappyNumber = 0;
                    currentHappyCandidate = 0;
                    currentHappySum = 0;
                    printf("Enter a number:\n");
                    scanf("%lld", &maxHappyNumber);
                    while (maxHappyNumber <= 0) {
                        printf("Only positive number is allowed, please try again:\n");
                        scanf("%lld", &maxHappyNumber);
                    }
                    printf("Between %d and %lld only these numbers bring happiness: ",
                           minHappyNumber, maxHappyNumber);
                    // Iterate over [minHappyNumber,...,maxHappyNumber]
                    for (long long int i = minHappyNumber; i <= maxHappyNumber; ++i) {
                        currentHappyCandidate = i;
                        currentHappySum = i;
                        /*
                         * As Wikipedia states, there is only one loop for 10-happy numbers:
                         * 4 -> 16 -> 37 -> 58 -> 89 -> 145 -> 42 -> 20 -> 4
                         * So if, at any point, we arrive to a member of this loop, we know
                         * that the currentHappyCandidate is not happy
                         * For the sake of simplicity, we will only check if candidate is equal
                         * to 4(unhappyLoopNumber). At most 7 extra iterations are an acceptable margin
                         * */
                        while (!(currentHappyCandidate == happyFinishNumber
                                || currentHappyCandidate == unhappyLoopNumber)) {
                            currentHappySum = 0;
                            // Loop to calculate the sum of squares of all digits
                            while (currentHappyCandidate > 0) {
                                long long int currentLastDigit = currentHappyCandidate % happyCountingBase;
                                currentHappySum += currentLastDigit * currentLastDigit;
                                currentHappyCandidate /= happyCountingBase;
                            }
                            currentHappyCandidate = currentHappySum;
                        }
                        if (currentHappySum == happyFinishNumber) {
                            printf("%lld ", i);
                        }
                    }
                    // line break after all happy numbers have been printed
                    printf("\n");
                    break;
                case FESTIVAL_OF_LAUGHTER_MENU_OPTION:
                    // initializing all variables
                    smileNumber = 0;
                    cheerNumber = 0;
                    maxFestivalNumber = 0;
                    printf("Enter a smile and cheer number:\n");
                    /*
                     * format string ensures that:
                     * all trailing whitespaces are skipped
                     * the string "smile:" is in the input before the first number
                     * any amount of whitespaces following a first number is in the input
                     * the string ", cheer:" is in the input after a first number
                     * any amount of whitespaces following a second number is in the input
                     *
                     * scanf function returns the number of successfully read arguments
                     * If this number is not equal to 2, reading has failed, for whatever reason,
                     * and must be repeated
                     * */
                    while (!(scanf(" smile: %lld, cheer: %lld", &smileNumber, &cheerNumber) == numberOfSmileCheerArgs
                             && smileNumber > 0 && cheerNumber > 0 && smileNumber != cheerNumber)) {
                        printf("Only 2 different positive numbers in the given format are allowed"
                               " for the festival, please try again:\n");
                        // clear buffer after invalid format
                        scanf("%*[^\n]");
                    }
                    // clear buffer after valid format in case of any leftovers
                    scanf("%*[^\n]");
                    printf("Enter maximum number for the festival:\n");
                    while (!(scanf("%lld", &maxFestivalNumber) == numberOfMaxFestivalArgs && maxFestivalNumber > 0)) {
                        printf("Only positive maximum number is allowed, please try again:\n");
                    }
                    for (long long int i = minFestivalNumber; i <= maxFestivalNumber; ++i) {
                        if (!(i % smileNumber)) {
                            if (!(i % cheerNumber)) {
                                printf("Festival!\n");
                            } else {
                                printf("Smile!\n");
                            }
                        } else if (!(i % cheerNumber)) {
                            printf("Cheer!\n");
                        } else {
                            printf("%lld\n", i);
                        }
                    }
                    break;
                default:
                    // exit option
                    printf("Thank you for your journey through Numeria!\n");
                    break;
            }
        }
    }
	return 0;
}
