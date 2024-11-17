/***************************************************************/
/* Cradle                                                        */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

/* Constant Declarations */
#define TAB '\t'

/* Variable Declarations */
char Look;      /* Lookahead Character */

void Term();
void Add();
void Subtract();
void Multiply();
void Divide();
void Expression();


/***************************************************************/
/* Read New Character From Input Stream */
void GetChar() {
    Look = getchar();
}

/* Report an Error */
void Error(const char* s) {
    printf("\n\007Error: %s.\n", s);
}

/* Report Error and Halt */
void Abort(const char* s) {
    Error(s);
    exit(1);
}

/* Report What Was Expected */
void Expected(const char* s) {
    char buf[100];
    sprintf(buf, "%s Expected", s);
    Abort(buf);
}

/* Match a Specific Input Character */
void Match(char x) {
    if (Look == x) {
        GetChar();
    } else {
        char buf[4];
        buf[0] = '\'';
        buf[1] = x;
        buf[2] = '\'';
        buf[3] = '\0';
        Expected(buf);
    }
}

/* Recognize an Alpha Character */
bool IsAlpha(char c) {
    return isalpha(c) != 0;
}

/* Recognize a Decimal Digit */
bool IsDigit(char c) {
    return isdigit(c) != 0;
}

/* Get an Identifier */
char GetName() {
    if (!IsAlpha(Look)) {
        Expected("Name");
    }
    char result = toupper(Look);
    GetChar();
    return result;
}

/* Get a Number */
char GetNum() {
    if (!IsDigit(Look)) {
        Expected("Integer");
    }
    char result = Look;
    GetChar();
    return result;
}

/* Output a String with Tab */
void Emit(const char* s) {
    printf("%c%s", TAB, s);
}

/* Output a String with Tab and CRLF */
void EmitLn(const char* s) {
    Emit(s);
    printf("\n");
}

/* Parse and Translate a Math Factor */
void Factor() {
    if (Look == '(') {
        Match('(');
        Expression();
        Match(')');
    } else {
        char num[20];
        sprintf(num, "MOVE #%c,D0", GetNum());
        EmitLn(num);
    } 
    /*
    char num[20];
    sprintf(num, "MOVE #%c,D0", GetNum());    
    EmitLn(num);*/
}

/* Recognize and Translate a Multiply */
void Multiply() {
    Match('*');
    Factor();
    EmitLn("MULS (SP)+,D0");
}

/* Recognize and Translate a Divide */
void Divide() {
    Match('/');
    Factor();
    EmitLn("MOVE (SP)+,D1");
    EmitLn("DIVS D1,D0");
}

/* Parse and Translate a Math Expression */
void Term() {
    Factor();

    while (Look == '*' || Look == '/') {
        EmitLn("MOVE D0,-(SP) ");
        
        switch(Look) {
            case '*':
                Multiply();
                break;
            case '/':
                Divide();
                break;
            default:
                Expected("Mulop");
        }   
    }    
}

/* Recognize and Translate an Add */
void Add() {
    Match('+');
    Term();
    EmitLn("ADD (SP)+,D0");
}

/* Recognize and Translate a Subtract */
void Subtract() {
    Match('-');
    Term();
    EmitLn("SUB (SP)+,D0");
    EmitLn("NEG D0");
}

bool IsAddop(char c) {
    return (c == '+' || c == '-');
}

/* Parse and Translate a Math Expression */
void Expression() {
    // Term();
    if (IsAddop(Look)){
        EmitLn("CLR D0");
    } else {
        Term();
    }
    while (IsAddop(Look)) {
        EmitLn("MOVE D0,-(SP)"); /* -(SP) = push, (SP)+ = pop M68SK motorola */

        switch(Look) {
            case '+':
                Add();
                break;
            case '-':
                Subtract();
                break;
            default:
                Expected("Addop");
        }
    }
}


/***************************************************************/
/* Initialize */
void Init() {
    GetChar();
}

/***************************************************************/
/* Main Program */
int main() {
    Init();
    Expression(); 
    return 0;
}
/***************************************************************/



