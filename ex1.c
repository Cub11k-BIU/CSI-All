/******************
Name: Konstantin Ostashenko
ID: 345548929
Assignment: ex1
*******************/
#include <stdio.h>

// REMINDER : YOU CANT USE ANY CONTROL FLOW OPERATIONS OR FUNCTIONS, ONLY BITWISE.

int main() {
    // assuming the number to be integer, we'll use it for 4 tasks
    int number = 0;
    // assuming the position to be 0 <= idx <= 31, no need for int, we'll use it for 3 tasks
    char idx = 0;
    // for some tasks, a bit mask of the same size as number would be useful
    int mask = 0;


    // 1. What bit
    printf("What bit:\n");
    /*
     * Scan two integers (representing number and a position)
     * Print the bit in this position.
     *
     * 1. To get the value of the bit in position idx we need to do the following:
     * 1.1. (number >> idx) moves the bit we need to the rightmost position
     * 1.2. ((number >> idx) & 1) unsets all bits except the rightmost
     * The result is an int containing a value of the bit in position idx
     * */
    printf("Please enter a number:\n");
    // %d - decimal, pointer to an int
    scanf("%d", &number);
    printf("Please enter a position:\n");
    // %hhd - decimal, hh modifier - pointer to a signed char or unsigned char
    scanf("%hhd", &idx);
    printf("The bit in position %hhd of number %d is: %d\n",
           idx, number, (number >> idx) & 1);


    // 2. and 3. Set bit
    printf("\nSet bit:\n");
    /*
     * Scan two integers (representing number and a position)
     * Make sure the bit in this position is "on" (equal to 1)
     * Print the output
     * Now make sure it's "off" (equal to 0)
     * Print the output
     *
     * 2. To set a bit in position idx and leave all others untouched we need to do the following:
     * 2.1. Calculate a mask for bitwise OR: 0...010...0 where 1 is a bit in position idx
     * 2.1.1. X | 1 = 1 => set
     * 2.1.2. X | 0 = X => left untouched
     * 2.2. This mask can be calculated as: (1 << idx)
     * 2.3. (number | mask) applies the mask
     * The result is an int containing a value of the number with bit in position idx being set
     *
     * 3. To unset a bit in position idx and leave all others untouched we need to do the following:
     * 3.1. Calculate a mask for bitwise AND: 1...101...1 where 0 is a bit in position idx
     * 3.1.1. X & 0 = 0 => unset
     * 3.1.2. X & 1 = X => left untouched
     * 3.2. This mask can be calculated as: (~(1 << idx))
     * 3.3. (number & mask) applies the mask
     * The result is an int containing a value of the number with bit in position idx being unset
     * */
    // specifically not reusing the mask value for better readability
    printf("Please enter a number:\n");
    // %d - decimal, pointer to an int
    scanf("%d", &number);
    printf("Please enter a position:\n");
    // %hhd - decimal, hh modifier - pointer to a signed char or unsigned char
    scanf("%hhd", &idx);
    mask = 1 << idx;
    printf("Number with bit %hhd set to 1: %d\n", idx, number | mask);
    mask = ~(1 << idx);
    printf("Number with bit %hhd set to 0: %d\n", idx, number & mask);


    // 4. Toggle bit
    printf("\nToggle bit:\n");
    /*
     * Scan two integers (representing number and a position)
     * Toggle the bit in this position
     * Print the new number
     *
     * 4. To toggle a bit in position idx and leave all others untouched we need to do the following:
     * 4.1. Calculate a mask for bitwise XOR: 0...010...0 where 1 is a bit in position idx
     * 4.1.1. X ^ 1 = ~X => toggled
     * 4.1.2. X ^ 0 = X => left untouched
     * 4.2. This mask can be calculated as: (1 << idx)
     * 4.3. (number ^ mask) applies the mask
     * The result is an int containing a value of the number with bit in position idx being toggled
     * */
    printf("Please enter a number:\n");
    // %d - decimal, pointer to an int
    scanf("%d", &number);
    printf("Please enter a position:\n");
    // %hhd - decimal, hh modifier - pointer to a signed char or unsigned char
    scanf("%hhd", &idx);
    mask = 1 << idx;
    printf("Number with bit %hhd toggled: %d\n", idx, number ^ mask);


    // 5. Even - Odd
    printf("\nEven - Odd:\n");
    /*
     * Scan an integer
     * If the number is even - print 1, else - print 0.
     *
     * 5. To determine whether the number is even or odd we need to examine its rightmost bit
     * 5.1. (number & 1) isolates the rightmost bit, unsetting all others
     * 5.2.1. If the rightmost bit is set, then the number is odd, we need to print 0
     * 5.2.2. If the rightmost bit is unset, then the number is even, we need to print 1
     * 5.3. To print 0 and 1 accordingly, we need to toggle the rightmost bit
     * 5.4. We already know how to toggle a bit in position idx, so now we just need to do it for idx=0
     * 5.5. Another way to do this would be: ((number - 1) & 1), but let's stick to bitwise operations
     * */
    printf("Please enter a number:\n");
    // %d - decimal, pointer to an int
    scanf("%d", &number);
    printf("%d\n", (number & 1) ^ 1);


    // 6. 3, 5, 7, 11
    printf("\n3, 5, 7, 11:\n");
    /*
     * Scan two integers in octal base
     * Sum them up and print the result in hexadecimal base
     * Print only 4 bits, in positions: 3,5,7,11 in the result.
     *
     * 6.1. To output the sum in hexadecimal(capitalized) we need to use the %X conversion specifier
     * with the %l size specifier for long
     * The result is a hexadecimal unsigned long int (%lX) containing value of the sum
     * 6.2. To output the bits in positions 3, 5, 7 and 11 we need to use the formula from task 1.
     * with idx=3, idx=5, idx=7 and idx=11
     * The results are unsigned long ints (%lu) containing values of bits 3, 5, 7 and 11
     * */

    /*
     * assuming both numbers to be integers
     * should be using unsigned int as per %o conversion specifier in C99 standard,
     * but I'll stick to int as per the task definition
     * */
    int firstNumber = 0;
    int secondNumber = 0;
    int sum = 0;
    printf("Please enter the first number (octal):\n");
    // %o - octal, should be a pointer to an unsigned int as per C99 standard
    scanf("%o", &firstNumber);
    printf("Please enter the second number (octal):\n");
    // %o - octal, should be a pointer to an unsigned int as per C99 standard
    scanf("%o", &secondNumber);
    sum = firstNumber + secondNumber;
    printf("The sum in hexadecimal: %X\n", sum);
    printf("The 3,5,7,11 bits are: %d%d%d%d\n",
           (sum >> 3) & 1,
           (sum >> 5) & 1,
           (sum >> 7) & 1,
           (sum >> 11) & 1);

    printf("Bye!\n");

    return 0;
}