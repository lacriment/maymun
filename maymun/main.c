#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "language.h"
#include "lexer.h"

enum {
	T_ILLEGAL = 0,
	T_EOF = 1,

	// Identifiers, literals
	T_IDENTITY,      // 
	T_INT,           // 1,2,3,...

	// Operators
	T_ASSIGN,        // =
	T_PLUS,          // +
	T_MINUS,         // -
	T_ASTERISK,      // *
	T_DIVIDE,        // /
	T_BANG,          // !
	T_LESS_THAN,     // <
	T_GREATER_THAN,  // >
	T_EQUALS,        // ==
	T_NOT_EQUALS,    // !=

	// Delimiters
	T_DOT,           // .
	T_COMMA,         // ,
	T_SEMICOLON,     // ;

	T_LPAREN,        // (
	T_RPAREN,        // )
	T_LBRACE,        // {
	T_RBRACE,        // }
	T_LBRACKET,      // [
	T_RBRACKET,      // ]

	// Reserved keywords
	T_FUNCTION,
	T_LET,
	T_IF,
	T_ELSE,
	T_RETURN,
	T_TRUE,
	T_FALSE
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

Lexer* lexer_init(Lexer *lexer, const char* input) {
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

char lexer_peek_char(Lexer* lexer) {
	if (lexer->read_position >= lexer->input_length) {
		return 0;
	} else {
		return lexer->input[lexer->read_position];
	}
}

char* str_slice(const char* str, int start, int end)
{
	unsigned int size = end - start;
	char* output = (char*)malloc(size * sizeof(char) + 1);

	for (unsigned int i = 0; i < size; i++)
	{
		output[i] = str[start + i];
	}
	output[size] = '\0';

	return output;
}

bool str_comp(const char* s1, const char* s2) {
	while (*s1 && *s2) {
		if (*s1 == *s2) {
			s1++;
			s2++;
		}
		else {
			return false;
		}
	}
	return *s1 == *s2;
}

char* lexer_read_identifier(Lexer* lexer) {
	int position = lexer->position;
	while (is_letter(lexer->current)) {
		lexer_advance(lexer);
	}
	return str_slice(lexer->input, position, lexer->position);
}

char* lexer_read_number(Lexer* lexer) {
	int position = lexer->position;
	while (is_digit(lexer->current)) {
		lexer_advance(lexer);
	}
	return str_slice(lexer->input, position, lexer->position);
}

TokenType lexer_lookup_ident(const char* str) {
	if (str_comp(str, "let") == 1) {
		return T_LET;
	} else if (str_comp(str, "function")) {
		return T_FUNCTION;
	} else if (str_comp(str, "if")) {
		return T_IF;
	} else if (str_comp(str, "else")) {
		return T_ELSE;
	} else if (str_comp(str, "return")) {
		return T_RETURN;
	} else if (str_comp(str, "true")) {
		return T_TRUE;
	} else if (str_comp(str, "false")) {
		return T_FALSE;
	} else {
		return T_IDENTITY;
	}
}

void lexer_skip_whitespaces(Lexer* lexer) {
	while (lexer->current == ' ' || lexer->current == '\t' || lexer->current == '\n' || lexer->current == '\r')
	{
		lexer_advance(lexer);
	}
}

Token lexer_next_token(Lexer* lexer) {
	Token token;
	lexer_skip_whitespaces(lexer);

	switch (lexer->current)
	{
	case '=':
		if (lexer_peek_char(lexer) == '=') {
			token.token_type = T_EQUALS;
			token.literal = '==';
			lexer_advance(lexer);
		}
		else {
			token.token_type = T_ASSIGN;
			token.literal = '=';
		}
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
		token.token_type = T_ASTERISK;
		token.literal = '*';
		break;
	case '/':
		token.token_type = T_DIVIDE;
		token.literal = '/';
		break;
	case '!':
		if (lexer_peek_char(lexer) == '=') {
			token.token_type = T_NOT_EQUALS;
			token.literal = '!=';
			lexer_advance(lexer);
		}
		else {
			token.token_type = T_BANG;
			token.literal = '!';
		}
		break;
	case '<':
		token.token_type = T_LESS_THAN;
		token.literal = '<';
		break;
	case '>':
		token.token_type = T_GREATER_THAN;
		token.literal = '>';
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
			token.literal = lexer_read_identifier(lexer);
			token.token_type = lexer_lookup_ident(token.literal);
			return token;
		}
		else if (is_digit(lexer->current)) {
			token.literal = lexer_read_number(lexer);
			token.token_type = T_INT;
			return token;
		}
		else {
			token.token_type = T_ILLEGAL;
			token.literal = 0; 
		}
	}
	lexer_advance(lexer);
	return token;
};


void test_next_token() {
	printf("TEST STARTED: next_token()\n");
	char* input = "let five = 5;"
		"let ten = 10;"
		""
		"let add = function(x, y) {"
		"	x + y;"
		"};"
		""
		"let result = add(five, ten);"
		"!-/*5;"
		"5 < 10 > 5;"
		"if (5 < 10) {"
		"  return true;"
		"} else {"
		"  return false;"
		"}"
		""
		"10 == 10;"
		"10 != 9;";
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
	assert(lexer_next_token(&lexer).token_type == T_BANG);
	assert(lexer_next_token(&lexer).token_type == T_MINUS);
	assert(lexer_next_token(&lexer).token_type == T_DIVIDE);
	assert(lexer_next_token(&lexer).token_type == T_ASTERISK);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_LESS_THAN);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_GREATER_THAN);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_IF);
	assert(lexer_next_token(&lexer).token_type == T_LPAREN);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_LESS_THAN);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_RPAREN);
	assert(lexer_next_token(&lexer).token_type == T_LBRACE);
	assert(lexer_next_token(&lexer).token_type == T_RETURN);
	assert(lexer_next_token(&lexer).token_type == T_TRUE);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_RBRACE);
	assert(lexer_next_token(&lexer).token_type == T_ELSE);
	assert(lexer_next_token(&lexer).token_type == T_LBRACE);
	assert(lexer_next_token(&lexer).token_type == T_RETURN);
	assert(lexer_next_token(&lexer).token_type == T_FALSE);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_RBRACE);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_EQUALS);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_NOT_EQUALS);
	assert(lexer_next_token(&lexer).token_type == T_INT);
	assert(lexer_next_token(&lexer).token_type == T_SEMICOLON);

	/*
	"if (5 < 10) {"
	"  return true;"
	"} else {"
	"  return false;"
	"}"
	""
	"10 == 10;"
	"10 != 9;"
	*/
	printf("TEST SUCCESS: next_token()\n");
}

int main(int argc, char** argv) {
	printf("Maymun v0.1\n");
	test_next_token();
	return 0;
}