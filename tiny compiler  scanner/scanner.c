#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0

#define MAXRESERVED 8
#define MAXTOKENLEN 40
#define BUFLEN 256
typedef enum
{
    ENDFILE,
    ERROR,
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    ID,
    NUM,
    ASSIGN,
    EQ,
    LT,
    PLUS,
    MINUS,
    TIMES,
    OVER,
    LPAREN,
    RPAREN,
    SEMI
} TokenType;

typedef enum
{
    START,
    INASSIGN,
    INCOMMENT,
    INNUM,
    INID,
    DONE
} StateType;

FILE *source;
FILE *listing;
FILE *code;

int lineno = 0;
int EchoSource = TRUE;
static int EOF_flag = FALSE;
int TraceScan = TRUE;

TokenType getToken(void);

char tokenString[MAXTOKENLEN + 1];
static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;
static struct
{
    char *str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {{"if", IF}, {"then", THEN}, {"else", ELSE}, {"end", END}, {"repeat", REPEAT}, {"until", UNTIL}, {"read", READ}, {"write", WRITE}};

static int getNextChar(void)
{
    if (!(linepos < bufsize))
    {
        lineno++;
        if (fgets(lineBuf, BUFLEN - 1, source))
        {
            if (EchoSource)
                fprintf(listing, "%4d: %s", lineno, lineBuf);
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        }
        else
        {
            EOF_flag = TRUE;
            return EOF;
        }
    }
    else
        return lineBuf[linepos++];
}
static void ungetNextChar(void)
{
    linepos--;
}
static TokenType reservedLookup(char *s)
{
    int i;
    for (i = 0; i < MAXRESERVED; i++)
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}

void printToken(TokenType token, const char *tokenString)
{
    switch (token)
    {
    case IF:
    case THEN:
    case ELSE:
    case END:
    case REPEAT:
    case UNTIL:
    case READ:
    case WRITE:
        fprintf(listing,
                "reserved word: %s\n", tokenString);
        break;
    case ASSIGN:
        fprintf(listing, ":=\n");
        break;
    case LT:
        fprintf(listing, "<\n");
        break;
    case EQ:
        fprintf(listing, "=\n");
        break;
    case LPAREN:
        fprintf(listing, "(\n");
        break;
    case RPAREN:
        fprintf(listing, ")\n");
        break;
    case SEMI:
        fprintf(listing, ";\n");
        break;
    case PLUS:
        fprintf(listing, "+\n");
        break;
    case MINUS:
        fprintf(listing, "-\n");
        break;
    case TIMES:
        fprintf(listing, "*\n");
        break;
    case OVER:
        fprintf(listing, "/\n");
        break;
    case ENDFILE:
        fprintf(listing, "EOF\n");
        break;
    case NUM:
        fprintf(listing,
                "NUM, val= %s\n", tokenString);
        break;
    case ID:
        fprintf(listing,
                "ID, name= %s\n", tokenString);
        break;
    case ERROR:
        fprintf(listing,
                "ERROR: %s\n", tokenString);
        break;
    default: 
        fprintf(listing, "Unknown token: %d\n", token);
    }
}

void main()
{
    char pgm[20]="SAMPLE.TNY";
    source = fopen(pgm, "r");
    if (source == NULL)
    {
        fprintf(stderr, "File %s not found\n", pgm);
        exit(1);
    }

    listing = stdout;
    fprintf(listing, "\nTINY COMPILATION: %s\n", pgm);
    while (getToken() != ENDFILE);
}

TokenType getToken(void)
{ 
    int tokenStringIndex = 0;
    TokenType currentToken;
    StateType state = START;
    int save;
    while (state != DONE)
    {
        int c = getNextChar();
        save = TRUE;
        switch (state)
        {
        case START:
            if (isdigit(c))
                state = INNUM;
            else if (isalpha(c))
                state = INID;
            else if (c == ':')
                state = INASSIGN;
            else if ((c == ' ') || (c == '\t') || (c == '\n'))
                save = FALSE;
            else if (c == '{')
            {
                save = FALSE;
                state = INCOMMENT;
            }
            else
            {
                state = DONE;
                switch (c)
                {
                case EOF:
                    save = FALSE;
                    currentToken = ENDFILE;
                    break;
                case '=':
                    currentToken = EQ;
                    break;
                case '<':
                    currentToken = LT;
                    break;
                case '+':
                    currentToken = PLUS;
                    break;
                case '-':
                    currentToken = MINUS;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case '/':
                    currentToken = OVER;
                    break;
                case '(':
                    currentToken = LPAREN;
                    break;
                case ')':
                    currentToken = RPAREN;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                default:
                    currentToken = ERROR;
                    break;
                }
            }
            break;
        case INCOMMENT:
            save = FALSE;
            if (c == EOF)
            {
                state = DONE;
                currentToken = ENDFILE;
            }
            else if (c == '}')
                state = START;
            break;
        case INASSIGN:
            state = DONE;
            if (c == '=')
                currentToken = ASSIGN;
            else
            {
                ungetNextChar();
                save = FALSE;
                currentToken = ERROR;
            }
            break;
        case INNUM:
            if (!isdigit(c))
            { 
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;
        case INID:
            if (!isalpha(c))
            { 
                ungetNextChar();
                save = FALSE;
                state = DONE;
                currentToken = ID;
            }
            break;
        case DONE:
        default: 
            fprintf(listing, "Scanner Bug: state= %d\n", state);
            state = DONE;
            currentToken = ERROR;
            break;
        }
        if ((save) && (tokenStringIndex <= MAXTOKENLEN))
            tokenString[tokenStringIndex++] = (char)c;
        if (state == DONE)
        {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }
    if (TraceScan)
    {
        fprintf(listing, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken;
} 
