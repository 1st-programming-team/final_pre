#include "lexer.h"

#define is_number(ch) \
	(((ch)<='9')&&((ch)>='0'))
#define is_word(ch) \
	(((ch)<='Z')&&((ch)>='A')||\
	 ((ch)<='z')&&((ch)>='a')||\
	 ((ch)=='_'))

#define move_pointer(ptr)\
	do {\
		while (**(ptr) &&\
			(**(ptr) == ' '||**(ptr) == '\t'||**(ptr) == '\n'||**(ptr) == '\r')) \
		(*(ptr))++;\
	} while(0)

#define return__token(n, Ty) \
    { (*pbuffer)+=(n); move_pointer(pbuffer); return Ty; } break

#define return_token(n, Ty) \
    return__token(n, Ty)

static int get_token(char **pbuffer)
{
	switch (**pbuffer) {
		case '[': return_token(1, TOK_M_BRAK_O);
		case ']': return_token(1, TOK_M_BRAK_C);
		case '(': return_token(1, TOK_S_BRAK_O);
		case ')': return_token(1, TOK_S_BRAK_C);
		case '+': return_token(1, TOK_ADD);
		case '-': return_token(1, TOK_SUB);
		case '*': return_token(1, TOK_MUL);
		case '/': return_token(1, TOK_DIV);
		case '^': return_token(1, TOK_POW);
		default: {
			if (is_number(**pbuffer)) {
				char *num_ptr = *pbuffer;
				(*pbuffer)++;
				while (is_number(**pbuffer)) {
					(*pbuffer)++;
				}
				char ch = **pbuffer;
				**pbuffer = 0;
				int num = 0;
				sscanf(num_ptr, "%d", &num);
				**pbuffer = ch;
				move_pointer(pbuffer);
				return num;
			} else if (is_word(**pbuffer)) {
				(*pbuffer)++;
				while (is_word(**pbuffer)||is_number(**pbuffer)) {
					(*pbuffer)++;
				}
				move_pointer(pbuffer);
				return TOK_WORD;
			} else {
				return LEX_ERROR;
			}
		}
	}
}

tokens lex(char *buffer)
{
	move_pointer(&buffer);
	int capacity = 1;
	tokens result = {
		(int*)malloc(capacity * sizeof(int)),
		0,
		0,
	};
	while (*buffer) {
		int tok = get_token(&buffer);
		if (!~tok) {
			free(result.list);
			result.list = 0;
			return result;
		}
		//printf("%d\n", tok);
		result.list[result.size++] = tok;
		if (result.size == capacity) {
			int *p = (int*)malloc(capacity * 2 * sizeof(int));
			memcpy(p, result.list, capacity * sizeof(int));
			free(result.list);
			result.list = p;
			capacity *= 2;
		}
	}
	return result;
}
