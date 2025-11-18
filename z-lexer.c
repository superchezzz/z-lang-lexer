#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//TOKEN DEF
typedef enum {
    TOKEN_TYPE_NULL = -1,

    //IDENTIFIERS
    TOKEN_IDENTIFIER,
    TOKEN_COMMENT,

    //OPERATORS
    TOKEN_OP_PLUS,
    TOKEN_OP_MINUS,
    TOKEN_OP_MULTIPLY,
    TOKEN_OP_DIVIDE,
    TOKEN_OP_MODULO,
    TOKEN_OP_ASSIGN,
    TOKEN_OP_PLUS_ASSIGN,
    TOKEN_OP_MINUS_ASSIGN,
    TOKEN_OP_MULTIPLY_ASSIGN, 
    TOKEN_OP_DIVIDE_ASSIGN,
    TOKEN_OP_MOD_ASSIGN,
    TOKEN_OP_XOR_ASSIGN,
    TOKEN_OP_AND_ASSIGN,
    TOKEN_OP_OR_ASSIGN,
    TOKEN_OP_LESS,
    TOKEN_OP_GREATER,
    TOKEN_OP_LESS_EQUAL,
    TOKEN_OP_GREATER_EQUAL,
    TOKEN_OP_EQUAL,
    TOKEN_OP_NOT_EQUAL,
    TOKEN_OP_NOT,
    TOKEN_OP_AND,
    TOKEN_OP_OR,
    TOKEN_OP_INCREMENT,
    TOKEN_OP_DECREMENT,

    //KEYWORDS
    TOKEN_KEYWORD_ALIAS,
    TOKEN_KEYWORD_BLEND,
    TOKEN_KEYWORD_BOOL,
    TOKEN_KEYWORD_BOUNCE,
    TOKEN_KEYWORD_CAP,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_CORE,
    TOKEN_KEYWORD_DECI,
    TOKEN_KEYWORD_DOUBLE,
    TOKEN_KEYWORD_DROP,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_EMOJI,
    TOKEN_KEYWORD_EMPTY,
    TOKEN_KEYWORD_ENUM,
    TOKEN_KEYWORD_FAM,
    TOKEN_KEYWORD_FIXED,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_GRAB,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_IMPORT,
    TOKEN_KEYWORD_LENGTH,
    TOKEN_KEYWORD_LETT,
    TOKEN_KEYWORD_MAXI,
    TOKEN_KEYWORD_MINI,
    TOKEN_KEYWORD_MATIC,
    TOKEN_KEYWORD_NEXT,
    TOKEN_KEYWORD_NOCAP,
    TOKEN_KEYWORD_NORM,
    TOKEN_KEYWORD_NUMBS,
    TOKEN_KEYWORD_OUT,
    TOKEN_KEYWORD_SHADY,
    TOKEN_KEYWORD_SPILL,
    TOKEN_KEYWORD_STAY,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_SWIM,
    TOKEN_KEYWORD_SWITCH,
    TOKEN_KEYWORD_TAG,
    TOKEN_KEYWORD_TEXT,
    TOKEN_KEYWORD_VIBE,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_ZAVED,

    //FUNCTIONS 
    TOKEN_FUNCTION_AVG,
    TOKEN_FUNCTION_ASCENDING,
    TOKEN_FUNCTION_DESCENDING,
    TOKEN_FUNCTION_MAX,
    TOKEN_FUNCTION_MIN,
    TOKEN_FUNCTION_FINDSTRING,

    //DELIMITERS
    TOKEN_DELIMITER_SEMICOLON,
    TOKEN_DELIMITER_LPAREN,
    TOKEN_DELIMITER_RPAREN,
    TOKEN_DELIMITER_LBRACE,
    TOKEN_DELIMITER_RBRACE,
    TOKEN_DELIMITER_LBRACKET,
    TOKEN_DELIMITER_RBRACKET,
    TOKEN_DELIMITER_COMMA,
    TOKEN_DELIMITER_QUOTE,

    TOKEN_ERROR,
    TOKEN_EOF 
} TokenType;


typedef struct {
    TokenType type;
    char* lexeme;
    int line;
} Token;

//LEXER STATE
const char* source_code;
int start = 0;
int current = 0;
int line = 1;
int tokenStartLine = 1;

Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.line = tokenStartLine;
    int length = current - start;
    token.lexeme = (char*)malloc(length + 1);
    strncpy(token.lexeme, source_code + start, length);
    token.lexeme[length] = '\0';

    if (length > 0 && token.lexeme[length - 1] == '\r') {
        token.lexeme[length - 1] = '\0';
    }

    return token;
}

Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.line = line;
    token.lexeme = (char*)malloc(strlen(message) + 1);
    strcpy(token.lexeme, message);

    return token;
}

int isAtEnd() {
    return source_code[current] == '\0';
}

char nextChar() {
    current++;
    return source_code[current - 1];
}

char peek() {
    return source_code[current]; //CHECK LANG NG CURRENT CHAR
}

char peekNext() {
    if (isAtEnd()) return '\0';
    return source_code[current + 1]; //CHECK LANG NG NEXT NG CURRENT CHAR
}


int match(char expected) {
    if (isAtEnd()) return 0;
    if (source_code[current] != expected) return 0;
    
    current++;
    return 1;
}

//TOKENIZING FUNCTIONS
void skipWhitespace() {
    while (1) {
        char c = peek();
        switch (c) {
            //WHITESPACE
            case ' ':
            case '\r':
            case '\t':
                nextChar();
                break;
            //NEWLINE
            case '\n':
                line++;
                nextChar();
                break;
            
            default:
                return;
        }
    }
}

Token handleString() {
    start = current; 
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++; // IF MULTI LINE YUNG STRING
        nextChar();
    }
    if (isAtEnd()) return errorToken("Unterminated string.");

    Token token = makeToken(TOKEN_KEYWORD_TEXT);
    nextChar(); 
    return token;
}

Token handleCharacter() {
    start = current;
    if (!isAtEnd()) {
        nextChar();
    }
    Token token = makeToken(TOKEN_KEYWORD_EMOJI);
    if (isAtEnd() || peek() != '\'') {
        free(token.lexeme); 
        return errorToken("Unterminated or malformed char literal.");
    }
    nextChar();
    return token;
}

Token handleNumber() {
    while (isdigit(peek())) {
        nextChar();
    }
    //FOR FLOATING POINT NUMBERS
    if (peek() == '.' && isdigit(peekNext())) { 
        nextChar();
        while (isdigit(peek())) {
            nextChar();
        }
        return makeToken(TOKEN_KEYWORD_SWIM);
    }
    return makeToken(TOKEN_KEYWORD_NUMBS);
}


TokenType checkKeyword() {
    int length = current - start;
    char startChar = source_code[start];

    switch (startChar) {
        //FUNCTION KEYWORDS CHECKER
        case 'a':
            if (length == 3 && strncmp(source_code + start, "avg", 3) == 0) return TOKEN_FUNCTION_AVG;
            if (length == 9 && strncmp(source_code + start, "ascending", 9) == 0) return TOKEN_FUNCTION_ASCENDING;
            break;
        case 'd':
            if (length == 10 && strncmp(source_code + start, "descending", 10) == 0) return TOKEN_FUNCTION_DESCENDING;
            break;
        case 'f':
            if (length == 10 && strncmp(source_code + start, "findString", 10) == 0) return TOKEN_FUNCTION_FINDSTRING;
            break;
        case 'm':
            if (length == 3 && strncmp(source_code + start, "max", 3) == 0) return TOKEN_FUNCTION_MAX;
            if (length == 3 && strncmp(source_code + start, "min", 3) == 0) return TOKEN_FUNCTION_MIN;
            break;
        case 's':
            break;
        //KEYWORDS CHECKER
        case 'A':
            if (length == 5 && strncmp(source_code + start, "ALIAS", 5) == 0) return TOKEN_KEYWORD_ALIAS;
            break;
        case 'B':
            if (length == 5 && strncmp(source_code + start, "BLEND", 5) == 0) return TOKEN_KEYWORD_BLEND;
            if (length == 4 && strncmp(source_code + start, "BOOL", 4) == 0) return TOKEN_KEYWORD_BOOL;
            if (length == 6 && strncmp(source_code + start, "BOUNCE", 6) == 0) return TOKEN_KEYWORD_BOUNCE;
            break;
        case 'C':
            if (length == 3 && strncmp(source_code + start, "CAP", 3) == 0) return TOKEN_KEYWORD_CAP;
            if (length == 4 && strncmp(source_code + start, "CASE", 4) == 0) return TOKEN_KEYWORD_CASE;
            if (length == 4 && strncmp(source_code + start, "CORE", 4) == 0) return TOKEN_KEYWORD_CORE;
            break;
        case 'D':
            if (length == 4 && strncmp(source_code + start, "DECI", 4) == 0) return TOKEN_KEYWORD_DECI;
            if (length == 6 && strncmp(source_code + start, "DOUBLE", 6) == 0) return TOKEN_KEYWORD_DOUBLE;
            if (length == 4 && strncmp(source_code + start, "DROP", 4) == 0) return TOKEN_KEYWORD_DROP;
            break;
        case 'E':
            if (length == 4 && strncmp(source_code + start, "ELSE", 4) == 0) return TOKEN_KEYWORD_ELSE;
            if (length == 5 && strncmp(source_code + start, "EMOJI", 5) == 0) return TOKEN_KEYWORD_EMOJI;
            if (length == 5 && strncmp(source_code + start, "EMPTY", 5) == 0) return TOKEN_KEYWORD_EMPTY;
            if (length == 4 && strncmp(source_code + start, "ENUM", 4) == 0) return TOKEN_KEYWORD_ENUM;
            break;
        case 'F':
            if (length == 3 && strncmp(source_code + start, "FAM", 3) == 0) return TOKEN_KEYWORD_FAM;
            if (length == 5 && strncmp(source_code + start, "FIXED", 5) == 0) return TOKEN_KEYWORD_FIXED;
            if (length == 3 && strncmp(source_code + start, "FOR", 3) == 0) return TOKEN_KEYWORD_FOR;
            break;
        case 'G':
            if (length == 4 && strncmp(source_code + start, "GRAB", 4) == 0) return TOKEN_KEYWORD_GRAB;
            break;
        case 'I':
            if (length == 2 && strncmp(source_code + start, "IF", 2) == 0) return TOKEN_KEYWORD_IF;
            if (length == 6 && strncmp(source_code + start, "IMPORT", 6) == 0) return TOKEN_KEYWORD_IMPORT;
            break;
        case 'L':
            if (length == 6 && strncmp(source_code + start, "LENGTH", 6) == 0) return TOKEN_KEYWORD_LENGTH;
            if (length == 4 && strncmp(source_code + start, "LETT", 4) == 0) return TOKEN_KEYWORD_LETT;
            break;
        case 'M':
            if (length == 4 && strncmp(source_code + start, "MAXI", 4) == 0) return TOKEN_KEYWORD_MAXI;
            if (length == 4 && strncmp(source_code + start, "MINI", 4) == 0) return TOKEN_KEYWORD_MINI;
            if (length == 5 && strncmp(source_code + start, "MATIC", 5) == 0) return TOKEN_KEYWORD_MATIC;
            break;
        case 'N':
            if (length == 4 && strncmp(source_code + start, "NEXT", 4) == 0) return TOKEN_KEYWORD_NEXT;
            if (length == 5 && strncmp(source_code + start, "NOCAP", 5) == 0) return TOKEN_KEYWORD_NOCAP;
            if (length == 4 && strncmp(source_code + start, "NORM", 4) == 0) return TOKEN_KEYWORD_NORM;
            if (length == 5 && strncmp(source_code + start, "NUMBS", 5) == 0) return TOKEN_KEYWORD_NUMBS;
            break;
        case 'O':
            if (length == 3 && strncmp(source_code + start, "OUT", 3) == 0) return TOKEN_KEYWORD_OUT;
            break;
        case 'S':
            if (length == 5 && strncmp(source_code + start, "SHADY", 5) == 0) return TOKEN_KEYWORD_SHADY;
            if (length == 5 && strncmp(source_code + start, "SPILL", 5) == 0) return TOKEN_KEYWORD_SPILL; 
            if (length == 4 && strncmp(source_code + start, "STAY", 4) == 0) return TOKEN_KEYWORD_STAY;
            if (length == 6 && strncmp(source_code + start, "STRUCT", 6) == 0) return TOKEN_KEYWORD_STRUCT;
            if (length == 4 && strncmp(source_code + start, "SWIM", 4) == 0) return TOKEN_KEYWORD_SWIM;
            if (length == 6 && strncmp(source_code + start, "SWITCH", 6) == 0) return TOKEN_KEYWORD_SWITCH;
            break;
        case 'T':
            if (length == 3 && strncmp(source_code + start, "TAG", 3) == 0) return TOKEN_KEYWORD_TAG;
            if (length == 4 && strncmp(source_code + start, "TEXT", 4) == 0) return TOKEN_KEYWORD_TEXT;
            break;
        case 'V':
            if (length == 4 && strncmp(source_code + start, "VIBE", 4) == 0) return TOKEN_KEYWORD_VIBE;
            break;
        case 'W':
            if (length == 5 && strncmp(source_code + start, "WHILE", 5) == 0) return TOKEN_KEYWORD_WHILE;
            break;
        case 'Z':
            if (length == 5 && strncmp(source_code + start, "ZAVED", 5) == 0) return TOKEN_KEYWORD_ZAVED;
            break;
    }
    
    return TOKEN_IDENTIFIER;
}

Token handleIdentifier() { // IDENTIFIER RULE: (CAPS | SM) (CAPS | SM | _ | DIGITS)
    while (isalpha(peek()) || isdigit(peek()) || peek() == '_') {
        nextChar();
    }
    return makeToken(checkKeyword());
}

//MAIN TOKEN SCANNER FUNCTION
Token scanToken() {
    skipWhitespace();
    start = current;
    tokenStartLine = line;
    if (isAtEnd()) {
        return makeToken(TOKEN_EOF);
    }

    char c = nextChar();
    if (isalpha(c)) {
        return handleIdentifier();
    }
    
    if (isdigit(c)) {
        return handleNumber();
    }

    //DELIMS AND OPERATOR CHECKER
    switch (c) {
        //SINGLE CHAR DELIMS
        case '(': return makeToken(TOKEN_DELIMITER_LPAREN);
        case ')': return makeToken(TOKEN_DELIMITER_RPAREN);
        case '{': return makeToken(TOKEN_DELIMITER_LBRACE);
        case '}': return makeToken(TOKEN_DELIMITER_RBRACE);
        case '[': return makeToken(TOKEN_DELIMITER_LBRACKET);
        case ']': return makeToken(TOKEN_DELIMITER_RBRACKET);
        case ';': return makeToken(TOKEN_DELIMITER_SEMICOLON);
        case ',': return makeToken(TOKEN_DELIMITER_COMMA);
        // STRING LITERAL
        case '"': return handleString();
        // CHAR LITERAL
        case '\'': return handleCharacter();
        // OPERATORS
        case '+':
            if (match('=')) return makeToken(TOKEN_OP_PLUS_ASSIGN);
            if (match('+')) return makeToken(TOKEN_OP_INCREMENT);
            return makeToken(TOKEN_OP_PLUS);
        case '-':
            if (match('=')) return makeToken(TOKEN_OP_MINUS_ASSIGN);
            if (match('-')) return makeToken(TOKEN_OP_DECREMENT);
            return makeToken(TOKEN_OP_MINUS);
        case '*':
            return makeToken(match('=') ? TOKEN_OP_MULTIPLY_ASSIGN : TOKEN_OP_MULTIPLY);
        //COMMENTS
        case '/':
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) {
                    nextChar();
                }
                return makeToken(TOKEN_COMMENT);
            } else if (match('*')) {
                while (!(peek() == '*' && peekNext() == '/') && !isAtEnd()) {
                    if (peek() == '\n') line++;
                    nextChar();
                }
                if (isAtEnd()) return errorToken("Unterminated multi-line comment.");
                nextChar();
                nextChar();
                return makeToken(TOKEN_COMMENT);
            } else {
                return makeToken(match('=') ? TOKEN_OP_DIVIDE_ASSIGN : TOKEN_OP_DIVIDE);
            }

        case '%':
            return makeToken(match('=') ? TOKEN_OP_MOD_ASSIGN : TOKEN_OP_MODULO);
        case '^':
            return makeToken(match('=') ? TOKEN_OP_XOR_ASSIGN : TOKEN_ERROR);
        case '=':
            return makeToken(match('=') ? TOKEN_OP_EQUAL : TOKEN_OP_ASSIGN);
        case '!':
            return makeToken(match('=') ? TOKEN_OP_NOT_EQUAL : TOKEN_OP_NOT);
        case '<':
            return makeToken(match('=') ? TOKEN_OP_LESS_EQUAL : TOKEN_OP_LESS);
        case '>':
            return makeToken(match('=') ? TOKEN_OP_GREATER_EQUAL : TOKEN_OP_GREATER);
        case '&':
            if (match('=')) return makeToken(TOKEN_OP_AND_ASSIGN);
            if (match('&')) return makeToken(TOKEN_OP_AND);
            return errorToken("Unexpected character '&'. Did you mean '&&' or '&='?");
        case '|':
            if (match('=')) return makeToken(TOKEN_OP_OR_ASSIGN);
            if (match('|')) return makeToken(TOKEN_OP_OR);
            return errorToken("Unexpected character '|'. Did you mean '||' or '|='?");
    }
    return errorToken("Unexpected character.");
}


const char* tokenTypeToString(TokenType type) {
    switch (type) {
        //LITERALS
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_COMMENT:    return "TOKEN_COMMENT";

        //OPERATORS
        case TOKEN_OP_PLUS:           return "TOKEN_OP_PLUS";
        case TOKEN_OP_MINUS:          return "TOKEN_OP_MINUS";
        case TOKEN_OP_MULTIPLY:       return "TOKEN_OP_MULTIPLY";
        case TOKEN_OP_DIVIDE:         return "TOKEN_OP_DIVIDE";
        case TOKEN_OP_MODULO:         return "TOKEN_OP_MODULO";
        case TOKEN_OP_ASSIGN:         return "TOKEN_OP_ASSIGN";
        case TOKEN_OP_PLUS_ASSIGN:    return "TOKEN_OP_PLUS_ASSIGN";
        case TOKEN_OP_MINUS_ASSIGN:   return "TOKEN_OP_MINUS_ASSIGN";
        case TOKEN_OP_MULTIPLY_ASSIGN:return "TOKEN_OP_MULTIPLY_ASSIGN";
        case TOKEN_OP_DIVIDE_ASSIGN:  return "TOKEN_OP_DIVIDE_ASSIGN";
        case TOKEN_OP_MOD_ASSIGN:     return "TOKEN_OP_MOD_ASSIGN";
        case TOKEN_OP_XOR_ASSIGN:     return "TOKEN_OP_XOR_ASSIGN";
        case TOKEN_OP_AND_ASSIGN:     return "TOKEN_OP_AND_ASSIGN";
        case TOKEN_OP_OR_ASSIGN:      return "TOKEN_OP_OR_ASSIGN";
        case TOKEN_OP_LESS:           return "TOKEN_OP_LESS";
        case TOKEN_OP_GREATER:        return "TOKEN_OP_GREATER";
        case TOKEN_OP_LESS_EQUAL:     return "TOKEN_OP_LESS_EQUAL";
        case TOKEN_OP_GREATER_EQUAL:  return "TOKEN_OP_GREATER_EQUAL";
        case TOKEN_OP_EQUAL:          return "TOKEN_OP_EQUAL";
        case TOKEN_OP_NOT_EQUAL:      return "TOKEN_OP_NOT_EQUAL";
        case TOKEN_OP_NOT:            return "TOKEN_OP_NOT";
        case TOKEN_OP_AND:            return "TOKEN_OP_AND";
        case TOKEN_OP_OR:             return "TOKEN_OP_OR";
        case TOKEN_OP_INCREMENT:      return "TOKEN_OP_INCREMENT";
        case TOKEN_OP_DECREMENT:      return "TOKEN_OP_DECREMENT";

        //KEYWORDS
        case TOKEN_KEYWORD_ALIAS:     return "TOKEN_KEYWORD_ALIAS";
        case TOKEN_KEYWORD_BLEND:     return "TOKEN_KEYWORD_BLEND";
        case TOKEN_KEYWORD_BOOL:      return "TOKEN_KEYWORD_BOOL";
        case TOKEN_KEYWORD_BOUNCE:    return "TOKEN_KEYWORD_BOUNCE";
        case TOKEN_KEYWORD_CAP:       return "TOKEN_KEYWORD_CAP";
        case TOKEN_KEYWORD_CASE:      return "TOKEN_KEYWORD_CASE";
        case TOKEN_KEYWORD_CORE:      return "TOKEN_KEYWORD_CORE";
        case TOKEN_KEYWORD_DECI:      return "TOKEN_KEYWORD_DECI";
        case TOKEN_KEYWORD_DOUBLE:    return "TOKEN_KEYWORD_DOUBLE";
        case TOKEN_KEYWORD_DROP:      return "TOKEN_KEYWORD_DROP";
        case TOKEN_KEYWORD_ELSE:      return "TOKEN_KEYWORD_ELSE";
        case TOKEN_KEYWORD_EMOJI:     return "TOKEN_KEYWORD_EMOJI";
        case TOKEN_KEYWORD_EMPTY:     return "TOKEN_KEYWORD_EMPTY";
        case TOKEN_KEYWORD_ENUM:      return "TOKEN_KEYWORD_ENUM";
        case TOKEN_KEYWORD_FAM:       return "TOKEN_KEYWORD_FAM";
        case TOKEN_KEYWORD_FIXED:     return "TOKEN_KEYWORD_FIXED";
        case TOKEN_KEYWORD_FOR:       return "TOKEN_KEYWORD_FOR";
        case TOKEN_KEYWORD_GRAB:      return "TOKEN_KEYWORD_GRAB";
        case TOKEN_KEYWORD_IF:        return "TOKEN_KEYWORD_IF";
        case TOKEN_KEYWORD_IMPORT:    return "TOKEN_KEYWORD_IMPORT";
        case TOKEN_KEYWORD_LENGTH:    return "TOKEN_KEYWORD_LENGTH";
        case TOKEN_KEYWORD_LETT:      return "TOKEN_KEYWORD_LETT";
        case TOKEN_KEYWORD_MAXI:      return "TOKEN_KEYWORD_MAXI";
        case TOKEN_KEYWORD_MINI:      return "TOKEN_KEYWORD_MINI";
        case TOKEN_KEYWORD_MATIC:     return "TOKEN_KEYWORD_MATIC";
        case TOKEN_KEYWORD_NEXT:      return "TOKEN_KEYWORD_NEXT";
        case TOKEN_KEYWORD_NOCAP:     return "TOKEN_KEYWORD_NOCAP";
        case TOKEN_KEYWORD_NORM:      return "TOKEN_KEYWORD_NORM";
        case TOKEN_KEYWORD_NUMBS:     return "TOKEN_KEYWORD_NUMBS";
        case TOKEN_KEYWORD_OUT:       return "TOKEN_KEYWORD_OUT";
        case TOKEN_KEYWORD_SHADY:     return "TOKEN_KEYWORD_SHADY";
        case TOKEN_KEYWORD_SPILL:     return "TOKEN_KEYWORD_SPILL";
        case TOKEN_KEYWORD_STAY:      return "TOKEN_KEYWORD_STAY";
        case TOKEN_KEYWORD_STRUCT:    return "TOKEN_KEYWORD_STRUCT";
        case TOKEN_KEYWORD_SWIM:      return "TOKEN_KEYWORD_SWIM";
        case TOKEN_KEYWORD_SWITCH:    return "TOKEN_KEYWORD_SWITCH";
        case TOKEN_KEYWORD_TAG:       return "TOKEN_KEYWORD_TAG";
        case TOKEN_KEYWORD_TEXT:      return "TOKEN_KEYWORD_TEXT";
        case TOKEN_KEYWORD_VIBE:      return "TOKEN_KEYWORD_VIBE";
        case TOKEN_KEYWORD_WHILE:     return "TOKEN_KEYWORD_WHILE";
        case TOKEN_KEYWORD_ZAVED:     return "TOKEN_KEYWORD_ZAVED";

        //FUNCTIONS
        case TOKEN_FUNCTION_AVG:        return "TOKEN_FUNCTION_AVG";
        case TOKEN_FUNCTION_ASCENDING:  return "TOKEN_FUNCTION_ASCENDING";
        case TOKEN_FUNCTION_DESCENDING: return "TOKEN_FUNCTION_DESCENDING";
        case TOKEN_FUNCTION_MAX:        return "TOKEN_FUNCTION_MAX";
        case TOKEN_FUNCTION_MIN:        return "TOKEN_FUNCTION_MIN";
        case TOKEN_FUNCTION_FINDSTRING: return "TOKEN_FUNCTION_FINDSTRING";

        //DELIMITERS
        case TOKEN_DELIMITER_SEMICOLON: return "TOKEN_DELIMITER_SEMICOLON";
        case TOKEN_DELIMITER_LPAREN:    return "TOKEN_DELIMITER_LPAREN";
        case TOKEN_DELIMITER_RPAREN:    return "TOKEN_DELIMITER_RPAREN";
        case TOKEN_DELIMITER_LBRACE:    return "TOKEN_DELIMITER_LBRACE";
        case TOKEN_DELIMITER_RBRACE:    return "TOKEN_DELIMITER_RBRACE";
        case TOKEN_DELIMITER_LBRACKET:  return "TOKEN_DELIMITER_LBRACKET";
        case TOKEN_DELIMITER_RBRACKET:  return "TOKEN_DELIMITER_RBRACKET";
        case TOKEN_DELIMITER_COMMA:     return "TOKEN_DELIMITER_COMMA";
        case TOKEN_DELIMITER_QUOTE:     return "TOKEN_DELIMITER_QUOTE";
        
        //OTHERS
        case TOKEN_ERROR: return "TOKEN_ERROR";
        case TOKEN_EOF:   return "TOKEN_EOF";
        default:          return "TOKEN_UNKNOWN";
    }
}

bool endsWith(const char* str, const char* suffix) {
    if (!str || !suffix) {
        return false;
    }
    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    if (len_suffix > len_str) {
        return false;
    }
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        return NULL;
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read entire file \"%s\"..\n", path);
        free(buffer);
        return NULL;
    }
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}


int main(int argc, char* argv[]) {
    if (argc != 2 || !endsWith(argv[1], ".zlang")) {
        printf("Error: Please provide one .zlang file.\n");
        printf("Usage: ./lexer file.zlang\n");
        return 1;
    }

    char* buffer = readFile(argv[1]);
    if (buffer == NULL) {
        return 1;
    }

    source_code = buffer;
    start = 0;
    current = 0;
    line = 1;

    printf("--- Tokenizing %s ---\n\n", argv[1]);
    printf("%-5s %-25s %s\n", "Line", "Token Type", "Lexeme");
    printf("------------------------------------------------------\n");

    Token token;
    do {
        token = scanToken();
        printf("%-5d %-25s '%s'\n",
               token.line, 
               tokenTypeToString(token.type), 
               token.lexeme);
        free(token.lexeme);
    } while (token.type != TOKEN_EOF);

    printf("------------------------------------------------------\n");
    printf("Lexical analysis complete.\n");
    free(buffer);
    
    return 0;
}