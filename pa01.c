/*=============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Vigenere cipher
|
| Author: Vraj Patel
| Language: c
|
| To Compile:gcc -o pa01 pa01.c
| 
| 
| 
|
| To Execute: ./pa01 kX.txt pX.txt
| 
| 
| 
| 
| 
| 
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Spring 2023
| Instructor: McAlpin
| Due Date: March 6, 2023
|
+=============================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


//Used to read in the encryption key data
//Function takes 2 parameter
//First Parameter: Name of key file
//Second Parameter: "&size", used to read and store size of the array
int** readKey(char* filename, int* size) {
    FILE* Key = fopen(filename, "r");
    if (Key == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    // Read the size of the matrix from the file
    fscanf(Key, "%d", size);

    // Allocate memory for the matrix
    int** matrix = (int**) malloc(*size * sizeof(int*));
    for (int i = 0; i < *size; i++) {
        matrix[i] = (int*) malloc(*size * sizeof(int));
    }

    // Read the matrix data from the file
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            fscanf(Key, "%d", &matrix[i][j]);
        }
    }

    // Close the file and return the matrix
    fclose(Key);
    
    return matrix;

    // Free memory allocated for the matrix
        for (int i = 0; i < *size; i++) {
            free(matrix[i]);
        }
        free(matrix);

    

}


//Used to format plaintext by eliminating any non alphabetical characters and making them all lowercase
//Function takes 2 paramaters
//First Parameter: Name of file to read from ex."p0.txt"
//Second Parameter: Name of string to write data to (Initialize the string beforehand (char string[10000 + 1]))
void readPlaintext(char *filename, char *formattedPlaintext) {
    #define MAX_LETTERS 10000
    FILE *plaintext;
    char c;
    int i = 0;
    
    plaintext = fopen(filename, "r");
    if (plaintext == NULL) {
        printf("File can not be opened");
        return;
    }
    
    while ((c = fgetc(plaintext)) != -1) {
        if (isalpha(c) != 0) {
            formattedPlaintext[i++] = tolower(c);
            if (i >= MAX_LETTERS) {
                break;
            }
        }
    }
    
    fclose(plaintext);
    formattedPlaintext[i] = '\0';  // add null terminator at the end of character array
}


//Encrypt the Plaintext using Hill Cipher and return encrypted string
char* encryption( int **matrixKey, int size, char *plaintext ){
    int n = strlen(plaintext);
    int num_blocks = ceil(n / (double)size);
    int padded_len = num_blocks * size;

    // Pad the plaintext with 'x' to make its length a multiple of "size"
    //then add a null terminator to the end of the string
    char* padded_plaintext = (char*) malloc((padded_len + 1) * sizeof(char));
    memset(padded_plaintext, 'x', padded_len);
    memcpy(padded_plaintext, plaintext, n);
    padded_plaintext[padded_len] = '\0';


    printf("Plaintext: \n");
    printf("%s\n", padded_plaintext);
    printf("\n");





    // Allocate memory for the ciphertext
    char* ciphertext = (char*) malloc((padded_len + 1) * sizeof(char));
    memset(ciphertext, 0, (padded_len + 1) * sizeof(char));


    // Encrypt each block of plaintext
    for (int i = 0; i < num_blocks; i++) {

        // Extract block of plaintext
        char* block = (char*) malloc((size + 1) * sizeof(char));
        memcpy(block, padded_plaintext + i * size, size);
        block[size] = '\0';

        // Matrix multiplication
        for (int j = 0; j < size; j++) {
            int sum = 0;
            for (int k = 0; k < size; k++) {
                sum += matrixKey[j][k] * (block[k] - 'a');
            }
            ciphertext[i * size + j] = (sum % 26) + 'a';
        }

        free(block);
    }

    free(padded_plaintext);
    return ciphertext;

}

    



int main() {

    int size;
    int** matrix = readKey("k1.txt", &size);

    // Print Key Matrix---------------------
    if (matrix != NULL) {
        printf("\n");
        printf("Key Matrix:\n");
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    //---------------------------------------
   

    // Format Plaintext to prepare for encryption
    char plaintext[10000 + 1];
    readPlaintext("p1.txt", plaintext);

    

    //--------------------------------------------

    char* ciphertext = encryption(matrix, size, plaintext);

    printf("Ciphertext:\n");
    printf(ciphertext);
    printf("\n");


        return 0;
}


/*=============================================================================
| I Vraj Patel (vr837263) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/