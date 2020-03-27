#include <stdio.h>
#include <ctype.h>

void addChar();
void getChar();
void getNonBlank();
int lex();
void factor();
void term();
void expr();
void stmt();

char nextChar;
char lexeme[100];
int lexLen, charClass, token, nextToken;

FILE *in_fp, *fopen();

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int lookup(char ch) {
  switch (ch) {
    case '(':
      addChar();
      nextToken = LEFT_PAREN;
      break;
    case ')':
      addChar();
      nextToken = RIGHT_PAREN;
      break;
    case '+':
      addChar();
      nextToken = ADD_OP;
      break;
    case '-':
      addChar();
      nextToken = SUB_OP;
      break;
    case '*':
      addChar();
      nextToken = MULT_OP;
      break;
    case '/':
      addChar();
      nextToken = DIV_OP;
      break;
    default:
      addChar();
      nextToken = EOF;
      break;
  }
  return nextToken;
}

void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  } else {
    printf("Error - lexeme is too long \n");
  }
}

void getChar() {
  if ((nextChar = getc(in_fp)) != EOF) {
    if (isalpha(nextChar)){
      charClass = LETTER;
    } else if (isdigit(nextChar)){
      charClass = DIGIT;
    } else {
      charClass = UNKNOWN;
    }
  } else {
    charClass = EOF;
  }
}

void getNonBlank() {
  while (isspace(nextChar)){
    getChar();
  }
}

int lex(){
  lexLen = 0;
  getNonBlank();
  switch(charClass){
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT) {
        addChar();
        getChar();
      }
      nextToken = IDENT;
      break;

    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT) {
        addChar();
        getChar();
      }
      nextToken = INT_LIT;
      break;

    case UNKNOWN:
      lookup(nextChar);
      getChar();
      break;

    case EOF:
      nextToken = EOF;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      break;
  }
  printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
  return nextToken;
}

void factor() {
  printf("Enter <factor>\n");

  if (nextToken == IDENT){
    lex();
    if(nextToken == ASSIGN_OP){
      stmt();
    }
  } else if (nextToken == INT_LIT){
    lex();
  } else {
    if (nextToken == LEFT_PAREN) {
      lex();
      expr();
      if (nextToken == RIGHT_PAREN){
        lex();
      } else {
        error();
      }
    } else {
      error();
    }
  }
  printf("Exit <factor>\n");;
}

void term() {
  printf("Enter <term>\n");

  factor();

  while (nextToken == MULT_OP || nextToken == DIV_OP) {
    lex();
    factor();
  }
  printf("Exit <term>\n");
}

void expr() {
  printf("Enter <expr>\n");

  term();

  while (nextToken == ADD_OP || nextToken == SUB_OP) {
    lex();
    term();
  }
  printf("Exit <expr>\n");
}

void stmt(){
    printf("Enter <stmt>\n");
    lex();
    expr();
    printf("Exit <stmt>\n");
}

int main(){
  if ((in_fp = fopen("front.in", "r")) == NULL){
    printf("ERROR - cannot open front.in \n");
  } else {
    getChar();
    do {
      lex();
      expr();
    } while (nextToken != EOF);
  }
}