//Erhan Yalnýz  -  150117905  -  HOMEWORK1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Will be used as max line of characters in string inputs.
#define MAX_LINE 100
// 2 bytes per unsigned/signed integers.
#define INT_SIZE 2
// Used to allocate max space for decimal to binary operations.
#define MAX_PRECISION 64

// This variable will hold each one of the lines in input file as a array of strings.
// After will be used to hold binary equivalents of each.
// Finally will be used to hold hexadecimals that we will write to file.
char **datas;

// Simple function to calculate 2^n.
long powerOfTwo(int n);

// Convert given string of integer decimal to binary equivalent with "n_bits" bits.
char *decimalToBin(char *integerString, int n_bits);

// Convert string of a fraction decimal to binary equivalent with "n_bits" bits. Ex: 0.25 ---> 01 (binary equivalent)
char *fractionToBin(char *floatString, int n_bits);

// Round near even a given binary fraction with "n_bits" bits. Ex: str = "1011", n_bits = 3 ---> "110"
char *roundNearEven(char *str, int n_bits);

// Convert a string of binary numbers to hexadecimal equivalent.
char *binToHex(char *binaryString);

// Convert given string to binary equivalent with float byte size "n_bytes". This function implements everyone of signed, unsigned and float conversions.
char *convert(char *str, int n_bytes);

// Convert given file "filename" to hexadecimals with float byte size "n_bytes" and write output to array of strings: "outputArray".
char **convertFile(char *filename, char **outputArray, int n_bytes);

// Write given array of strings: "outputArray" to output file with name: "filename" according to endianness of 'B' for Big Endian or 'L' for Little Endian.
void writeOutputFile(char *filename, char **outputArray, char endianness);

int main(){
    // Create input variables that will be taken from user.
    char endianness, s[MAX_LINE];
    int n_bytes;
    // Get the byte ordering (Endianness) first.
    printf("Byte ordering: ");
    // Read till newline is reached.
    scanf("%[^\n]s", s);
    // get the first character as it will represent endianness with 'L' or 'B'.
    endianness = s[0];
    printf("Floating point size: ");
    // Flush out remaining '\n' then read the number of bytes from user.
    scanf("%*c%d", &n_bytes);
    // Convert given "input.txt" file into a string array "datas" filled with hexadecimal equivalents. Use float byte size: "n_bytes".
    datas = convertFile("input.txt", datas, n_bytes);
    // Write resulting string array to "output.txt" according to endianness represented with "endianness" character variable.
    writeOutputFile("output.txt", datas, endianness);
    return 0;
}

// Simple function to calculate 2^n.
long powerOfTwo(int n){
    // Starting from 2^0 assign res 1 so the algorithm multiplies it by 2 everytime to get 2^n.
    int res = 1;
    for(;n > 0;n--){
        res*=2;
    }
    return res;
}

// Convert given string of integer decimal to binary equivalent with "n_bits" bits.
char *decimalToBin(char *integerString, int n_bits){
    // Parse integer from given integer string "integerString".
    int i, integer = atoi(integerString);
    // Allocate space for resulting binary string.
    char *result = (char *) malloc(n_bits + 1);
    // Iterate through every bit of given result with "n_bits" bits.
    for(i = 0; i < n_bits; i++){
        // Use reverse index when assigning current bit of result so we can start from least significant bit.
        // Taking bitwise "AND" operation with current bit of "integer" shifted to right by "i" bits will give current bit.
        result[n_bits - 1 - i] = ((integer >>  i) & 1) ? '1' : '0';
    }
    // Assign null at the and to terminate string.
    result[i] = '\0';
    return result;
}
// Convert string of a fraction decimal to binary equivalent with "n_bits" bits. Ex: 0.25 ---> 01 (binary equivalent)
char *fractionToBin(char *floatString, int n_bits){
    int i;
    // Parse float from fraction "floatString".
    double fraction = atof(floatString), remainingFraction;
    // Allocate space for resulting binary string.
    char *result = (char *) malloc(n_bits + 1);
    // Iterate through every bit of given result with "n_bits" bits.  
    for(i = 0; i < n_bits; i++){
        // Multiply by 2 to get current bit in fraction.
        fraction *= 2;
        // get the remaining fraction part for next iteration.
        remainingFraction = fraction - ((int) fraction);
        // If there is a integer part of fraction in current iteration current binary digit is 1 otherwise 0.
        result[i] = ((int) fraction) ? '1' : '0';
        // If there is no remaining fraction break the loop as the binary fraction is complete.
        if(remainingFraction == 0){
            i++;
            break;
        }
        // assign remaining non-integer part to fraction.
        fraction = remainingFraction;
    }
    // Fill other bits with 0.
    for(;i < n_bits; i++){
        result[i] = '0';
    }
    // Terminate binary string with null.
    result[i] = '\0';
    return result;
}

// Convert a string of binary numbers to hexadecimal equivalent.
char *roundNearEven(char *str, int n_bits){
    // Get the number of bits from "str" binary string.
    // This will be used when rounding to even, adding 1 to binary string if it is odd.
    int i, n = strlen(str);
    // Allocate space for resulting binary string.
    char *result = (char *) malloc(n_bits + 1);
    // Copy "n_bits" bits of binary string "str" to "result". Truncating the input binary string.
    strncpy(result, str, n_bits);
    // If number of bits to round to is less than current bits in "str" then round off.
    if(n > n_bits){
        // Terminate "result" binary string with null.
        result[n_bits] = '\0';
        // If resulting binary string has '0' bit at the and then it is already a even number so return "result".
        if(result[n_bits - 1] == '0'){
            return result;
        }
        // Otherwise It must be rounded off by incrementing it by 1.
        else{
            // Iterate through each bits starting from least significant bit.
            for(i=n_bits - 1;i >= 0; i--){
                // If current bit is 1 adding 1 to it makes current it 0.
                if(str[i] == '1'){
                    result[i] = '0';
                }
                // If current bit is 0 addition stops by just making current bit 1.
                else{
                    result[i] = '1';
                    break;
                }
                
            }
            // Return resulting binary string from addition.
            return result;
        }
    }
    // If "str" binary string has less number of bits then bits function is trying to truncate to then just fill the rest of the bits with 0.
    else if(n < n_bits){
        for(i=n;i < n_bits; i++){
            result[i] = '0';
        }
        // Return resulting binary string.
        return result;
    }
    else{
        // Return "str" otherwise (n_bits and number of bits in "str" is equivalent).
        return str;
    }
}

// Convert a string of binary numbers to hexadecimal equivalent.
char *binToHex(char *binaryString){
    // Get number of bits in "binaryString". xdigit will be used to determine current hexadecimal.
    int i, j, xdigit, n = strlen(binaryString);
    // k will be used to hold number of hexadecimals it would take to convert "binaryString" to hexadecimals.
    int k = (n%4 != 0) ? n/4 : n/4 - 1;
    // Allocate space for resulting hexadecimal string.
    char *result = (char *) malloc(k + 1);
    // Iterate through each 4 bit groups in "binaryString" by decrementing with i.
    for(i = n - 1; i > 0; i-=4){
        // assume xdigit to be 0.
        xdigit = 0;
        // Iterate through each bit in 4 bit group.
        for(j = i; j>i-4;j--){
            // Do not let j to get out of bounds relative to index in "binaryString". 
            if(j < 0)
                break;
            // Increment xdigit by current power of bit. (Ex: 0110 ---> 2. bit current ---> i-j = 1 ---> 2^1)
            xdigit += (binaryString[j] == '1') ? powerOfTwo(i-j) : 0;
        }
        // Get the corresponding character to xdigit.
        if(xdigit <= 9){
            // If the xdigit is between 0 and 9 then get it using this shortcut.
            result[k] = '0' + xdigit;
        }
        else{
            // If the xdigit is between 10 and 15  then get it using this shortcut.
            result[k] = 'A' + xdigit - 10;
        }
        // Decrease index of resulting hexadecimal string.
        k--;
    }
    // Terminate hexadecimal string with null.
    result[n] = '\0'; 
    return result;
}

// Convert given string to binary equivalent with float byte size "n_bytes". This function implements everyone of signed, unsigned and float conversions.
char *convert(char *str, int n_bytes){
    // Get number of characters in "str". "b" and "E" will be used to calculate bias in float representation.
    int n = strlen(str), i, b, E, exponent_digits;
    // Allocate space for resulting binary string.
    char *result = (char *) malloc(MAX_LINE), *ch, *left, *right, *exponent, *fraction;
    ch = strchr(str, 'u');
    // If there is a 'u' char found in current line save it as a unsigned integer.
    if(ch != NULL){
        // Remove last character as it is 'u' representing unsigned integer.
        str[n-1] = '\0';
        // Convert to binary string using function we defined. Enter the number of bits using constant INT_SIZE multiplied by 8.
        result = decimalToBin(str, INT_SIZE * 8);
    }
    // If there is no 'u' char found in current line save it as a either float or signed integer.
    else{
        // According to given IEEE-like format
        switch (n_bytes)
        {
        case 1:
            exponent_digits = 4;
            break;
        case 2:
            exponent_digits = 6;
            break;
        case 3:
            exponent_digits = 8;
            break;
        case 4:
            exponent_digits = 10;
            break;
        case 6:
            exponent_digits = 14;
            break;
        }

        // Find floating point to see if it is signed character or float.
        ch = strchr(str, '.');
        // If there is a '.' char found in current line save it as a float.
        if(ch != NULL){
            // Check for Sign bit and assign it.
            result[0] = (str[0] == '-') ? '1' : '0';
            result[1] = '\0';
            // If there was a sign in input string increment by 1 to disregard it.
            if(str[0] == '-')
                str++;
            
            // get the index of floating point by using pointer arithmetic.
            i = ch - str;
            // assign parts after floating point to "right" string.
            right = ++ch;
            // split "str" from its fraction part by terminating from floating point.
            str[i] = '\0';
            // Allocate "left" string with constant length "MAX_PRECISION".
            left = (char *) malloc(MAX_PRECISION);
            // Copy integer part of input string to left.
            strcpy(left, str);
            // Convert left part of float and express it with total number of digits for float.
            left = decimalToBin(left, n_bytes * 8);
            // Cut the unnecessary '0' bits in front.
            left = strchr(left, '1');
            // If left part of float is 0 makes left NULL so give it '0' bit instead.
            if(left == NULL){
                strcpy(left, "0");
            }
            // Get the number of bits used to represent left part and decrement it by 1
            // This gives number of bits needed to shift left for mantissa.
            E = strlen(left) - 1;
            // Allocate "exponent" string with needed number of bits according to given IEEE-like format.
            exponent = (char *) malloc(exponent_digits + 1);
            // Calculate "exponent" using formula "exp = 2^(b-1) - 1 + E"
            sprintf(exponent, "%ld", powerOfTwo(exponent_digits - 1) - 1 + E);
            // Convert "exponent" to binary equivalent.
            exponent = decimalToBin(exponent, exponent_digits);
            // Concatinate with result.
            strcat(result, exponent);

            // Calculate the fraction before it is shifted to be in mantissa format.
            // 2 extra characters here are reserved for "0.".
            fraction = (char *) malloc(strlen(right) + 3);
            // Create "fraction" in "0." + "right" format
            strcpy(fraction, "0.");
            strcat(fraction, right);
            // Convert to corresponding binary fraction.
            fraction = fractionToBin(fraction, strlen(right));
            // Concatinate left and fraction part making it in scientific format of Sign Exponent Fraction.
            // First bit of left is unneeded as it will be like 1.mantissa equivalent to 1.mantissa_bits
            strcat(++left, fraction);
            // Round resulting mantissa to nearest event as there maybe too much bits in mantissa.
            left = roundNearEven(left, n_bytes * 8 - exponent_digits - 1);
            // Concatinate mantissa with already existing parts of sign and exponent in result.
            strcat(result, left);

        }
        // Otherwise it must be a signed integer.
        else{
            // Convert directly to binary.
            result = decimalToBin(str, INT_SIZE * 8);
        }
    }
    return result;
}

// Convert given file "filename" to hexadecimals with float byte size "n_bytes" and write output to array of strings: "outputArray".
char **convertFile(char *filename, char **outputArray, int n_bytes){
    // Open file for reading.
    FILE *fptr = fopen(filename, "r");
    char line[MAX_LINE];
    int i;
    // Read each line of strings from file. fscanf will return -1 if the input is ended.
    for(i = 0; fscanf(fptr, "%s", line) != -1; i++){
        // Reallocate space for array of strings "outputArray".
        outputArray = (char **) realloc(outputArray, sizeof(char *) * (i+1));
        // Convert current string to binary then save it to "outputArray".
        outputArray[i] = convert(line, n_bytes);
        // Convert current binary string to hexadecimal and save it to "outputArray".
        outputArray[i] = binToHex(outputArray[i]);
    }
    return outputArray;
}

// Write given array of strings: "outputArray" to output file with name: "filename" according to endianness of 'B' for Big Endian or 'L' for Little Endian.
void writeOutputFile(char *filename, char **outputArray, char endianness){
    int i, j, len;
    // Open file for writing.
    FILE *fptr = fopen(filename, "w");
    // If the endianness is Big Endian.
    if(endianness == 'B'){
        // Write till current string in outputArray is not NULL.
        for(i=0; outputArray[i] != NULL; i++){
            // Get the number of hexadecimal digits to be written.
            len = strlen(outputArray[i]);
            // Go through each hexadecimals in given outputArray 2 by 2. Incrementing by 2 as the hexadecimal order is normal.
            for(j = 0; j < len; j+=2){
                // Write current digit and digit after.
                fprintf(fptr, "%c%c", outputArray[i][j], outputArray[i][j+1]);
                // Write spaces in between as long as it is not the last element of array.
                if(j != len-2)
                    fprintf(fptr, " ");
            }
            // Print newline for each line.
            fprintf(fptr, "\n");
        }
    }
    // If the endianness is Little Endian.
    if(endianness == 'L'){
        // Write till current string in outputArray is not NULL.
        for(i=0; outputArray[i] != NULL; i++){
            // Get the number of hexadecimal digits to be written.
            len = strlen(outputArray[i]);
            // Go through each hexadecimals in given outputArray 2 by 2. Decrementing by 2 as the hexadecimal order is reversed.
            for(j = len - 1; j >0;j-=2){
                // Write digit before and current digit.
                fprintf(fptr, "%c%c", outputArray[i][j-1], outputArray[i][j]);
                // Write spaces in between as long as it is not the last element of array.
                if(j != 1)
                    fprintf(fptr, " ");
            }
            // Print newline for each line.
            fprintf(fptr, "\n");
        }
    }
}
