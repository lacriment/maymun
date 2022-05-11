#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "language.h"
#include "lexer.h"

enum {
	T_ILLEGAL,
	T_EOF,

	// Identifiers, literals
	T_IDENTITY,  // 
	T_INT,       // 1,2,3,...

	// Operators
	T_ASSIGN,    // =
	T_PLUS,      // +
	T_MINUS,     // -
	T_ASTERIKS,  // *
	T_DIVIDE,    // /

	// Delimiters
	T_DOT,       // .
	T_COMMA,     // ,
	T_SEMICOLON, // ;

	T_LPAREN,    // (
	T_RPAREN,    // )
	T_LBRACE,    // {
	T_RBRACE,    // }
	T_LBRACKET,  // [
	T_RBRACKET,  // ]

	// Reserved keywords
	T_FUNCTION,
	T_LET
};

typedef char* TokenType;
typedef struct Token Token;
typedef struct Lexer Lexer;

struct Token {
	TokenType token_type;
	char* literal;
};

struct Lexer {
	char* input;
	size_t input_length;
	int position; // current position in input
	int read_position; // position + 1
	char current;
};

Lexer* lexer_init(Lexer *lexer, char* input) {
	lexer->input = input;
	lexer->input_length = strlen(input);
	assert(lexer->input_length > 0);
	lexer->current = lexer->input[0];
	lexer->position = 0;
	lexer->read_position = 1;
}

void lexer_advance(Lexer* lexer) {
	if (lexer->read_position >= lexer->input_length) {
		lexer->current = 0;
	}
	else {
		lexer->current = lexer->input[lexer->read_position];
	}
	lexer->position = lexer->read_position;
	lexer->read_position++;
}

Token lexer_next_token(Lexer* lexer) {
	Token token;

	switch (lexer->current)
	{
	case '=':
		token.token_type = T_ASSIGN;
		token.literal = '=';
		break;
	case '+':
		token.token_type = T_PLUS;
		token.literal = '+';
		break;
	case '-':
		token.token_type = T_MINUS;
		token.literal = '-';
		break;
	case '*':
		token.token_type = T_ASTERIKS;
		token.literal = '*';
		break;
	case '/':
		token.token_type = T_DIVIDE;
		token.literal = '/';
		break;
	case '.':
		token.token_type = T_DOT;
		token.literal = '.';
		break;
	case ',':
		token.token_type = T_COMMA;
		token.literal = ',';
		break;
	case ';':
		token.token_type = T_SEMICOLON;
		token.literal = ';';
		break;
	case '(':
		token.token_type = T_LPAREN;
		token.literal = '(';
		break;
	case ')':
		token.token_type = T_RPAREN;
		token.literal = ')';
		break;
	case '{':
		token.token_type = T_LBRACE;
		token.literal = '{';
		break;
	case '}':
		token.token_type = T_RBRACE;
		token.literal = '}';
		break;
	case '[':
		token.token_type = T_LBRACKET;
		token.literal = '[';
		break;
	case ']':
		token.token_type = T_RBRACKET;
		token.literal = ']';
		break;
	case 0:
		token.token_type = T_EOF;
		token.literal = 0;
		break;
	default:
		if (is_letter(lexer->current)) {
			
		}
		token.token_type = T_ILLEGAL;
		token.literal = 0; 
	}
	lexer_advance(lexer);
	return token;
};


void test_next_token() {
	printf("TEST STARTED: next_token()\n");
	char* input =
		"let five = 5;"
		"let ten = 10;"
		""
		"let add = fn(x, y) {"
		"	x + y;"
		"};"
		""
		"let result = add(five, ten);";
	Lexer lexer;
	lexer_init(&lexer, input);
	assert(lexer_next_token(&lexer).token_type == T_LET);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_ASSIGN);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_LET);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_ASSIGN);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_LET);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_ASSIGN);
	assert(lexer_next_token(&lexer).token_type == T_FUNCTION);
	assert(lexer_next_token(&lexer).token_type == T_LPAREN);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_COMMA);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_RPAREN);
	assert(lexer_next_token(&lexer).token_type == T_LBRACE);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_PLUS);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_RBRACE);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_LET);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_ASSIGN);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_LPAREN);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_COMMA);
	assert(lexer_next_token(&lexer).token_type == T_IDENTITY);
	assert(lexer_next_token(&lexer).token_type == T_RPAREN);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	printf("TEST SUCCESS: next_token()\n");
}

int main(int argc, char** argv) {
	printf("Maymun v0.1\n");
	test_next_token();
	return 0;
}