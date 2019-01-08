/*
 * Midiel Rodriguez
 * ID: xxxxxxx
 * Course: COP-4338
 * Assignment: 1
 *
 * This program sorts the lines of an input file( or from standard input)
 * and prints the sorted lines to the screen, or saves them to the output file.
 * 
 *
 * Disclaimer:
 * I hereby certify that this code is my work and my work alone and understand
 * the syllabus regarding plagiarized code.
 * Copyright 2018 Midiel
 */

/*
 * File:   bstsort.c
 * Author: Midiel Rodriguez
 *
 * Created on June 24, 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// It displays/prints the proper usage to run the program.
static void print_usage() {
    printf("Usage: bstsort [-c] [-o output_file_name] [input_file_name]\n");
}

// Displays a memory error message
static void memoryError() {
    printf("Error. Allocation of memory was unsuccessful.\n");
}

// Node struct
typedef struct node_struct {
    char* string;                                  
    int count;                    
    struct node_struct* left;
    struct node_struct* right;      
} Node;

/**
 * Creates nodes, it accepts a string
 * @param string the string to add to the node
 * @return a node
 */
Node* createNode(char* string) { 
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->string = (char*)malloc(strlen(string) * sizeof(char));
    newNode->string = strdup(string);
    newNode->count = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/**
 * It compares character by character case insensitive
 * @param str1 string to compare
 * @param str2 string to compare
 * @return 1 if str1 is bigger, 2 if str2 is bigger, 0 if they are equal
 */
static int compare(char* str1, char* str2) {
    int length = 0;
    int i;
    if(strlen(str1) < strlen(str2)) {
        length = strlen(str2);
    } else {
        length = strlen(str1);
    }

    char a[100];
    char b[100];
    
    strcpy(a, str1);
    strcpy(b, str2);

    for (i = 0; i < length - 1; i++) {  
        if((toupper(a[i])) > (toupper(b[i]))) {
            return 1;
        } else if((toupper(a[i])) < (toupper(b[i]))) {
            return 2;
        }
    }
    return 0;
}

/**
 * It compares character by character case sensitive
 * @param str1 string to compare
 * @param str2 string to compare
 * @return 1 if str1 is bigger, 2 if str2 is bigger, 0 if they are equal
 */
static int compareCS(char* str1, char* str2) {
    int length = 0;
    int i;
    if(strlen(str1) > strlen(str2)) {
        length = strlen(str2);
    } else {
        length = strlen(str1);
    }
    
    char a[100];
    char b[100];
    
    strcpy(a, str1);
    strcpy(b, str2);
    
    for (i = 0; i < length - 1; i++) {  
        if((a[i]) < (b[i])) {
            return 1;
        } else if((a[i]) < (b[i])) {
            return 2;
        }
    }
    return 0;
}

/**
 * It inserts node recursively depending on the comparison of the string
 * by case insensitive.
 * 
 * @param tree the tree to add the node to
 * @param string the string of the new node
 * @return a new node
 */
Node* insert(Node* tree, char* string) {

    if (tree == NULL) {
        return createNode(string);
    }
       
    if (compare(string, tree->string) == 2) {
        tree->left = insert(tree->left, string);
    } else if(compare(string, tree->string) == 1){
        tree->right = insert(tree->right, string);
    } else if(compare(string, tree->string) == 0) {
        tree->count++;
        return tree;
    }
    return tree;
}

/**
 * It inserts node recursively depending on the comparison of the string
 * by case sensitive.
 * 
 * @param tree the tree to add the node to
 * @param string the string of the new node
 * @return a new node
 */
Node* insertCS(Node* tree, char* string) {
   
    if (tree == NULL) {
        return createNode(string);
    }
       
    if (compareCS(string, tree->string) == 2) {
        tree->left = insert(tree->left, string);
    } else if(compareCS(string, tree->string) == 1){
        tree->right = insert(tree->right, string);
    } else if(compareCS(string, tree->string) == 0) {
        tree->count++;
        return tree;
    }
           
    return tree;
}

/**
 * It displays the entire tree "inorder" traversal
 * @param tree the tree to print/display
 */

static void printInOrder(Node* tree) {
    if (tree == NULL) {
        return;
    }
    printInOrder(tree->left);
    printf("%d %s", tree->count, tree->string);
    free(tree->string);
    printInOrder(tree->right);
}


/**
 * It prints the entire tree to the file "inorder" traversal
 * @param OutFile the file to print it to
 * @param tree the tree to print/save
 */
static void saveToFile(FILE* outFile, Node *tree) {
    
    if (tree) {
        saveToFile(outFile, tree->left);
        fprintf(outFile, "%d %s\n", tree->count, tree->string);
        free(tree->string);
        saveToFile(outFile, tree->right);
    }
}

/**
 * Gets a list of lines either from a text file or from keyboard and sorts them
 * by character. The either prints it to display or saves it to a text file.
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv) {
    
    // To hold the flags and variables
    extern char *optarg;
    extern int optind;
    int c;
    int cFlag = 0, oFlag = 0;
    
    // To hold the tree and file pointers
    Node* tree = NULL;
    FILE* inFile = NULL;
    FILE* outFile = NULL;
       

    /* It iterates through the array of characters using "getopt" to parse the
     * command line.
     */ 
    
    char* oName;                // pointer for name of file
    
    while ((c = getopt(argc, argv, "co:")) != -1){
        switch (c) {
            case 'c':                                   // sets cFlag
                cFlag = 1;
                break;        
            case 'o':                                   // sets oFlag
                oFlag = 1;
                oName = (char*)malloc(strlen(optarg) * sizeof(char));
                oName = optarg; 
                break;
            case '?':
                printf("Error: Invalid format.\n");
                print_usage();
                exit (0);                    
        }
    }

    // open the output file if option selected it
    if(oFlag) {
        outFile = fopen(oName, "w");
        
        // Validate that file did open
        if(outFile == NULL) {
            printf("Could not open output file.");
            print_usage();
            return -1;
        }
    }
 
    // open the input file if user selected it, and gets the lines from it.
    if((argv[optind])) {
        // Try to open the file in read mode
        inFile = fopen(argv[optind], "r");

        // Validate that file did open
        if(inFile == NULL) {
            printf("Could not open file %s.\n", argv[optind]);
            print_usage();
            return -1;
        }
        
        char tempLine[100];
        if(inFile) {
            while(fgets(tempLine, 100, inFile) != NULL && (tempLine[0] != '\n') && (tempLine[0] != '\r')) {
                if(cFlag) {
                    tree = insertCS(tree, tempLine);
                } else {
                    tree = insert(tree, tempLine);
                }  
            }
            fclose(inFile);
        }
        
    // if no file option was selected, then get the lines from the keyboard
    } else {
        char tempLine[100];
        printf("Plase enter your strings followed by ENTER\n");
        
        // get the lines and insert them into the tree
        while((fgets(tempLine, 100, stdin) != NULL) && (tempLine[0] != '\n') && (tempLine[0] != '\r')) {
                if(cFlag) {
                    tree = insertCS(tree, tempLine);
                } else {
                    tree = insert(tree, tempLine);
                }          
        }
    }

    // if oFlag selected, then print the tree to the file
    if(oFlag) {
        saveToFile(outFile, tree);
        fclose(outFile);
    } else {
        // print to console
        printf("\nLines in order.\n");
        printInOrder(tree);
    }

    // free the tree memory
    free(tree);
    
    return 0;
}

