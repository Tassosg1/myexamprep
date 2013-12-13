%option noyywrap

 /*
	Author: Georgopoulos Anastasios
	Date: 1/12/13

	This .l file is built to be an input file for the FLEX (Fast LEXical analyzer) tool.
	In short, the result we want is to instruct FLEX to perform pattern-matching on text,
	giving it the REGular EXpressions to do so. The result is an outline of the source code,
	with each piece identified correctly. The syntax style is C-like, customed to FLEX's requirements.
	Now including every symbol or reserved word there is in Light-C! (REV2.0 9/12/13)
	More about the proper format of a FLEX input file can be found on http://flex.sourceforge.net/manual/

	Version 1.0 was compiled with the last version of FLEX on the Ubuntu repository as of 1st Dec 2013,
	and the result C file was complied by gcc tool, on a Ubuntu 12.03 x64 system.
	Version 2.0 was compiled with the last version of FLEX on the Ubuntu repository as of 9th Dec 2013,
	and the result C file was complied by gcc tool, on a Ubuntu 12.10 x64 system.

	For TEI of Athens and Mrs. Georgouli, a project for Compilers class. ?©Anastasios Georgopoulos, Athens 2013
 */

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
void tprint(int token,char *lexema);
int check(char *lexema,int choice);
int line=1;
char stringtobeprinted[256];
 /* That char* will hold any STRING that is currently scanned (see below). */
%}
%x STRING BACKSLASHEDSTRING
  /* That's a list of exclusive START conditions; That is, conditions that can only be met by explicitly
  /* BEGIN()'ing them. STRING will be used when an " is in the input; and BACKSLASHEDSTRING when a \ is met in the STRING. */
BLANK [ \s\t]+
 /* Used to eliminate blanks on sight, an elegant way to get rid of blanks and tabs, delimiters for Light-C. */
IDENTIFIER [A-Za-z][A-Za-z0-9]*(['_'][A-Za-z0-9]+)?
 /* A legit identifier (with one underscore)? */
INTCONST [1-9][0-9]*|[0]
REALCONST {INTCONST}[\.][0-9]+
 /* End of legitimate rules. Now for the (E)rror identification. */
E_IDUNDERUNDER [A-Za-z][A-Za-z0-9]*(['_']+[A-Za-z0-9]*){2,}
 /* The identifier contains two or more underscores. */
E_ZERODIGIT [0]+({INTCONST}|{REALCONST})
 /* In compliance with Light-C, an elegant way to eliminate zeros on the start of a num constant. F.e. 01, 023.0, but not 0.0 . */
E_DIGITID [0-9]+{IDENTIFIER}((['_']+{IDENTIFIER})+)?
 /* An ID that starts with a number is an invalid ID. */
E_DIGITMISC  0*{INTCONST}[A-Za-z_]|0*{REALCONST}[A-Za-z_]|{INTCONST}[\.]+
 /* The NUMCONST is followed by one or more illegal characters (letters), nor regognisable by NUMCONST (not a dot or a number). */
%%

{INTCONST}  { tprint (INTCONST,yytext);}
{BLANK} {}
 /* Q: What do we do when we meet a blank or a tab? A: Absolutely Nothing! */
{E_DIGITID} 	{
					printf("***ERROR in line %d: Illegal identifier (Initial character is Digit): %s\n",line,yytext);
 /* 'line' and 'yytext' are global variables, containing the line number and the read word respectively. */
					}
{E_ZERODIGIT} 	{	
					printf("***ERROR in line %d: Illegal digit %s (initial digit is zero)\n",line,yytext);
					}
{E_DIGITMISC} {
					printf("***ERROR in line %d: Illegal digit %s \n",line,yytext);
					}
{E_IDUNDERUNDER}	{	if(yyleng>30)
	   						printf("***ERROR %d: Identifier \'%s\' length more than 30 characters\n",line,yytext);
						else
	  						printf("***ERROR line %d: Illegal identifier %s, more than one '_'\n",line,yytext);
	  					/* The priority of the errors is: 1st) Starts with num, 2nd) Length, 3rd) Undescores */ 
}
{REALCONST} {		tprint (REALCONST,yytext);}
{IDENTIFIER}  	{
				if(yyleng>30)
	  			 	printf("***ERROR %d: Identifier \'%s\' length more than 30 characters\n",line,yytext);
				else
					tprint (check(yytext,0),yytext);
 /* Here, we check if the identifier is a reserved word, by using the check() method with flag 0 (word). (see below) */
				}

"+"|"-"|"*"|"/"|"<"|"<="|"=="|">"|">="|"<>"|","|";"|"("|")"|"["|"]"|"=" {tprint(check(yytext,1),yytext);}
 /* These are symbols with special meaning, so we pass the identification to check() method, with flag 1 (symbol) */

"//"  	{
	   register int c;
	   register int initline = line;
 /* Used in case of error, so the message will print the line the comment started */
	   for(;;)
	    {
	     while((c=input())!='/'&&c!=EOF)
         		  if(c=='\n')line++;

		 if(c=='/'&&(c=input())=='/') break;
        		  if(c=='\n')line++;
                 	 if(c==EOF)
             {
              printf("***ERROR in line %d: Unterminated comment at EOF\n",initline); 
             break;
             }
          }
 /* We scan the source code scanning for comment ending, changing lines appropriately. */
       }
"\n" {line++;}

"\"" {strcpy(stringtobeprinted,""); BEGIN(STRING);}
 /* A quote is a START condition for the STRING set of rules. */

 /* stringtobeprinted: It is the char* to which the STRING to be printed is built. */

<STRING>{
	[^\\\"\n] {strcat(stringtobeprinted,yytext);} 
 /* Any character you meet (except for quotes and backslashes), copy it as-it-is on the stringtobeprinted. */
	\" 		{printf("STRCONST %s\n",stringtobeprinted);
			BEGIN(0);}
 /* If you meet a quote at this state, it's a legit strconst, and it's printed accordingly. From now on, normal rules apply. */

	\n 		{printf("***ERROR line %d: Unterminated string \"%s \n",line,stringtobeprinted);
			line++;
			BEGIN(0);}
 /* If you meet a end-of-line though, the string is not terminated correctly, and an error is produced. */

	\\ 		{BEGIN(BACKSLASHEDSTRING);}
 /* If you meet a backslash, go to the BACKSLASHEDSTRING state, from which the next character will be read. */
	}

<BACKSLASHEDSTRING>{
	"n" 	{strcat(stringtobeprinted,"\n");BEGIN(STRING);}
 /* If there is an 'n' after the backslash, add a \n character to the STRING. It will also affect the final printing. */
 /* Note that, in here, we can quickly and effectively give special meaning to backslashed characters. Next line is an example. */
	"t" 	{strcat(stringtobeprinted,"\t");BEGIN(STRING);}
	. 		{strcat(stringtobeprinted,yytext);BEGIN(STRING);}
 /* If there is any other character after the backslash, add it as-it-is to the STRING. */
 
 /* After all that, return to the STRING rules, to continue parsing of the String. */
	}

. {printf("Unidentified Symbol %s at line %d\n",yytext,line);}
%%

char *array[28]={"main","char","int","real","void","begin","end","if","else","return","+","-","*","/","<","<=","==",">",">=","<>",",",";","(",")","[","]","="};
char *tname[32]={"SMAIN","SCHAR","SINT","SREAL","SVOID","SBEGIN","SEND","SIF","SELSE","SRETURN", "PLUS","MINUS","MULTIPLY","DIVIDE","LS","LEQ","EQ","GT","GEQ","NEQ","COMMA","SEMI","OPENPAR","CLOSEPAR","OPENBRAC","CLOSEBRAC","ASSIGNOP", "IDENTIFIER","STRCONST", "INTCONST","REALCONST"};

 /* Definitions of Light_C's special words and symbols, to be used by the check() method. */

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
//   void tprint(int token,char *lexema);
//Ektyponei onoma token,lexema

void tprint(int token,char *lexema)
{
 printf("%s  %s\n",tname[token],lexema);
}

/*------------------------------------------------------------------------*/
//  int check(char *lexema,int choice);
//Elengxei an to lexema pou diabasame einai leksi kleidi episterfei to token
//IDENTIFIER an den einai allios ton arithmo tou antistoixou token,
//eno gia ta operators aplos epistrefei ton arithmo tou antistoixou token.

int check(char *lexema,int choice)
{
int i;
if(choice==0)
	{
	for(i=0;i<10;i++)
		{
		if (strcmp(lexema,array[i])==0)
			switch(i)
				  {
				  case(0): return(SMAIN);
            	  case(1): return(SCHAR);
            	  case(2): return(SINT);
            	  case(3): return(SREAL);
            	  case(4): return(SVOID);
            	  case(5): return(SBEGIN);
            	  case(6): return(SEND);
            	  case(7): return(SIF);
            	  case(8): return(SELSE);
            	  case(9): return(SRETURN);
				  }
		}
	 return(IDENTIFIER);
	}
else
	for(i=10;i<27;i++)
		{
		if(strcmp(lexema,array[i])==0)
			switch(i)
				  {
				  case(10): return(PLUS);
				  case(11): return(MINUS);
				  case(12): return(MULTIPLY);
				  case(13): return(DIVIDE);
				  case(14): return(LS);
				  case(15): return(LEQ);
				  case(16): return(EQ);
				  case(17): return(GT);
				  case(18): return(GEQ);
				  case(19): return(NEQ);
				  case(20): return(COMMA);
				  case(21): return(SEMI);
				  case(22): return(OPENPAR);
				  case(23): return(CLOSEPAR);
				  case(24): return(OPENBRAC);
				  case(25): return(CLOSEBRAC);
				  case(26): return(ASSIGNOP);
				  }
			 }
}

main(int argc,char **argv)
{
  ++argv,--argc;
  if(argc>0)yyin=fopen(argv[0],"r");
  	else yyin=stdin;
  yylex();
}