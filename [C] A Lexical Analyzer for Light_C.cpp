#include <iostream>
#define BAD -1
#define GOOD -2
#define ALMOST_GOOD -3 //state awaiting only \0 or \n
#define BADEXIT -4
using namespace std;

/*
Author: Georgopoulos Anastasios
Date:   4/11/13

The program emulates a lexical analyzer, in C programming language.
At first, it scans the first letter of the user-input string, and it
calls the appropriate function for the job (variables, comment, number, strings).
Each function, built by the template of its DFA (http://en.wikipedia.org/wiki/DFSA),
then checks the string, and outputs the validity and charlength of the input.
If there is a text file named 'input.exe' on its directory, the program will also
scan the file's contents as lines, outputing the information (type and size) to a
new file named 'output.exe'.
f.e input: 6.478932 \n //com// \n "etc." | output: NUMBER 8 \n COMMENT 3 \n STRING 4

Compiled with DEV-C++ {mingw32-gcc-3.4.2} on a Windows 7 x64 system.
For TEI of Athens and Mrs. Georgouli, a project for Compilers class.
©Anastasios Georgopoulos, Athens 2013
*/

int variables(char* a);
int number(char* a);
int str(char* a);
int comment(char* a);

int main() {
    char a[256]; //the char* to hold the user input {and possibly line input from file}
    int res;
    //////////////////////////////////////ONLY FILE PROCESSES/////////////////////////////////////////////
    //////// If there is no input.txt in the executable's directory, this part will be skipped////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    FILE *in=fopen("input.txt","r");
    if (in!=NULL) {
        FILE *fp=fopen("output.txt","w");
        while (fgets(a, sizeof(a), in)) {
              if ( (a[0] >= 'A' && a[0] <= 'Z') || (a[0] >= 'a' && a[0] <= 'z') ) {
              res = variables(a); if (res!=BADEXIT) fprintf(fp,"VARIABLE %d\n",res); else fprintf(fp,"Invalid Variable Name.\n");
              } else if ( a[0] >= '0' && a[0] <= '9' ) {
                res = number(a); if (res!=BADEXIT) fprintf(fp,"NUMBER %d\n",res); else fprintf(fp,"Invalid Number.\n");
                } else if ( a[0] == '\"') {
                  res = str(a); if (res!=BADEXIT) fprintf(fp,"STRING %d\n",res); else fprintf(fp,"Invalid String.\n");
                  } else if ( a[0] == '/') {
                    res = comment(a); if (res!=BADEXIT) fprintf(fp,"COMMENT %d\n",res); else fprintf(fp,"Invalid Comment.\n");
                    } else fprintf(fp,"Unknown character. Skipping line.");
        }
        fclose(in);
        fclose(fp);
        printf("Finished file operations. File 'output.txt' was created.\n");
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////Main part of the program. It will analyze every user's input, awaiting for the next one.////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////// 
    printf("Entering while loop. You may enter your input. Escape by pressing CTRL+C.\n");
    while(1) {
                 cin.getline(a,255); // fun fact! I spent 2 hours of my life just to figure out why 'scanf' was a bad idea.
                 if ( (a[0] >= 'A' && a[0] <= 'Z') || (a[0] >= 'a' && a[0] <= 'z') ) {
                      res = variables(a);
                      if (res != BADEXIT) {printf("It's a valid VARIABLE with a charlength of %d\n",res);}
                      else {printf("That's not a valid variable.\n");};
                      }
                 else if ( a[0] >= '0' && a[0] <= '9' ) {
                       res = number(a);
                       if (res != BADEXIT) {printf("It's a valid NUMBER with a charlength of %d\n",res);}
                       else {printf("That's not a valid number.\n");};
                       }
                       
                 else if ( a[0] == '\"') {
                       res = str(a);
                       if (res != BADEXIT) {printf("It's a valid STRING with a charlength of %d\n",res);}
                       else {printf("That's not a valid string.\n");};
                       }
                 else if ( a[0] == '/') {
                       res = comment(a);
                       if (res != BADEXIT) {printf("It's a valid COMMENT with a charlength of %d\n",res);}
                       else {printf("That's not a valid comment.\n");};
                       }
                 else printf("Unknown Character at the start of the string.\n");
             }
    return 0;
};

// A decision was made, to have the functions scan the string from the start, and not the second letter.
// The thought behind this is that we could use these pieces of code in other applications afterwards.

int number(char* a) {
    // It recieves the input (which is (supposed to be a) number),
    // and returns BADEXIT for unvalid 'number', or the charlength for a legitimate one.
    int curr = /*S*/0; //every 'magic numeral' is actually a state of the DFA. 
    int counter = 0;
    while (curr != GOOD && curr != BAD) {
        switch ( curr ) {
               case 0: if ( a[counter] >= '1' && a[counter] <= '9' ) {curr=4;counter++;}
                       else if ( a[counter] == '0' ) {curr=5; counter++;}
                       else curr = BAD;
                       break;
               case 4: if ( a[counter] >= '1' && a[counter] <= '9' ) counter++;
                       else if ( a[counter] == '.' ) {curr=6; counter++;}
                       else if ( a[counter] == '\0' || a[counter] == '\n' ) curr = GOOD;
                       else curr = BAD;
                       break;
               case 5: if ( a[counter] == '\0' || a[counter] == '\n' ) curr = GOOD;
                       else if ( a[counter] == '.' ) {curr=6; counter++;}
                       else curr = BAD;
                       break;
               case 6: if ( a[counter] >= '0' && a[counter] <= '9' ) {curr=7;counter++;}
                       else curr = BAD;
                       break;
               case 7: if ( a[counter] >= '0' && a[counter] <= '9' ) counter++;
                       else if ( a[counter] == '\0' || a[counter] == '\n' ) curr = GOOD;
                       else curr = BAD;
                       break;                   
               };
        };
    if (curr == BAD) return BADEXIT;
    else return counter; 
    };

int variables(char* a) {
    // It recieves the input (which is (supposed to be a) variable name),
    // and returns BADEXIT for unvalid 'variable', or the charlength for a legitimate one.
    int curr = /*S*/0;
    int counter = 0;
    while (curr != GOOD && curr != BAD) {
          switch ( curr ) {
                 case 0: if ( (a[counter] >= 'A' && a[counter] <= 'Z') || (a[counter] >= 'a' && a[counter] <= 'z') ) {curr=1;counter++;}
                         else curr = BAD;
                         break;
                 case 1: if ( (a[counter] >= 'A' && a[counter] <= 'Z') || (a[counter] >= 'a' && a[counter] <= 'z') || (a[counter] >= '0' && a[counter] <= '9') ) counter++;
                         else if ( a[counter] == '\0' || a[counter] == '\n' ) curr = GOOD;
                         else if ( a[counter] == '_' ) {curr=2;counter++;}
                         else curr = BAD;
                         break;
                 case 2: if ( (a[counter] >= 'A' && a[counter] <= 'Z') || (a[counter] >= 'a' && a[counter] <= 'z') || (a[counter] >= '0' && a[counter] <= '9') ) {curr=3;counter++;}
                         else curr = BAD;
                         break;
                 case 3: if ( (a[counter] >= 'A' && a[counter] <= 'Z') || (a[counter] >= 'a' && a[counter] <= 'z') || (a[counter] >= '0' && a[counter] <= '9') ) counter++;
                         else if ( a[counter] == '\0' || a[counter] == '\n' ) curr = GOOD;
                         else curr = BAD;
                         break;
                         };
                 };
    if (curr == BAD) return BADEXIT;
    else return counter;
    };

int str(char* a) {
    // It recieves the input (which is (supposed to be a) string),
    // and returns BADEXIT for unvalid 'string', or the charlength for a legitimate one.
    int curr = /*S*/0;
    int counter = 0;
    while (curr != GOOD && curr != BAD) {
          switch ( curr ) {
                 case 0: if ( a[counter] == '\"' ) {curr=8;counter++;}
                         else curr = BAD;
                         break;
                 case 8: if ( a[counter] == ' ' || a[counter] == '!' || (a[counter] >= '#' && a[counter] <= '[') || (a[counter] >= ']' && a[counter] <= '~') ) {counter++;}
                         else if ( a[counter] == '\\' ) {curr=9;counter++;}
                         else if ( a[counter] == '\"' ) {curr=ALMOST_GOOD;counter++;}
                         else curr = BAD;
                         break;
                 case 9:if ( (a[counter] >= '!' && a[counter] <= '~') || a[counter] == ' ' ) {curr=8;counter++;}
                         else curr = BAD;
                         break;
                 case ALMOST_GOOD: if ( a[counter] == '\0' || a[counter] == '\n' ) curr = GOOD; else curr = BAD;
                         break;
                 };
          };
    if (curr == BAD) return BADEXIT;
    else return counter-2;
    };

int comment(char* a) {
    // It recieves the input (which is (supposed to be a) comment),
    // and returns BADEXIT for unvalid 'comment', or the charlength for a legitimate one.
    int curr = /*S*/0;
    int counter = 0;
    while (curr != GOOD && curr != BAD) {
          switch ( curr ) {
                 case 0: if ( a[counter] == '/' ) {curr=10;counter++;}
                         else curr = BAD;
                         break;
                 case 10:if ( a[counter] == '/' ) {curr=11;counter++;}
                         else curr = BAD;
                         break;
                 case 11:if ( a[counter] == '/' ) {curr=12;counter++;}
                         else if ( a[counter] == '\0' || a[counter] == '\n' ) curr = BAD;
                         else counter++;
                         break;
                 case 12:if ( a[counter] == '/' ) {curr=ALMOST_GOOD;counter++;}
                         else if ( a[counter] == '\0' || a[counter] == '\n' ) curr = BAD;
                         else {curr=11;counter++;}
                         break;
                 case ALMOST_GOOD: if ( a[counter] == '\0' || a[counter] == '\n' ) curr=GOOD; else curr = BAD;
                         break;
                 };
          };
    if (curr == BAD) return BADEXIT;
    else return counter-4;
    };
