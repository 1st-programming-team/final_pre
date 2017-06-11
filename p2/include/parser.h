#ifndef POLY__PARSER_HEADER_FILE___
#define POLY__PARSER_HEADER_FILE___
#include "poly.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define register_ret_type(T) \
    typedef struct {\
        int error_code;\
        T data;\
    } T##_data

register_ret_type(poly);
register_ret_type(int);

poly_data get_poly(tokens *token_list);
poly_data get_expr2(tokens *token_list);
poly_data get_expr1(tokens *token_list);
poly_data get_expr(tokens *token_list);

#endif
